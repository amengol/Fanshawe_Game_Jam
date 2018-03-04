#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>
#include <vector>
#include "cGameObject.h"
#include "Utilities.h"
#include <rapidjson\document.h>
#include "Assimp\cAssimpAssetLoader.h"
#include <iostream>
#include "globalGameStuff.h"
#include "Assimp\cAnimationCollection.h"

cAnimationCollection gAnimationCollection;

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
        // meshName
        std::string meshName;
        if (a[i].HasMember("meshName"))
        {
            if (a[i]["meshName"].IsString())
            {
                meshName = a[i]["meshName"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(i + 1) +
                    " is not properly formated for its \"meshName\" member!";
                return false;
            }
        }

        // meshFile
        std::string meshFile;
        if (a[i].HasMember("meshFile"))
        {
            if (a[i]["meshFile"].IsString())
            {
                meshFile = a[i]["meshFile"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(i + 1) +
                    " is not properly formated for its \"meshFile\" member!";
                return false;
            }
        }

        // type
        unsigned int type = 0;
        if (a[i].HasMember("type"))
        {
            if (a[i]["type"].IsNumber())
            {
                type = a[i]["type"].GetUint();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(i + 1) +
                    " is not properly formated for its \"type\" member!";
                return false;
            }
        }
       
        switch (type)
        {
        case 0: // REGULAR
            break;
        case 1: // SKINNED
        {
            cSkinnedMesh* skinnedMesh = new cSkinnedMesh();
            if (!skinnedMesh->LoadMeshFromFile(filePath + meshFile))
            {
                delete skinnedMesh;
                break;
            }                
            skinnedMesh->friendlyName = meshName;
            gAnimationCollection.addSkinnedMesh(meshName, skinnedMesh);
            continue;
        }
            break;
        case 2: // ANIMATIONS
        {
            if (a[i]["animName"].IsString() && a[i]["animFile"].IsString())
            {
                std::string animName = a[i]["animName"].GetString();
                if (animName == "")
                {
                    std::cout << "The animName must have a value!\n";
                    return false;
                }

                std::string animFile = a[i]["animFile"].GetString();
                if (animFile == "")
                {
                    std::cout << "The animFile must have a value!\n";
                    return false;
                }

                // Assimp
                cAssimpAssetLoader ail;
                const aiScene* pScene = ail.GetAiSceneFromFile(filePath, animFile);
                // Load the models
                if (pScene == nullptr)
                {
                    std::cout << "There was an error importing the aiScene from file. "
                        << "See \"assimp_log.txt\" for details.\n";
                    return false;
                }
                else
                {
                    std::cout << "Assimp Animation Scene created.\n";
                }

                gAnimationCollection.addAnimationCollection(animName, pScene);

                continue;
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(i + 1) +
                    " is not properly formated for its \"animName\" or \"animFile\" member!";
                return false;
            }
        }
            break;
        default:
            break;
        }

        // isPersistent
        bool isPersistent = false;
        if (a[i].HasMember("isPersistent"))
        {
            if (a[i]["isPersistent"].IsBool())
            {
                isPersistent = a[i]["isPersistent"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(i + 1) +
                    " is not properly formated for its \"isPersistent\" member!";
                return false;
            }
        }

        // Assimp
        cAssimpAssetLoader ail;

        // Load the models
        if (!ail.Import3DFromFile(filePath, meshFile))
        {
            std::cout << "There was an error importing the assimp model. "
                      << "See \"assimp_log.txt\" for details.\n";
            return false;
        }
        else
        {
            std::cout << "Assimp scene created.\n";
        }

        // Load the meshes into the VAO manager
        if (!ail.loadMeshesIntoVAO(pVAOManager, shaderID, meshName, isPersistent))
        {
            std::cout << "There was an error loading the meshes into the VAO manager. "
                      << "See \"assimp_log.txt\" for details.\n";
            return false;
        }
        else
        {
            std::cout << "Meshes loaded into the VAO manager.\n";
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