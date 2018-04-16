#include "cAssimpAssetLoader.h"

#include <fstream>
#include <iostream>
#include "..\cMesh.h"
#include "..\cVAOMeshManager.h"

// assimp include files.
#include "assimp/postprocess.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

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

bool cAssimpAssetLoader::Import3DFromFile(const std::string& pPath, const std::string & pFile)
{
    // Check if file exists
    std::string file = pPath + pFile;
    std::ifstream fin(file.c_str());
    if (!fin.fail())
    {
        fin.close();
    }
    else
    {
        std::cout << "Couldn't open file: " + file;
        //this->logInfo(importer.GetErrorString());
        return false;
    }

    this->scene = importer.ReadFile(pPath + pFile, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if (!this->scene)
    {
        //this->logInfo(importer.GetErrorString());
        return false;
    }

    // Now we can access the file's contents.
    //this->logInfo("Import of scene " + pFile + " succeeded.");

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

const aiScene* cAssimpAssetLoader::GetAiSceneFromFile(const std::string& pPath, const std::string& pFile)
{
    // Check if file exists
    std::string file = pPath + pFile;
    std::ifstream fin(file.c_str());
    if (!fin.fail())
    {
        fin.close();
    }
    else
    {
        std::cout << "Couldn't open file: " + file;
        //this->logInfo(importer.GetErrorString());
        return NULL;
    }

    Assimp::Importer* pImporter = new Assimp::Importer();
    unsigned int Flags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices;
    const aiScene* pScene = pImporter->ReadFile(pPath + pFile, Flags);

    // If the import failed, report it
    if (!pScene)
    {
        //this->logInfo(importer.GetErrorString());
        return NULL;
    }

    // Now we can access the file's contents.
    //this->logInfo("Import of scene " + pFile + " succeeded.");

    // We're done. Everything will be cleaned up by the importer destructor
    return pScene;
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

            if (mesh->mTangents != NULL)
            {
                engMesh.pVertices[vertexIndex].tx = mesh->mTangents[vertexIndex].x;
                engMesh.pVertices[vertexIndex].ty = mesh->mTangents[vertexIndex].y;
                engMesh.pVertices[vertexIndex].tz = mesh->mTangents[vertexIndex].z;
            }

            if (mesh->mBitangents != NULL)
            {
                engMesh.pVertices[vertexIndex].bx = mesh->mBitangents[vertexIndex].x;
                engMesh.pVertices[vertexIndex].by = mesh->mBitangents[vertexIndex].y;
                engMesh.pVertices[vertexIndex].bz = mesh->mBitangents[vertexIndex].z;
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
        if (!pVAO->loadMeshIntoStaticVAO(engMesh, shaderID, isPersistent))
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
