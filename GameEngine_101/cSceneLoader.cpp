#include "cSceneLoader.h"
#include "ModelUtilities.h"
#include <rapidjson\document.h>
#include <sstream>
#include "Utilities.h"
#include "cTransparencyManager.h"
#include "cLightManager.h"
#include "cCameraObject.h"
#include "cVAOMeshManager.h"

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pSkyBoxObject;
extern cTransparencyManager* g_pTranspManager;
extern cVAOMeshManager* g_pVAOManager;

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
    if (!Load3DModelsIntoMeshManager(shaderID, VAO, assetLoader, error))
    {
        return false;
    }

    std::string jsonStr;

    if (!loadFileIntoString(jsonStr, "_Scene.json"))
    {
        error = "Didn't load the Json Scene file!";
        return false;
    }

    const char* json = new char[jsonStr.size() + 1];
    json = jsonStr.c_str();

    rapidjson::Document document;

    if (document.Parse(json).HasParseError())
    {
        error = "There was an error parsing the Json Scene file";
        return false;
    }

    if (!(document.IsObject() 
       && document.HasMember("GameObject") 
       && document["GameObject"].IsArray()))
    {
        error =  "The Json Scene file had a wrong format";
        return false;
    }


    const rapidjson::Value& gameObject = document["GameObject"];

    for (rapidjson::SizeType jsIndex = 0; jsIndex < gameObject.Size(); jsIndex++)
    {
        // Test all variables before reading
        if(!(gameObject[jsIndex]["meshName"].IsString()
           && gameObject[jsIndex]["friendlyName"].IsString()
           && gameObject[jsIndex]["typeOfObject"].IsNumber()
           && gameObject[jsIndex]["radius"].IsNumber()
           && gameObject[jsIndex]["position"].IsArray()
           && gameObject[jsIndex]["diffuseColour"].IsArray()
           && gameObject[jsIndex]["hasMaterialColour"].IsBool()
           && gameObject[jsIndex]["specular"].IsArray()
           && gameObject[jsIndex]["scale"].IsNumber()
           && gameObject[jsIndex]["bIsUpdatedInPhysics"].IsBool()
           && gameObject[jsIndex]["bIsWireFrame"].IsBool()
           && gameObject[jsIndex]["isDebugAABBActive"].IsBool()
           && gameObject[jsIndex]["textureBlend[0]"].IsNumber()
           && gameObject[jsIndex]["textureNames[0]"].IsString()
           && gameObject[jsIndex]["textureBlend[1]"].IsNumber()
           && gameObject[jsIndex]["textureNames[1]"].IsString()
           && gameObject[jsIndex]["textureBlend[2]"].IsNumber()
           && gameObject[jsIndex]["textureNames[2]"].IsString()
           && gameObject[jsIndex]["hasAlpha"].IsBool()
           && gameObject[jsIndex]["useDiscardAlpha"].IsBool()
           && gameObject[jsIndex]["cullFace"].IsBool()
           && gameObject[jsIndex]["hasReflection"].IsBool()
           && gameObject[jsIndex]["rotateX"].IsNumber()
           && gameObject[jsIndex]["rotateY"].IsNumber()
           && gameObject[jsIndex]["rotateZ"].IsNumber()
           && gameObject[jsIndex]["hasToBeSorted"].IsBool()
           && gameObject[jsIndex]["hasCollisionMesh"].IsBool()
           && gameObject[jsIndex]["numCollisionMeshes"].IsNumber()
           && gameObject[jsIndex]["collisionMeshesNames"].IsArray()
           ))
        {
            error = "The Json Gameobject number " + std::to_string(jsIndex + 1) + " is not properly formated!";
            return false;
        }

        std::string meshName = gameObject[jsIndex]["meshName"].GetString();
        std::string friendlyName = gameObject[jsIndex]["friendlyName"].GetString();
        unsigned int typeOfObject = gameObject[jsIndex]["typeOfObject"].GetUint();
        float radius = gameObject[jsIndex]["radius"].GetFloat();
        
        if(!(gameObject[jsIndex]["position"][0].IsNumber()
           && gameObject[jsIndex]["position"][1].IsNumber()
           && gameObject[jsIndex]["position"][2].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                " is not properly formated for its \"position\" array!";
            return false;
        }

        glm::vec3 position;
        position.x = gameObject[jsIndex]["position"][0].GetFloat();
        position.y = gameObject[jsIndex]["position"][1].GetFloat();
        position.z = gameObject[jsIndex]["position"][2].GetFloat();
        
        if(!(gameObject[jsIndex]["diffuseColour"][0].IsNumber()
           && gameObject[jsIndex]["diffuseColour"][1].IsNumber()
           && gameObject[jsIndex]["diffuseColour"][2].IsNumber()
           && gameObject[jsIndex]["diffuseColour"][3].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                " is not properly formated for its \"diffuseColour\" array!";
            return false;
        }

        glm::vec4 diffuseColour;
        diffuseColour.x = gameObject[jsIndex]["diffuseColour"][0].GetFloat();
        diffuseColour.y = gameObject[jsIndex]["diffuseColour"][1].GetFloat();
        diffuseColour.z = gameObject[jsIndex]["diffuseColour"][2].GetFloat();
        diffuseColour.w = gameObject[jsIndex]["diffuseColour"][3].GetFloat();

        bool hasMaterialColour = gameObject[jsIndex]["hasMaterialColour"].GetBool();

        if(!(gameObject[jsIndex]["specular"][0].IsNumber()
           && gameObject[jsIndex]["specular"][1].IsNumber()
           && gameObject[jsIndex]["specular"][2].IsNumber()
           && gameObject[jsIndex]["specular"][3].IsNumber()))
        {
            error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                " is not properly formated for its \"specular\" array!";
            return false;
        }

        glm::vec4 specular;
        specular.x = gameObject[jsIndex]["specular"][0].GetFloat();
        specular.y = gameObject[jsIndex]["specular"][1].GetFloat();
        specular.z = gameObject[jsIndex]["specular"][2].GetFloat();
        specular.w = gameObject[jsIndex]["specular"][3].GetFloat();

        float scale = gameObject[jsIndex]["scale"].GetFloat();
        bool bIsUpdatedInPhysics = gameObject[jsIndex]["bIsUpdatedInPhysics"].GetBool();
        bool bIsWireFrame = gameObject[jsIndex]["bIsWireFrame"].GetBool();
        bool isDebugAABBActive = gameObject[jsIndex]["isDebugAABBActive"].GetBool();
        float textureBlend_0 = gameObject[jsIndex]["textureBlend[0]"].GetFloat();
        std::string textureNames_0 = gameObject[jsIndex]["textureNames[0]"].GetString();
        float textureBlend_1 = gameObject[jsIndex]["textureBlend[1]"].GetFloat();
        std::string textureNames_1 = gameObject[jsIndex]["textureNames[1]"].GetString();
        float textureBlend_2 = gameObject[jsIndex]["textureBlend[2]"].GetFloat();
        std::string textureNames_2 = gameObject[jsIndex]["textureNames[2]"].GetString();
        bool hasAlpha = gameObject[jsIndex]["hasAlpha"].GetBool();
        bool useDiscardAlpha = gameObject[jsIndex]["useDiscardAlpha"].GetBool();
        bool cullFace = gameObject[jsIndex]["cullFace"].GetBool();
        bool hasReflection = gameObject[jsIndex]["hasReflection"].GetBool();
        float rotateX = gameObject[jsIndex]["rotateX"].GetFloat();
        float rotateY = gameObject[jsIndex]["rotateY"].GetFloat();
        float rotateZ = gameObject[jsIndex]["rotateZ"].GetFloat();
        bool hasToBeSorted = gameObject[jsIndex]["hasToBeSorted"].GetBool();
        bool hasCollisionMesh = gameObject[jsIndex]["hasCollisionMesh"].GetBool();

        std::vector<std::string> conllisionMeshesNames;

        if (hasCollisionMesh)
        {
            if(!(gameObject[jsIndex]["collisionMeshesNames"].IsArray()))
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"collisionMeshesNames\" array!";
                return false;
            }
            
            unsigned int numCollisionMeshes = gameObject[jsIndex]["numCollisionMeshes"].GetUint();

            for(int i = 0; i < numCollisionMeshes; i++)
            {
                if(!(gameObject[jsIndex]["collisionMeshesNames"][i].IsString()))
                {
                    error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                        " is not properly formated for its \"collisionMeshesNames\" array!";
                    return false;
                }

                conllisionMeshesNames.push_back(gameObject[jsIndex]["collisionMeshesNames"][i].GetString());
            }
        }
        
             

        cGameObject* theGO = new cGameObject();
        theGO->meshName = meshName;
        theGO->friendlyName = friendlyName;
        theGO->typeOfObject = (eTypeOfObject)typeOfObject;
        theGO->radius = radius;
        theGO->position = position;
        theGO->diffuseColour = diffuseColour;
        theGO->hasColour = hasMaterialColour;
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

        if (hasCollisionMesh)
        {
            for (int i = 0; i < conllisionMeshesNames.size(); i++)
            {
                cMesh collMesh;
                g_pVAOManager->lookupMeshFromName(conllisionMeshesNames[i], collMesh);
                theGO->setCollisionGeometry(collMesh);
            }
        }

        if (hasToBeSorted)
        {
            g_pTranspManager->transpObjects.push_back(theGO);
            continue;
        }

        
        if (theGO->typeOfObject == SKYBOX)
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
    const unsigned int MAXNUMLIGHTS = 20;

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
