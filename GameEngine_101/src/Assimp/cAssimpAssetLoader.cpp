#include "cAssimpAssetLoader.h"

#include <fstream>
#include <iostream>
#include "..\cMesh.h"
#include "..\cVAOMeshManager.h"

// assimp include files.
#include "assimp/postprocess.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat)
{
    return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,
                     mat.a2, mat.b2, mat.c2, mat.d2,
                     mat.a3, mat.b3, mat.c3, mat.d3,
                     mat.a4, mat.b4, mat.c4, mat.d4);
}

cAssimpAssetLoader::cAssimpAssetLoader()
{
    // To enable loggin, uncoment the next line and all "logInfo" ones
    //this->createAILogger();
    this->scene = NULL;
}

cAssimpAssetLoader::~cAssimpAssetLoader()
{
}

void cAssimpAssetLoader::logInfo(std::string logString)
{
    // Will add message to File with "info" Tag
    Assimp::DefaultLogger::get()->info(logString.c_str());
}

bool cAssimpAssetLoader::Import3DFromFile(const std::string & pFile)
{
    // Check if file exists
    std::ifstream fin(pFile.c_str());
    if (!fin.fail())
    {
        fin.close();
    }
    else
    {
        std::cout << "Couldn't open file: " + pFile;
        //this->logInfo(importer.GetErrorString());
        return false;
    }

    this->scene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if (!this->scene)
    {
        //this->logInfo(importer.GetErrorString());
        return false;
    }
    else
    {
        this->fileName = pFile;
        // Assume the friendlyName is the same as the file, for now
        this->friendlyName = pFile;

        this->mGlobalInverseTransformation = AIMatrixToGLMMatrix(this->scene->mRootNode->mTransformation);
        this->mGlobalInverseTransformation = glm::inverse(this->mGlobalInverseTransformation);

        // Calcualte all the bone things
        if (!this->Initialize())
        {	// There was an issue doing this calculation
            return false;
        }
    }

    // Now we can access the file's contents.
    //this->logInfo("Import of scene " + pFile + " succeeded.");

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

bool cAssimpAssetLoader::loadMeshesIntoVAO(cVAOMeshManager* pVAO,
                                          int shaderID,
                                          std::string meshName, 
                                          bool isPersistent)
{
    //this->logInfo("========= Start loadMeshesIntoVAO ==============");

    if (!recursiveVAOMeshLoader(pVAO, shaderID, meshName,
                                this->scene,
                                this->scene->mRootNode,
                                isPersistent))
    {
        //this->logInfo("========= There was an error loading the mesh ==============");
        return false;
    }

    //this->logInfo("========= loadMeshesIntoVAO Succeeded ==============");
    return true;
}

bool cAssimpAssetLoader::Initialize(void)
{
    this->m_numberOfVertices = this->scene->mMeshes[0]->mNumVertices;

    // This is the vertex information for JUST the bone stuff
    this->vecVertexBoneData.resize(this->m_numberOfVertices);

    this->LoadBones(this->scene->mMeshes[0], this->vecVertexBoneData);

    return true;
}

void cAssimpAssetLoader::LoadBones(const aiMesh * Mesh, std::vector<sVertexBoneData>& vertexBoneData)
{
    for (unsigned int boneIndex = 0; boneIndex != Mesh->mNumBones; boneIndex++)
    {
        unsigned int BoneIndex = 0;
        std::string BoneName(Mesh->mBones[boneIndex]->mName.data);

        //	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> mMapping;
        // 	std::vector<sBoneInfo> mInfo;

        std::map<std::string, unsigned int>::iterator it = this->m_mapBoneNameToBoneIndex.find(BoneName);
        if (it == this->m_mapBoneNameToBoneIndex.end())
        {
            BoneIndex = this->mNumBones;
            this->mNumBones++;
            sBoneInfo bi;
            this->mBoneInfo.push_back(bi);

            this->mBoneInfo[BoneIndex].BoneOffset = AIMatrixToGLMMatrix(Mesh->mBones[boneIndex]->mOffsetMatrix);
            this->m_mapBoneNameToBoneIndex[BoneName] = BoneIndex;
        }
        else
        {
            BoneIndex = it->second;
        }

        for (unsigned int WeightIndex = 0; WeightIndex != Mesh->mBones[boneIndex]->mNumWeights; WeightIndex++)
        {
            unsigned int VertexID = /*mMeshEntries[MeshIndex].BaseVertex +*/ Mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
            float Weight = Mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
            vertexBoneData[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
    return;
}

bool cAssimpAssetLoader::recursiveVAOMeshLoader(cVAOMeshManager* pVAO,
                                               int shaderID,
                                               std::string meshName,
                                               const struct aiScene* sc, 
                                               const struct aiNode* nd,
                                               bool isPersistent)
{
    
    unsigned int meshIndex = 0;
    unsigned int faceIndex = 0;
    unsigned int vertexIndex = 0;

    for (; meshIndex < nd->mNumMeshes; meshIndex++)
    {
        cMesh engMesh;  // The Engine Mesh
        const struct aiMesh* mesh = NULL; // Assim mesh

        engMesh.name = meshName;
        mesh = scene->mMeshes[nd->mMeshes[meshIndex]];
        
        engMesh.numberOfVertices = mesh->mNumVertices;
        engMesh.numberOfTriangles = mesh->mNumFaces;
        engMesh.pVertices = new sVertex[mesh->mNumVertices];
        engMesh.pTriangles = new cTriangle[mesh->mNumFaces];

        // Load the vertices into Engine Mesh
        for (vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
        {
            engMesh.pVertices[vertexIndex].x = mesh->mVertices[vertexIndex].x;
            engMesh.pVertices[vertexIndex].y = mesh->mVertices[vertexIndex].y;
            engMesh.pVertices[vertexIndex].z = mesh->mVertices[vertexIndex].z;

            if (mesh->mColors[0] != NULL)
            {
                engMesh.pVertices[vertexIndex].r = mesh->mColors[0][vertexIndex].r;
                engMesh.pVertices[vertexIndex].g = mesh->mColors[0][vertexIndex].g;
                engMesh.pVertices[vertexIndex].b = mesh->mColors[0][vertexIndex].b;
                engMesh.pVertices[vertexIndex].a = mesh->mColors[0][vertexIndex].a;
            }

            if (mesh->mNormals != NULL)
            {
                engMesh.pVertices[vertexIndex].nx = mesh->mNormals[vertexIndex].x;
                engMesh.pVertices[vertexIndex].ny = mesh->mNormals[vertexIndex].y;
                engMesh.pVertices[vertexIndex].nz = mesh->mNormals[vertexIndex].z;
            }

            if (mesh->HasTextureCoords(0))
            {
                engMesh.pVertices[vertexIndex].u1 = mesh->mTextureCoords[0][vertexIndex].x;
                engMesh.pVertices[vertexIndex].v1 = mesh->mTextureCoords[0][vertexIndex].y;
            }
        }

        // Load the indeces into Engine Mesh
        for (faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
        {
            const struct aiFace* face = &mesh->mFaces[faceIndex];

            engMesh.pTriangles[faceIndex].vertex_ID_0 = face->mIndices[0];
            engMesh.pTriangles[faceIndex].vertex_ID_1 = face->mIndices[1];
            engMesh.pTriangles[faceIndex].vertex_ID_2 = face->mIndices[2];
        }

        // Now send the Engine Mesh to the VAO
        if (!pVAO->loadMeshIntoVAO(engMesh, shaderID, isPersistent))
        {
            //this->logInfo("Some went wrong during the \"loadMeshIntoVAO\" of the cVAOMeshManager!\n");
            return false;
        }
    }

    // Put children into the VAO too
    for (int i = 0; i < nd->mNumChildren; ++i)
    {
        if (!this->recursiveVAOMeshLoader(pVAO, shaderID, meshName, sc,
                                          nd->mChildren[i], isPersistent))
        {
            return false;
        }
    }

    // We loaded the meshes into the VAO
    return true;
}

void cAssimpAssetLoader::createAILogger()
{
    // Change this line to normal if not wanting to analyse the import process
    //Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
    Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

    // Create a logger instance for Console Output
    Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);

    // Create a logger instance for File Output (found in project folder or near .exe)
    Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);

    // Now I am ready for logging my stuff
    Assimp::DefaultLogger::get()->info("Starting the Assimp Logger");
}

void cAssimpAssetLoader::sVertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
    for (unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++)
    {
        if (this->weights[Index] == 0.0f)
        {
            this->ids[Index] = (float)BoneID;
            this->weights[Index] = Weight;
            return;
        }
    }
}
