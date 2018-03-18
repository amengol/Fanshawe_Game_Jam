#include "cSceneLoader.h"
#include "ModelUtilities.h"
#include <rapidjson\document.h>
#include <sstream>
#include "Utilities.h"
#include "cTransparencyManager.h"
#include "cLightManager.h"
#include "cCameraObject.h"
#include "cVAOMeshManager.h"
#include <iPhysicsFactory.h>
#include "globalGameStuff.h"
#include "Assimp\cAnimationState.h"

cCharacterManager g_characterManager;

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
        // Mesh Name
        std::string meshName;
        if (gameObject[jsIndex].HasMember("meshName"))
        {
            if (gameObject[jsIndex]["meshName"].IsString())
            {
                meshName = gameObject[jsIndex]["meshName"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"meshName\" member!";
                return false;
            }
        }
        
        // friendlyName
        std::string friendlyName;
        if (gameObject[jsIndex].HasMember("friendlyName"))
        {
            if (gameObject[jsIndex]["friendlyName"].IsString())
            {
                friendlyName = gameObject[jsIndex]["friendlyName"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"friendlyName\" member!";
                return false;;
            }
        }
        
        // typeOfObject
        unsigned int typeOfObject = 99;
        if (gameObject[jsIndex].HasMember("typeOfObject"))
        {
            if (gameObject[jsIndex]["typeOfObject"].IsNumber())
            {
                typeOfObject = gameObject[jsIndex]["typeOfObject"].GetUint();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"typeOfObject\" member!";
                return false;
            }
        }
        
        // radius
        float radius = 0.0f;
        if (gameObject[jsIndex].HasMember("radius"))
        {
            if (gameObject[jsIndex]["radius"].IsNumber())
            {
                radius = gameObject[jsIndex]["radius"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"radius\" member!";
                return false;
            }
        }        
        
        // Position
        glm::vec3 position(0.0f, 0.0f, 0.0f);
        if (gameObject[jsIndex].HasMember("position"))
        {
            if (!(gameObject[jsIndex]["position"][0].IsNumber()
                && gameObject[jsIndex]["position"][1].IsNumber()
                && gameObject[jsIndex]["position"][2].IsNumber()))
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"position\" member!";
                return false;
            }

            position.x = gameObject[jsIndex]["position"][0].GetFloat();
            position.y = gameObject[jsIndex]["position"][1].GetFloat();
            position.z = gameObject[jsIndex]["position"][2].GetFloat();
        }
        
        // Diffuse Colour
        glm::vec4 diffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
        if (gameObject[jsIndex].HasMember("diffuseColour"))
        {
            if (!(gameObject[jsIndex]["diffuseColour"][0].IsNumber()
                && gameObject[jsIndex]["diffuseColour"][1].IsNumber()
                && gameObject[jsIndex]["diffuseColour"][2].IsNumber()
                && gameObject[jsIndex]["diffuseColour"][3].IsNumber()))
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"diffuseColour\" member!";
                return false;
            }

            diffuseColour.x = gameObject[jsIndex]["diffuseColour"][0].GetFloat();
            diffuseColour.y = gameObject[jsIndex]["diffuseColour"][1].GetFloat();
            diffuseColour.z = gameObject[jsIndex]["diffuseColour"][2].GetFloat();
            diffuseColour.w = gameObject[jsIndex]["diffuseColour"][3].GetFloat();
        }

        // Has Material Colour?
        bool hasMaterialColour = false;
        if (gameObject[jsIndex].HasMember("hasMaterialColour"))
        {
            if (gameObject[jsIndex]["hasMaterialColour"].IsBool())
            {
                hasMaterialColour = gameObject[jsIndex]["hasMaterialColour"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"hasMaterialColour\" member!";
                return false;
            }
        }
        
        // Specular
        glm::vec4 specular(0.0f, 0.0f, 0.0f, 1.0f);
        if (gameObject[jsIndex].HasMember("specular"))
        {
            if (!(gameObject[jsIndex]["specular"][0].IsNumber()
                && gameObject[jsIndex]["specular"][1].IsNumber()
                && gameObject[jsIndex]["specular"][2].IsNumber()
                && gameObject[jsIndex]["specular"][3].IsNumber()))
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"specular\" member!";
                return false;
            }

            specular.x = gameObject[jsIndex]["specular"][0].GetFloat();
            specular.y = gameObject[jsIndex]["specular"][1].GetFloat();
            specular.z = gameObject[jsIndex]["specular"][2].GetFloat();
            specular.w = gameObject[jsIndex]["specular"][3].GetFloat();
        }

        // Scale
        float scale = 1.0f;
        if (gameObject[jsIndex].HasMember("scale"))
        {
            if (gameObject[jsIndex]["scale"].IsNumber())
            {
                scale = gameObject[jsIndex]["scale"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"scale\" member!";
                return false;
            }
        }

        // Is Updated In Physics
        bool bIsUpdatedInPhysics = false;
        if (gameObject[jsIndex].HasMember("bIsUpdatedInPhysics"))
        {
            if (gameObject[jsIndex]["bIsUpdatedInPhysics"].IsBool())
            {
                bIsUpdatedInPhysics = gameObject[jsIndex]["bIsUpdatedInPhysics"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"bIsUpdatedInPhysics\" member!";
                return false;
            }
        }

        // Is Wire Frame
        bool bIsWireFrame = false;
        if (gameObject[jsIndex].HasMember("bIsWireFrame"))
        {
            if (gameObject[jsIndex]["bIsWireFrame"].IsBool())
            {
                bIsWireFrame = gameObject[jsIndex]["bIsWireFrame"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"bIsWireFrame\" member!";
                return false;
            }
        }
       
        // Is Debug AABB Active
        bool isDebugAABBActive = false;
        if (gameObject[jsIndex].HasMember("isDebugAABBActive"))
        {
            if (gameObject[jsIndex]["isDebugAABBActive"].IsBool())
            {
                isDebugAABBActive = gameObject[jsIndex]["isDebugAABBActive"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"isDebugAABBActive\" member!";
                return false;
            }
        }
       
        // textureBlend_0
        float textureBlend_0 = 1.0f;
        if (gameObject[jsIndex].HasMember("textureBlend[0]"))
        {
            if (gameObject[jsIndex]["textureBlend[0]"].IsNumber())
            {
                textureBlend_0 = gameObject[jsIndex]["textureBlend[0]"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureBlend[0]\" member!";
                return false;
            }
        }

        // textureNames_0
        std::string textureNames_0;
        if (gameObject[jsIndex].HasMember("textureNames[0]"))
        {
            if (gameObject[jsIndex]["textureNames[0]"].IsString())
            {
                textureNames_0 = gameObject[jsIndex]["textureNames[0]"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureNames[0]\" member!";
                return false;
            }
        }
        
        // textureBlend_1
        float textureBlend_1 = 0.0f;
        if (gameObject[jsIndex].HasMember("textureBlend[1]"))
        {
            if (gameObject[jsIndex]["textureBlend[1]"].IsNumber())
            {
                textureBlend_1 = gameObject[jsIndex]["textureBlend[1]"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureBlend[1]\" member!";
                return false;
            }
        }
        
        // textureNames_1
        std::string textureNames_1;
        if (gameObject[jsIndex].HasMember("textureNames[1]"))
        {
            if (gameObject[jsIndex]["textureNames[1]"].IsString())
            {
                textureNames_1 = gameObject[jsIndex]["textureNames[1]"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureNames[1]\" member!";
                return false;
            }
        }
        
        // textureBlend_2
        float textureBlend_2 = 0.0f;
        if (gameObject[jsIndex].HasMember("textureBlend[2]"))
        {
            if (gameObject[jsIndex]["textureBlend[2]"].IsNumber())
            {
                textureBlend_2 = gameObject[jsIndex]["textureBlend[2]"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureBlend[2]\" member!";
                return false;
            }
        }

        // textureNames_2
        std::string textureNames_2;
        if (gameObject[jsIndex].HasMember("textureNames[2]"))
        {
            if (gameObject[jsIndex]["textureNames[2]"].IsString())
            {
                textureNames_2 = gameObject[jsIndex]["textureNames[2]"].GetString();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"textureNames[2]\" member!";
                return false;
            }
        }

        // Has Alpha?
        bool hasAlpha = false;
        if (gameObject[jsIndex].HasMember("hasAlpha"))
        {
            if (gameObject[jsIndex]["hasAlpha"].IsBool())
            {
                hasAlpha = gameObject[jsIndex]["hasAlpha"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"hasAlpha\" member!";
                return false;
            }
        }

        // Use Discard Alpha?
        bool useDiscardAlpha = false;
        if (gameObject[jsIndex].HasMember("useDiscardAlpha"))
        {
            if (gameObject[jsIndex]["useDiscardAlpha"].IsBool())
            {
                useDiscardAlpha = gameObject[jsIndex]["useDiscardAlpha"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"useDiscardAlpha\" member!";
                return false;
            }
        }

        // CullFace?
        bool cullFace = true;
        if (gameObject[jsIndex].HasMember("cullFace"))
        {
            if (gameObject[jsIndex]["cullFace"].IsBool())
            {
                cullFace = gameObject[jsIndex]["cullFace"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"cullFace\" member!";
                return false;
            }
        }
        
        // Has Reflection?
        bool hasReflection = false;
        if (gameObject[jsIndex].HasMember("hasReflection"))
        {
            if (gameObject[jsIndex]["hasReflection"].IsBool())
            {
                hasReflection = gameObject[jsIndex]["hasReflection"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"hasReflection\" member!";
                return false;
            }
        }
        
        // Rotate X
        float rotateX = 0.0f;
        if (gameObject[jsIndex].HasMember("rotateX"))
        {
            if (gameObject[jsIndex]["rotateX"].IsNumber())
            {
                rotateX = gameObject[jsIndex]["rotateX"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"rotateX\" member!";
                return false;
            }
        }

        // Rotate Y
        float rotateY = 0.0f;
        if (gameObject[jsIndex].HasMember("rotateY"))
        {
            if (gameObject[jsIndex]["rotateY"].IsNumber())
            {
                rotateY = gameObject[jsIndex]["rotateY"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"rotateY\" member!";
                return false;
            }
        }
        
        // Rotate Z
        float rotateZ = 0.0f;
        if (gameObject[jsIndex].HasMember("rotateZ"))
        {
            if (gameObject[jsIndex]["rotateZ"].IsNumber())
            {
                rotateZ = gameObject[jsIndex]["rotateZ"].GetFloat();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"rotateZ\" member!";
                return false;
            }
        }

        // Has to be Sorted?
        bool hasToBeSorted = false;
        if (gameObject[jsIndex].HasMember("hasToBeSorted"))
        {
            if (gameObject[jsIndex]["hasToBeSorted"].IsBool())
            {
                hasToBeSorted = gameObject[jsIndex]["hasToBeSorted"].GetBool();
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"hasToBeSorted\" member!";
                return false;
            }
        }

        // Has Collision Mesh?
        bool hasCollisionMesh = false;
        std::vector<std::string> conllisionMeshesNames;
        unsigned int numCollisionMeshes = 0;
        if (gameObject[jsIndex].HasMember("hasCollisionMesh"))
        {
            if (gameObject[jsIndex]["hasCollisionMesh"].IsBool())
            {
                hasCollisionMesh = gameObject[jsIndex]["hasCollisionMesh"].GetBool();

                if (hasCollisionMesh)
                {
                    if (!(gameObject[jsIndex]["collisionMeshesNames"].IsArray()))
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"collisionMeshesNames\" array!";
                        return false;
                    }

                    numCollisionMeshes = gameObject[jsIndex]["numCollisionMeshes"].GetUint();

                    for (int i = 0; i < numCollisionMeshes; i++)
                    {
                        if (!(gameObject[jsIndex]["collisionMeshesNames"][i].IsString()))
                        {
                            error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                                " is not properly formated for its \"collisionMeshesNames\" array!";
                            return false;
                        }

                        conllisionMeshesNames.push_back(gameObject[jsIndex]["collisionMeshesNames"][i].GetString());
                    }
                }
            }
            else
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"hasCollisionMesh\" member!";
                return false;
            }
        }
            

        cGameObject* theGO = new cGameObject();
        theGO->meshName = meshName;
        theGO->friendlyName = friendlyName;
        theGO->typeOfObject = (eTypeOfObject)typeOfObject;
        //---------------------------------------------------------------------
        // nPhysics
        switch (theGO->typeOfObject)
        {
        case SPHERE:
        {
            nPhysics::iShape* shape = g_pPhysicsFactory->CreateSphere(radius);
            //nPhysics::iShape* bt_shape = gbt_PhysicsFactory->CreateSphere(radius);
            nPhysics::sRigidBodyDesc desc;
            desc.Position = position;
            nPhysics::iRigidBody* rb = g_pPhysicsFactory->CreateRigidBody(desc, shape);
            theGO->rigidBody = rb;
            //nPhysics::iRigidBody* bt_rb = gbt_PhysicsFactory->CreateRigidBody(desc, bt_shape);
            //theGO->bt_rigidBody = bt_rb;
        }            
            break;
        case PLANE:
            break;
        case CAPSULE:
            break;
        case MESH:
            break;
        case TERRAIN:
            break;
        case CONTACT_POINTS:
            break;
        case SKYBOX:
            break;
        case SKINNED_MESH:
        {
            nPhysics::iShape* shape = g_pPhysicsFactory->CreateCube(1.0f);
            nPhysics::sRigidBodyDesc desc;
            desc.Position = position;
            nPhysics::iRigidBody* rb = g_pPhysicsFactory->CreateRigidBody(desc, shape);
            theGO->rigidBody = rb;

            // This assigns the game object to the particular skinned mesh type 
            theGO->pSimpleSkinnedMesh = ::g_animationCollection.getSkinnedMeshes(meshName);

            if (theGO->pSimpleSkinnedMesh == NULL)
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " has an error in its meshName element!";
                return false;
            }

            cMesh* pTheMesh = theGO->pSimpleSkinnedMesh->CreateMeshObjectFromCurrentModel();
            pTheMesh->name = meshName;

            if (pTheMesh)
            {
                if (!g_pVAOManager->loadMeshIntoStaticVAO(*pTheMesh, shaderID, false))
                {
                    error = "Could not load skinned mesh model into new VAO";
                }
            }
            else
            {
                error = "Could not create a cMesh object from skinned mesh file";
            }
            // Delete temporary mesh if still around
            if (pTheMesh)
            {
                delete pTheMesh;
            }

            if (gameObject[jsIndex].HasMember("defaultAnimation"))
            {
                if (gameObject[jsIndex]["defaultAnimation"].IsString())
                {
                    std::string defaultAnimation = gameObject[jsIndex]["defaultAnimation"].GetString();
                    const aiScene* animScene = ::g_animationCollection.getAnimation(defaultAnimation);
                    if (animScene == NULL)
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " has an error in its defaultAnimation element!";
                        return false;
                    }
                    theGO->pSimpleSkinnedMesh->AddAnimationScene(animScene, defaultAnimation);
                    theGO->pAniState = new cAnimationState();
                    theGO->pAniState->activeAnimation.name = defaultAnimation;
                    theGO->pAniState->activeAnimation.totalTime =
                        theGO->pSimpleSkinnedMesh->GetAnimationDuration(defaultAnimation);
                }
                else
                {
                    error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                        " is not properly formated for its \"defaultAnimation\" member!";
                    return false;
                }

            }
            else
            {
                theGO->pAniState = new cAnimationState();
                theGO->pAniState->activeAnimation.totalTime =
                    theGO->pSimpleSkinnedMesh->GetDefaultAnimationDuration();
            }

            if (gameObject[jsIndex].HasMember("animationList"))
            {
                // We want to consider any GameObject with one of these animations a Character
                bool isCharacter = false;

                if ((gameObject[jsIndex]["animationList"].HasMember("idle")))
                {
                    if (((gameObject[jsIndex]["animationList"]["idle"].IsString())))
                    {
                        theGO->animations.idle = gameObject[jsIndex]["animationList"]["idle"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"idle\" member!";
                        return false;
                    }
                }
                else
                {
                    error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                        "! We really need at least an \"idle\" animation in the \"animationList\" member!";
                    return false;
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("walking")))
                {
                    if (((gameObject[jsIndex]["animationList"]["walking"].IsString())))
                    {
                        theGO->animations.walking = gameObject[jsIndex]["animationList"]["walking"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"walking\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("walking_backwards")))
                {
                    if (((gameObject[jsIndex]["animationList"]["walking_backwards"].IsString())))
                    {
                        theGO->animations.walking_backwards =
                            gameObject[jsIndex]["animationList"]["walking_backwards"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"walking_backwards\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("running")))
                {
                    if (((gameObject[jsIndex]["animationList"]["running"].IsString())))
                    {
                        theGO->animations.running = gameObject[jsIndex]["animationList"]["running"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"running\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("jump")))
                {
                    if (((gameObject[jsIndex]["animationList"]["jump"].IsString())))
                    {
                        theGO->animations.jump = gameObject[jsIndex]["animationList"]["jump"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"jump\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("jump_forward_walking")))
                {
                    if (((gameObject[jsIndex]["animationList"]["jump_forward_walking"].IsString())))
                    {
                        theGO->animations.jump_forward_walking =
                            gameObject[jsIndex]["animationList"]["jump_forward_walking"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"jump_forward_walking\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("jump_forward_running")))
                {
                    if (((gameObject[jsIndex]["animationList"]["jump_forward_running"].IsString())))
                    {
                        theGO->animations.jump_forward_running =
                            gameObject[jsIndex]["animationList"]["jump_forward_running"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"jump_forward_running\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("left_strafe_running")))
                {
                    if (((gameObject[jsIndex]["animationList"]["left_strafe_running"].IsString())))
                    {
                        theGO->animations.left_strafe_running =
                            gameObject[jsIndex]["animationList"]["left_strafe_running"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"left_strafe_running\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("left_strafe_walking")))
                {
                    if (((gameObject[jsIndex]["animationList"]["left_strafe_walking"].IsString())))
                    {
                        theGO->animations.left_strafe =
                            gameObject[jsIndex]["animationList"]["left_strafe_walking"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"left_strafe_walking\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("left_turn")))
                {
                    if (((gameObject[jsIndex]["animationList"]["left_turn"].IsString())))
                    {
                        theGO->animations.left_turn =
                            gameObject[jsIndex]["animationList"]["left_turn"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"left_turn\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("left_turn_90")))
                {
                    if (((gameObject[jsIndex]["animationList"]["left_turn_90"].IsString())))
                    {
                        theGO->animations.left_turn_90 =
                            gameObject[jsIndex]["animationList"]["left_turn_90"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"left_turn_90\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_strafe_running")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_strafe_running"].IsString())))
                    {
                        theGO->animations.right_strafe_running =
                            gameObject[jsIndex]["animationList"]["right_strafe_running"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_strafe_running\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_strafe_walking")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_strafe_walking"].IsString())))
                    {
                        theGO->animations.right_strafe =
                            gameObject[jsIndex]["animationList"]["right_strafe_walking"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_strafe_walking\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_turn")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_turn"].IsString())))
                    {
                        theGO->animations.right_turn =
                            gameObject[jsIndex]["animationList"]["right_turn"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_turn\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_turn_90")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_turn_90"].IsString())))
                    {
                        theGO->animations.right_turn_90 =
                            gameObject[jsIndex]["animationList"]["right_turn_90"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_turn_90\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("trick")))
                {
                    if (((gameObject[jsIndex]["animationList"]["trick"].IsString())))
                    {
                        theGO->animations.trick =
                            gameObject[jsIndex]["animationList"]["trick"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"trick\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("violent_trick")))
                {
                    if (((gameObject[jsIndex]["animationList"]["violent_trick"].IsString())))
                    {
                        theGO->animations.violent_trick =
                            gameObject[jsIndex]["animationList"]["violent_trick"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"violent_trick\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("runaway")))
                {
                    if (((gameObject[jsIndex]["animationList"]["runaway"].IsString())))
                    {
                        theGO->animations.runaway =
                            gameObject[jsIndex]["animationList"]["runaway"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"runaway\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_cross_punch")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_cross_punch"].IsString())))
                    {
                        theGO->animations.right_cross_punch =
                            gameObject[jsIndex]["animationList"]["right_cross_punch"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_cross_punch\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("right_kicking")))
                {
                    if (((gameObject[jsIndex]["animationList"]["right_kicking"].IsString())))
                    {
                        theGO->animations.right_kicking =
                            gameObject[jsIndex]["animationList"]["right_kicking"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"right_kicking\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("praying")))
                {
                    if (((gameObject[jsIndex]["animationList"]["praying"].IsString())))
                    {
                        theGO->animations.praying =
                            gameObject[jsIndex]["animationList"]["praying"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"praying\" member!";
                        return false;
                    }
                }

                if ((gameObject[jsIndex]["animationList"].HasMember("dying")))
                {
                    if (((gameObject[jsIndex]["animationList"]["dying"].IsString())))
                    {
                        theGO->animations.dying =
                            gameObject[jsIndex]["animationList"]["dying"].GetString();
                        isCharacter = true;
                    }
                    else
                    {
                        error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                            " is not properly formated for its \"animationList\", \"dying\" member!";
                        return false;
                    }
                }

                // Check for a character
                if (isCharacter)
                {
                    if (!g_characterManager.AddCharacter(theGO, error))
                        return false;
                    if (!theGO->InitCharacterAnimations(error))
                        return false;
                }
            }
        }
        break;
        case CLOTH:
        {
            // Check for cloth parameters consistence
            bool clothIsOk = true;

            if (gameObject[jsIndex].HasMember("softBodyName")
                && gameObject[jsIndex].HasMember("upperLeftCornerPostion")
                && gameObject[jsIndex].HasMember("gravity")
                && gameObject[jsIndex].HasMember("wind")
                && gameObject[jsIndex].HasMember("nodeMass")
                && gameObject[jsIndex].HasMember("damping")
                && gameObject[jsIndex].HasMember("width")
                && gameObject[jsIndex].HasMember("height")
                && gameObject[jsIndex].HasMember("numNodesWidth")
                && gameObject[jsIndex].HasMember("numNodesHeight"))
            {
                if (gameObject[jsIndex]["softBodyName"].IsString()
                    && gameObject[jsIndex]["upperLeftCornerPostion"].IsArray()
                    && gameObject[jsIndex]["gravity"].IsArray()
                    && gameObject[jsIndex]["wind"].IsArray()
                    && gameObject[jsIndex]["nodeMass"].IsNumber()
                    && gameObject[jsIndex]["damping"].IsNumber()
                    && gameObject[jsIndex]["width"].IsNumber()
                    && gameObject[jsIndex]["height"].IsNumber()
                    && gameObject[jsIndex]["numNodesWidth"].IsNumber()
                    && gameObject[jsIndex]["numNodesHeight"].IsNumber())
                {
                    if (gameObject[jsIndex]["upperLeftCornerPostion"][0].IsNumber()
                        && gameObject[jsIndex]["upperLeftCornerPostion"][1].IsNumber()
                        && gameObject[jsIndex]["upperLeftCornerPostion"][2].IsNumber()
                        && gameObject[jsIndex]["gravity"][0].IsNumber()
                        && gameObject[jsIndex]["gravity"][1].IsNumber()
                        && gameObject[jsIndex]["gravity"][2].IsNumber()
                        && gameObject[jsIndex]["wind"][0].IsNumber()
                        && gameObject[jsIndex]["wind"][1].IsNumber()
                        && gameObject[jsIndex]["wind"][2].IsNumber())
                    {
                        theGO->friendlyName = gameObject[jsIndex]["softBodyName"].GetString();
                        if (theGO->friendlyName == "")
                            clothIsOk = false;

                        float xPos = gameObject[jsIndex]["upperLeftCornerPostion"][0].GetFloat();
                        float yPos = gameObject[jsIndex]["upperLeftCornerPostion"][1].GetFloat();
                        float zPos = gameObject[jsIndex]["upperLeftCornerPostion"][2].GetFloat();
                        float gravityX = gameObject[jsIndex]["gravity"][0].GetFloat();
                        float gravityY = gameObject[jsIndex]["gravity"][1].GetFloat();
                        float gravityZ = gameObject[jsIndex]["gravity"][2].GetFloat();
                        float windX = gameObject[jsIndex]["wind"][0].GetFloat();
                        float windY = gameObject[jsIndex]["wind"][1].GetFloat();
                        float windZ = gameObject[jsIndex]["wind"][2].GetFloat();
                        float nodeMass = gameObject[jsIndex]["nodeMass"].GetFloat();
                        float damping = gameObject[jsIndex]["damping"].GetFloat();
                        float width = gameObject[jsIndex]["width"].GetFloat();
                        float height = gameObject[jsIndex]["height"].GetFloat();
                        int numNodesWidth = gameObject[jsIndex]["numNodesWidth"].GetInt();
                        int numNodesHeight = gameObject[jsIndex]["numNodesHeight"].GetInt();

                        glm::vec3 ulcPos(xPos, yPos, zPos);

                        nPhysics::iForm* cloth = g_pPhysicsFactory->CreateCloth(ulcPos,
                                                                              damping,
                                                                              nodeMass, 
                                                                              width, 
                                                                              height, 
                                                                              numNodesWidth, 
                                                                              numNodesHeight);

                        glm::vec3 gravity(gravityX, gravityY, gravityZ);
                        glm::vec3 wind(windX, windY, windZ);

                        cloth->SetGravity(gravity);
                        cloth->SetWind(wind);

                        nPhysics::iSoftBody* sbCloth = g_pPhysicsFactory->CreateSoftBody(cloth);

                        theGO->softBody = sbCloth;
                        g_pPhysicsWorld->AddSoftBody(sbCloth);
                    }
                    else
                        clothIsOk = false;
                }
                else                    
                    clothIsOk = false;
            }
            else
                clothIsOk = false;

            if (!clothIsOk)
            {
                error = "The Json Gameobject number " + std::to_string(jsIndex + 1) +
                    " is not properly formated for its \"Cloth\" members!";
                return false;
            }
            
        }
            break;
        case UNKNOWN:
            break;
        default:
            break;
        }
        //---------------------------------------------------------------------
        theGO->radius = radius;
        theGO->SetPostiion(position);
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

    }// !for (rapidjson::SizeType jsIndex = 0...

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

bool cSceneLoader::loadCameraParams(cCameraObject* camera, std::string& error)
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

//bool cSceneLoader::loadAiGrid(std::string& error)
//{
//    std::string aiConf;
//
//    if (!loadFileIntoString(aiConf, "AI//maze.conf"))
//    {
//        error = "Didn't load the maze Scene file!";
//        return false;
//    }
//
//    
//    // It is a grid, so the num of collums has to be constant
//    std::string::iterator it = aiConf.begin();
//    int numColumns = 0;
//    bool isCountingColumns = false;
//    while (it != aiConf.end())
//    {
//        if (*it == '|')
//        {
//            isCountingColumns = !isCountingColumns;
//            if (isCountingColumns == false)
//                break;  // We already have a number of collumns
//            it++;
//            continue;
//        }
//        if (isCountingColumns)
//            numColumns++;
//        it++;
//    }
//
//    // The wallss
//    std::vector<int> theWalls;
//
//    int numColumnsInThisRow = 0;
//    int numRows = 0;
//    int start = -1;
//    int end = -1;
//    bool isCountingRows = false;
//    int indexID = 0;
//    for (size_t i = 0; i < aiConf.size(); i++)
//    {
//        char c = aiConf[i];
//        if (c == '|')
//        {
//            isCountingRows = !isCountingRows;
//            if (isCountingRows == false)
//            {
//                numColumnsInThisRow = 0;
//            }
//            else
//            {
//                numRows++;
//            }
//            continue;
//        }
//
//        numColumnsInThisRow++;
//
//        // Current ID wiil be
//        int ID = (numColumnsInThisRow - 1) * 100 + numRows -1;
//
//        if (c == '@')
//            theWalls.push_back(ID);
//        else if (c == 's' || c == 'S')
//            start = ID;
//        else if (c == 'e' || c == 'E')
//            end = ID;
//
//        indexID++;
//    }
//
//    g_AiManager.makeGrid(10, numRows, numColumns, glm::vec3(-70.0f, 0.0f, -70.0f));
//    g_AiManager.loadWalls("Bush", theWalls);
//    bool result = g_AiManager.createMainObjects("Bugs", "Carrot", start, end, error);
//    if (!result)
//    {
//        return false;
//    }
//
//    return true;
//}

bool cSceneLoader::loadLimitPlanes(std::string& error)
{
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
        && document.HasMember("Limit_Planes")
        && document["Limit_Planes"].IsArray()))
    {
        error = "The Json Scene file had a wrong format";
        return false;
    }


    const rapidjson::Value& limitPlanes = document["Limit_Planes"];

    for (rapidjson::SizeType i = 0; i < limitPlanes.Size(); i++)
    {
        // Test all variables before reading
        if (!(limitPlanes[i]["normal"].IsArray()))
        {
            error = "The Json limit plane parameters are not properly formated!";
            return false;
        }

        if (!(limitPlanes[i]["normal"][0].IsNumber()
            && limitPlanes[i]["normal"][1].IsNumber()
            && limitPlanes[i]["normal"][2].IsNumber()))
        {
            error = "The Json limit planes position is not properly formated!";
            return false;
        }

        glm::vec3 normal(0.0f);

        normal.x = limitPlanes[i]["normal"][0].GetFloat();
        normal.y = limitPlanes[i]["normal"][1].GetFloat();
        normal.z = limitPlanes[i]["normal"][2].GetFloat();

        if (!(limitPlanes[i]["constant"].IsNumber()))
        {
            error = "The Json limit plane parameters are not properly formated!";
            return false;
        }

        float constant = limitPlanes[i]["constant"].GetFloat();

        nPhysics::iShape* plane = g_pPhysicsFactory->CreatePlane(normal, constant);
        //nPhysics::iShape* bt_plane = gbt_PhysicsFactory->CreatePlane(normal, constant);

        nPhysics::sRigidBodyDesc desc;
        nPhysics::iRigidBody* rb = g_pPhysicsFactory->CreateRigidBody(desc, plane);
        g_pPhysicsWorld->AddRigidBody(rb);
        //nPhysics::iRigidBody* bt_rb = gbt_PhysicsFactory->CreateRigidBody(desc, bt_plane);
        //gbt_PhysicsWorld->AddRigidBody(bt_rb);

    }//for(rapidjson::SizeType...

    return true;
}
