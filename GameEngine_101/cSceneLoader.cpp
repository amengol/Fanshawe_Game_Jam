#include "cSceneLoader.h"
#include "ModelUtilities.h"
#include <rapidjson\document.h>
#include <sstream>
#include "Utilities.h"
#include "cTransparencyManager.h"

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pSkyBoxObject;
extern cTransparencyManager* g_pTranspManager;

extern std::vector< cGameObject* >  g_vecGameObjects;

cSceneLoader::cSceneLoader()
{
}

cSceneLoader::~cSceneLoader()
{
}

bool cSceneLoader::loadModelsIntoScene(int shaderID, 
                                       cVAOMeshManager* VAO,
                                       cModelAssetLoader* assetLoader, 
                                       std::string& error)
{
    if(!Load3DModelsIntoMeshManager(shaderID, VAO, assetLoader, error))
    {
        return false;
    }

    //std::stringstream ssError;
    //bool bAllGood = true;

    std::string jsonStr;

    if(!loadFileIntoString(jsonStr, "_Scene.json"))
    {
        error = "Didn't load the Json Scene file!";
        return false;
    }

    const char* json = new char[jsonStr.size() + 1];
    json = jsonStr.c_str();

    rapidjson::Document document;

    if(document.Parse(json).HasParseError())
    {
        error = "There was an error parsing the Json Scene file";
        return false;
    }


    if(!(document.IsObject() 
       && document.HasMember("GameObject") 
       && document["GameObject"].IsArray()))
    {
        error =  "The Json Scene file had a wrong format";
        return false;
    }


    const rapidjson::Value& gameObject = document["GameObject"];

    for(rapidjson::SizeType i = 0; i < gameObject.Size(); i++)
    {
        // Test all variables before reading
        if(!(gameObject[i]["meshName"].IsString()
           && gameObject[i]["friendlyName"].IsString()
           && gameObject[i]["typeOfObject"].IsNumber()
           && gameObject[i]["radius"].IsNumber()
           && gameObject[i]["position"].IsArray()
           && gameObject[i]["diffuseColour"].IsArray()
           && gameObject[i]["scale"].IsNumber()
           && gameObject[i]["bIsUpdatedInPhysics"].IsBool()
           && gameObject[i]["bIsWireFrame"].IsBool()
           && gameObject[i]["isDebugAABBActive"].IsBool()
           && gameObject[i]["textureBlend[0]"].IsNumber()
           && gameObject[i]["textureNames[0]"].IsString()
           && gameObject[i]["textureBlend[1]"].IsNumber()
           && gameObject[i]["textureNames[1]"].IsString()
           && gameObject[i]["textureBlend[2]"].IsNumber()
           && gameObject[i]["textureNames[2]"].IsString()
           && gameObject[i]["hasAlpha"].IsBool()
           && gameObject[i]["useDiscardAlpha"].IsBool()
           && gameObject[i]["cullFace"].IsBool()
           && gameObject[i]["hasReflection"].IsBool()
           && gameObject[i]["rotateX"].IsNumber()
           && gameObject[i]["rotateY"].IsNumber()
           && gameObject[i]["rotateZ"].IsNumber()
           && gameObject[i]["hasToBeSorted"].IsBool()))
        {
            error = "The Json Gameobject number " + std::to_string(i + 1) + " is not properly formated!";
            return false;
        }

        std::string meshName = gameObject[i]["meshName"].GetString();
        std::string friendlyName = gameObject[i]["friendlyName"].GetString();
        unsigned int typeOfObject = gameObject[i]["typeOfObject"].GetUint();
        float radius = gameObject[i]["radius"].GetFloat();
        
        if(!(gameObject[i]["position"][0].IsNumber()
           && gameObject[i]["position"][1].IsNumber()
           && gameObject[i]["position"][2].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(i + 1) + 
                " is not properly formated for its \"position\" array!";
            return false;
        }

        glm::vec3 position;
        position.x = gameObject[i]["position"][0].GetFloat();
        position.y = gameObject[i]["position"][1].GetFloat();
        position.z = gameObject[i]["position"][2].GetFloat();
        
        if(!(gameObject[i]["diffuseColour"][0].IsNumber()
           && gameObject[i]["diffuseColour"][1].IsNumber()
           && gameObject[i]["diffuseColour"][2].IsNumber()
           && gameObject[i]["diffuseColour"][3].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(i + 1) +
                " is not properly formated for its \"diffuseColour\" array!";
            return false;
        }

        glm::vec4 diffuseColour;
        diffuseColour.x = gameObject[i]["diffuseColour"][0].GetFloat();
        diffuseColour.y = gameObject[i]["diffuseColour"][1].GetFloat();
        diffuseColour.z = gameObject[i]["diffuseColour"][2].GetFloat();
        diffuseColour.w = gameObject[i]["diffuseColour"][3].GetFloat();

        float scale = gameObject[i]["scale"].GetFloat();
        bool bIsUpdatedInPhysics = gameObject[i]["bIsUpdatedInPhysics"].GetBool();
        bool bIsWireFrame = gameObject[i]["bIsWireFrame"].GetBool();
        bool isDebugAABBActive = gameObject[i]["isDebugAABBActive"].GetBool();
        float textureBlend_0 = gameObject[i]["textureBlend[0]"].GetFloat();
        std::string textureNames_0 = gameObject[i]["textureNames[0]"].GetString();
        float textureBlend_1 = gameObject[i]["textureBlend[1]"].GetFloat();
        std::string textureNames_1 = gameObject[i]["textureNames[1]"].GetString();
        float textureBlend_2 = gameObject[i]["textureBlend[2]"].GetFloat();
        std::string textureNames_2 = gameObject[i]["textureNames[2]"].GetString();
        bool hasAlpha = gameObject[i]["hasAlpha"].GetBool();
        bool useDiscardAlpha = gameObject[i]["useDiscardAlpha"].GetBool();
        bool cullFace = gameObject[i]["cullFace"].GetBool();
        bool hasReflection = gameObject[i]["hasReflection"].GetBool();
        float rotateX = gameObject[i]["rotateX"].GetFloat();
        float rotateY = gameObject[i]["rotateY"].GetFloat();
        float rotateZ = gameObject[i]["rotateZ"].GetFloat();
        bool hasToBeSorted = gameObject[i]["hasToBeSorted"].GetBool();

        cGameObject* theGO = new cGameObject();
        theGO->meshName = meshName;
        theGO->friendlyName = friendlyName;
        theGO->typeOfObject = (eTypeOfObject)typeOfObject;
        theGO->radius = radius;
        theGO->position = position;
        theGO->diffuseColour = diffuseColour;
        theGO->scale = scale;
        theGO->bIsUpdatedInPhysics = bIsUpdatedInPhysics;
        theGO->bIsWireFrame = bIsWireFrame;
        theGO->isDebugAABBActive = isDebugAABBActive;
        theGO->textureBlend[0] = textureBlend_0;
        theGO->textureNames[0] = textureNames_0;
		theGO->textureBlend[1] = textureBlend_1;
        theGO->textureNames[1] = textureNames_1;
		theGO->textureBlend[2] = textureBlend_2;
        theGO->textureNames[2] = textureNames_2;
        theGO->hasAlpha = hasAlpha;
        theGO->useDiscardAlpha = useDiscardAlpha;
        theGO->cullFace = cullFace;
        theGO->hasReflection = hasReflection;
        theGO->rotateX(rotateX);
		theGO->rotateY(rotateY);
		theGO->rotateZ(rotateZ);

        if(hasToBeSorted)
        {
            g_pTranspManager->transpObjects.push_back(theGO);
            continue;
        }

        
        if(theGO->typeOfObject == SKYBOX)
        {
            g_pSkyBoxObject = theGO;
        }

        g_vecGameObjects.push_back(theGO);

    }

    return true;
}
