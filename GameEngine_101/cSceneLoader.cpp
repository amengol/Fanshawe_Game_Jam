#include "cSceneLoader.h"
#include "ModelUtilities.h"
#include <rapidjson\document.h>
#include <sstream>
#include "Utilities.h"
#include "cTransparencyManager.h"
#include "cLightManager.h"
#include "cCameraObject.h"

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pSkyBoxObject;
extern cTransparencyManager* g_pTranspManager;

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
           && gameObject[i]["specular"].IsArray()
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

        if(!(gameObject[i]["specular"][0].IsNumber()
           && gameObject[i]["specular"][1].IsNumber()
           && gameObject[i]["specular"][2].IsNumber()
           && gameObject[i]["specular"][3].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(i + 1) +
                " is not properly formated for its \"specular\" array!";
            return false;
        }

        glm::vec4 specular;
        specular.x = gameObject[i]["specular"][0].GetFloat();
        specular.y = gameObject[i]["specular"][1].GetFloat();
        specular.z = gameObject[i]["specular"][2].GetFloat();
        specular.w = gameObject[i]["specular"][3].GetFloat();

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
        theGO->specular = specular;
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

bool cSceneLoader::loadLightParams(int shaderID, 
                                   cLightManager* lightManager, 
                                   std::string& error)
{
    // This number should reflect int the shader
    const unsigned int MAXNUMLIGHTS = 10;

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
       && document.HasMember("Lights")
       && document["Lights"].IsArray()))
    {
        error = "The Json Scene file had a wrong format";
        return false;
    }


    const rapidjson::Value& lights = document["Lights"];

    unsigned int numberOfLights = lights.Size();
    for(rapidjson::SizeType i = 0; i < numberOfLights; i++)
    {
        // Test all variables before reading
        if(!(lights[i]["position"].IsArray()
           && lights[i]["diffuse"].IsArray()
           && lights[i]["costantAttenuation"].IsNumber()
           && lights[i]["linearAttenuation"].IsNumber()
           && lights[i]["quadraticAttenuation"].IsNumber()
           && lights[i]["attachToGameObject"].IsString()
           && lights[i]["type"].IsNumber()
           && lights[i]["innerAngle"].IsNumber()
           && lights[i]["outerAngle"].IsNumber()
           && lights[i]["offset"].IsArray()
           && lights[i]["focusDirection"].IsArray()))
        {
            error = "The Json light number " + std::to_string(i + 1) 
                + " is not properly formated!";
            return false;
        }        
    }

    if(numberOfLights > MAXNUMLIGHTS)
    {
        lightManager->CreateLights(MAXNUMLIGHTS);
        lightManager->LoadShaderUniformLocations(shaderID);
    } else
    {
        lightManager->CreateLights(numberOfLights);
        lightManager->LoadShaderUniformLocations(shaderID);
    }

    for(rapidjson::SizeType i = 0; i < numberOfLights; i++)
    {
        if(!(lights[i]["position"][0].IsNumber()
           && lights[i]["position"][1].IsNumber()
           && lights[i]["position"][2].IsNumber()))
        {
            error = "The Json light number " + std::to_string(i + 1) +
                " is not properly formated for its \"position\" array!";
            return false;
        }
        lightManager->vecLights[i].position.x = lights[i]["position"][0].GetFloat();
        lightManager->vecLights[i].position.y = lights[i]["position"][1].GetFloat();
        lightManager->vecLights[i].position.z = lights[i]["position"][2].GetFloat();

        if(!(lights[i]["diffuse"][0].IsNumber()
           && lights[i]["diffuse"][1].IsNumber()
           && lights[i]["diffuse"][2].IsNumber()))
        {
            error = "The Json light number " + std::to_string(i + 1) +
                " is not properly formated for its \"diffuse\" array!";
            return false;
        }
        lightManager->vecLights[i].diffuse.x = lights[i]["diffuse"][0].GetFloat();
        lightManager->vecLights[i].diffuse.y = lights[i]["diffuse"][1].GetFloat();
        lightManager->vecLights[i].diffuse.z = lights[i]["diffuse"][2].GetFloat();

        lightManager->vecLights[i].attenuation.x = lights[i]["costantAttenuation"].GetFloat();
        lightManager->vecLights[i].attenuation.y = lights[i]["linearAttenuation"].GetFloat();
        lightManager->vecLights[i].attenuation.z = lights[i]["quadraticAttenuation"].GetFloat();

        std::string gameObject = lights[i]["attachToGameObject"].GetString();

        if(gameObject != "")
        {
            // Look for the GameObject in the g_vecGameObjects
            for(int j = 0; j < g_vecGameObjects.size(); j++)
            {
                if(g_vecGameObjects[j]->friendlyName == gameObject)
                {
                    g_vecGameObjects[j]->hasLights = true;
                    lightInfo GO_Light;
                    GO_Light.index = i;

                    if(!(lights[i]["offset"][0].IsNumber()
                       && lights[i]["offset"][1].IsNumber()
                       && lights[i]["offset"][2].IsNumber()))
                    {
                        error = "The Json light number " + std::to_string(i + 1) +
                            " is not properly formated for its \"offset\" array!";
                        return false;
                    }
                    GO_Light.offset.x = lights[i]["offset"][0].GetFloat();
                    GO_Light.offset.y = lights[i]["offset"][1].GetFloat();
                    GO_Light.offset.z = lights[i]["offset"][2].GetFloat();

                    GO_Light.type = (LightType)lights[i]["type"].GetUint();

                    if(GO_Light.type == SPOT)
                    {
                        if(!(lights[i]["focusDirection"][0].IsNumber()
                           && lights[i]["focusDirection"][1].IsNumber()
                           && lights[i]["focusDirection"][2].IsNumber()))
                        {
                            error = "The Json light number " + std::to_string(i + 1) +
                                " is not properly formated for its \"focusDirection\" array!";
                            return false;
                        }
                        GO_Light.focusDirection.x = lights[i]["focusDirection"][0].GetFloat();
                        GO_Light.focusDirection.y = lights[i]["focusDirection"][1].GetFloat();
                        GO_Light.focusDirection.z = lights[i]["focusDirection"][2].GetFloat();

                        lightManager->vecLights[i].typeParams.x = 2.0f;
                        lightManager->vecLights[i].typeParams.z = glm::radians(lights[i]["innerAngle"].GetFloat());
                        lightManager->vecLights[i].typeParams.w = glm::radians(lights[i]["outerAngle"].GetFloat());
                    }                    

                    g_vecGameObjects[j]->vecLightsInfo.push_back(GO_Light);
                    break;
                }//if(g_vecGameObjects[j]->friendlyName...
            }//for(int j = 0; j < g_vecGameObjects.size()...

             // Look for the GameObject in the g_pTranspManager
            for(int j = 0; j < g_pTranspManager->transpObjects.size(); j++)
            {
                if(g_pTranspManager->transpObjects[j]->friendlyName == gameObject)
                {
                    g_pTranspManager->transpObjects[j]->hasLights = true;
                    lightInfo GO_Light;
                    GO_Light.index = i;

                    if(!(lights[i]["offset"][0].IsNumber()
                       && lights[i]["offset"][1].IsNumber()
                       && lights[i]["offset"][2].IsNumber()))
                    {
                        error = "The Json light number " + std::to_string(i + 1) +
                            " is not properly formated for its \"offset\" array!";
                        return false;
                    }
                    GO_Light.offset.x = lights[i]["offset"][0].GetFloat();
                    GO_Light.offset.y = lights[i]["offset"][1].GetFloat();
                    GO_Light.offset.z = lights[i]["offset"][2].GetFloat();

                    GO_Light.type = (LightType)lights[i]["type"].GetUint();

                    if(GO_Light.type == SPOT)
                    {
                        if(!(lights[i]["focusDirection"][0].IsNumber()
                           && lights[i]["focusDirection"][1].IsNumber()
                           && lights[i]["focusDirection"][2].IsNumber()))
                        {
                            error = "The Json light number " + std::to_string(i + 1) +
                                " is not properly formated for its \"focusDirection\" array!";
                            return false;
                        }
                        GO_Light.focusDirection.x = lights[i]["focusDirection"][0].GetFloat();
                        GO_Light.focusDirection.y = lights[i]["focusDirection"][1].GetFloat();
                        GO_Light.focusDirection.z = lights[i]["focusDirection"][2].GetFloat();

                        lightManager->vecLights[i].typeParams.x = 2.0f;
                        lightManager->vecLights[i].typeParams.z = glm::radians(lights[i]["innerAngle"].GetFloat());
                        lightManager->vecLights[i].typeParams.w = glm::radians(lights[i]["outerAngle"].GetFloat());
                    }

                    g_pTranspManager->transpObjects[j]->vecLightsInfo.push_back(GO_Light);
                    break;
                }//if(g_pTranspManager->transpObjects[j]->friendlyName...
            }//for(int j = 0; j < g_pTranspManager->transpObjects.size()...
        }//if(gameObject != "")..
    }//for(rapidjson::SizeType i = 0...

    return true;
}

bool cSceneLoader::loadCameraParams(cCameraObject* camera, std::string error)
{
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
       && document.HasMember("Camera")
       && document["Camera"].IsArray()))
    {
        error = "The Json Scene file had a wrong format";
        return false;
    }


    const rapidjson::Value& cameraParams = document["Camera"];

    for(rapidjson::SizeType i = 0; i < cameraParams.Size(); i++)
    {
        // Test all variables before reading
        if(!(cameraParams[i]["CameraPosition"].IsArray()
           && cameraParams[i]["CameraOrientationX"].IsNumber()
           && cameraParams[i]["CameraOrientationY"].IsNumber()
           && cameraParams[i]["CameraOrientationZ"].IsNumber()))
        {
            error = "The Json camera parameters are not properly formated!";
            return false;
        }

        if(!(cameraParams[i]["CameraPosition"][0].IsNumber()
           && cameraParams[i]["CameraPosition"][1].IsNumber()
           && cameraParams[i]["CameraPosition"][2].IsNumber()))
        {
            error = "The Json camera position is not properly formated!";
            return false;
        }

        glm::vec3 camPos(0.0f);
        camPos.x = cameraParams[i]["CameraPosition"][0].GetFloat();
        camPos.y = cameraParams[i]["CameraPosition"][1].GetFloat();
        camPos.z = cameraParams[i]["CameraPosition"][2].GetFloat();
        camera->setCameraPosition(camPos);

        camera->setCameraOrientationY(cameraParams[i]["CameraOrientationY"].GetFloat());
        camera->setCameraOrientationX(cameraParams[i]["CameraOrientationX"].GetFloat());        
        camera->setCameraOrientationZ(cameraParams[i]["CameraOrientationZ"].GetFloat());

        break; // only one camera allowed
        
    }//for(rapidjson::SizeType...

    return true;
}
