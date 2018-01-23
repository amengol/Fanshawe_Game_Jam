#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>
#include <vector>
#include "cGameObject.h"
#include "Utilities.h"
#include <rapidjson\document.h>
#include "cAssimpAssetLoader.h"
#include <iostream>

cModelAssetLoader* g_pModelAssetLoader = NULL;

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects)
{
    // Linear search, baby!
    unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (vec_pGameObjects[index]->friendlyName == friendlyName)
        {
            return vec_pGameObjects[index];
        }
    }
    // Didn't find it
    return NULL;
}

cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects)
{
    // Linear search, baby!
    unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (ID = vec_pGameObjects[index]->getUniqueID())
        {
            return vec_pGameObjects[index];
        }
    }
    // Didn't find it
    return NULL;
}

bool Load3DModelsIntoMeshManager(int shaderID,
    cVAOMeshManager* pVAOManager,
    cModelAssetLoader* pModelAssetLoader,
    std::string &error)
{
    std::string filePath = "assets//models//";
    std::stringstream ssError;
    bool bAllGood = true;

    std::string jsonStr;

    if(!loadFileIntoString(jsonStr, filePath + "_models.json"))
    {
        ssError << "Didn't load the Json file!" << std::endl;
        bAllGood = false;
    }

    const char* json = new char[jsonStr.size() + 1];
    json = jsonStr.c_str();

    rapidjson::Document document;

    if(document.Parse(json).HasParseError())
    {
        ssError << "There was an error parsing the Json file" << std::endl;
        bAllGood = false;
    }
    

    if(!(document.IsObject() && document.HasMember("Meshes") && document["Meshes"].IsArray()))
    {
        ssError << "The Json file had a wrong format" << std::endl;
        error = ssError.str();
        return false;        
    }



    const rapidjson::Value& a = document["Meshes"];
    for(rapidjson::SizeType i = 0; i < a.Size(); i++)
    {
        // Test all variables before reading
        if(!(a[i].IsObject()
           && a[i].HasMember("meshName")
           && a[i]["meshName"].IsString()
           && a[i].HasMember("meshFile")
           && a[i]["meshFile"].IsString()
           && a[i].HasMember("type")
           && a[i]["type"].IsNumber()
           && a[i].HasMember("isPersistent")
           && a[i]["isPersistent"].IsBool()))
        {
            ssError << "The Json object number "<< i+1 << " is not properly formated!" << std::endl;
            error = ssError.str();
            return false;
        }

        std::string meshName = a[i]["meshName"].GetString();
        std::string meshFile = a[i]["meshFile"].GetString();
        unsigned int type = a[i]["type"].GetUint();
        bool isPersistent = a[i]["isPersistent"].GetBool();

        //// Assimp
        //cAssimAssetLoader ail;

        //// Load the models
        //if (!ail.Import3DFromFile(filePath + meshFile))
        //{
        //    std::cout << "There was an error importing the assimp model. See \"assimp_log.txt\" for details.\n";
        //    return false;
        //}
        //else
        //{
        //    std::cout << "Assim scene created.\n";
        //}

        //const aiScene* scene = ail.getScene();

        //// For now, we are goingo to just replace the ModelAssetLoader
        //cMesh theMesh;
        //const struct aiMesh* mesh = scene->mMeshes[0];
        //theMesh.numberOfTriangles = mesh->mNumFaces;
        //cTriangle* triangle;

        //for (int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
        //{
        //    const struct aiFace* face = &mesh->mFaces[faceIndex];
        //    theMesh.numberOfVertices = face->mNumIndices;            
        //   
        //    for (int indicesIndex = 0; indicesIndex < face->mNumIndices; indicesIndex++)		// go through all vertices in face
        //    {
        //        int vertexIndex = face->mIndices[indicesIndex];	// get group index for current index
        //        if (mesh->mColors[0] != NULL)
        //        {
        //            Color4f(&mesh->mColors[0][vertexIndex]);
        //            theMesh.
        //        }                    
        //        if (mesh->mNormals != NULL)

        //            if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
        //            {
        //                glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
        //            }

        //        glNormal3fv(&mesh->mNormals[vertexIndex].x);
        //        glVertex3fv(&mesh->mVertices[vertexIndex].x);
        //    }
        //    glEnd();
        //}

        switch(type)
        {
        case 0:
        {
            cMesh mesh;
            mesh.name = meshName;
            if(!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals(meshFile, mesh))
            {
                //std::cout << "Didn't load model" << std::endl;
                ssError << "Didn't load model >" << mesh.name << "<" << std::endl;
                bAllGood = false;
            }

            if(!pVAOManager->loadMeshIntoVAO(mesh, shaderID, isPersistent))
            {
                //std::cout << "Could not load mesh into VAO" << std::endl;
                ssError << "Could not load mesh >" << mesh.name << "< into VAO" << std::endl;
                bAllGood = false;
            }
        }
            break;
        case 1:
        {
            cMesh mesh;
            mesh.name = meshName;
            if(!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals_and_colours(meshFile, mesh))
            {
                //std::cout << "Didn't load model" << std::endl;
                ssError << "Didn't load model >" << mesh.name << "<" << std::endl;
                bAllGood = false;
            }

            if(!pVAOManager->loadMeshIntoVAO(mesh, shaderID, isPersistent))
            {
                //std::cout << "Could not load mesh into VAO" << std::endl;
                ssError << "Could not load mesh >" << mesh.name << "< into VAO" << std::endl;
                bAllGood = false;
            }
        }
        break;
        case 2:
        {
            cMesh mesh;
            mesh.name = meshName;
            if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV(meshFile, mesh))
            {
                //std::cout << "Didn't load model" << std::endl;
                ssError << "Didn't load model >" << mesh.name << "<" << std::endl;
                bAllGood = false;
            }

            if(!pVAOManager->loadMeshIntoVAO(mesh, shaderID, isPersistent))
            {
                //std::cout << "Could not load mesh into VAO" << std::endl;
                ssError << "Could not load mesh >" << mesh.name << "< into VAO" << std::endl;
                bAllGood = false;
            }
        }
        break;
        default:
            break;
        }    

    }

    if (!bAllGood)
    {
        // Copy the error string stream into the error string that
        //	gets "returned" (through pass by reference)
        error = ssError.str();
    }

    return bAllGood;
}