#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"
#include "cTransparencyManager.h"
#include "cPhysics_Switcher.h"
#include "AI\cCharacterControl.h"

extern cPhysics_Switcher g_physicsSwitcher;

//#include "AI\cSimpleAi_Manager.h"

//extern cSimpleAi_Manager g_AiManager;
//bool enableDebugGrid = false;

extern cTransparencyManager* g_pTranspManager;
extern float g_FOV;

const float ROTANGLE = 1.0f;
const float CAMSPEED = 1.0f;
static bool W_Pressed = false;
static bool S_Pressed = false;
static bool LA_Pressed = false;
static bool RA_Pressed = false;
static bool SHIFT_Pressed = false;


#include <iostream>

bool isShiftKeyDown( int mods, bool bByItself = true );
bool isCtrlKeyDown( int mods, bool bByItself = true );
bool isAltKeyDown( int mods, bool bByItself = true );

std::vector<cGameObject*>::iterator itGO;

void key_callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    //if(key == GLFW_KEY_SPACE)
    //    return;

    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
        ::g_pCamera->releaseGameObject();

    //if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    //{
    //    if (::g_pCamera->getGameObject() != NULL)            
    //    {
    //        cGameObject* theGO = ::g_pCamera->getGameObject();
    //        theGO->vel = glm::vec3(0.0f);
    //        theGO->rateOfTurnX = 0.0f;
    //        theGO->rateOfTurnY = 0.0f;
    //        theGO->rateOfTurnZ = 0.0f;

    //        if (::g_pCamera->getCameraMode() != CONTROL_CAMERA)
    //        {
    //            ::g_pCamera->setCameraMode(CONTROL_CAMERA);
    //        } else
    //        {
    //            ::g_pCamera->setCameraMode(FOLLOW_CAMERA);
    //        }
    //    }
    //}

    //if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    //{
    //    if (::g_pCamera->getCameraMode() != MANUAL)
    //    {
    //        ::g_pCamera->resetCamera();
    //        ::g_pCamera->setCameraMode(MANUAL);
    //    }
    //    else
    //    {
    //        ::g_pCamera->setCameraMode(STADIUM_CAMERA);
    //    }
    //}

    //if(key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    for(int i = 0; i < g_pTranspManager->transpObjects.size(); i++)
    //    {
    //        if(g_pTranspManager->transpObjects[i]->friendlyName == "Basketball")
    //        {
    //            ::g_pCamera->lockOnGameObject(g_pTranspManager->transpObjects[i]);
    //            g_pTranspManager->transpObjects[i]->rateOfTurnY = 0.0f;
    //        }
    //    }        
    //}

    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        // Get the character
        cGameObject* pCharacter = NULL;
        pCharacter = g_characterManager.GetActiveCharacter()->GetCharacter();
        if (pCharacter == NULL)
            return;
        g_pCamera->lockOnCharacter(pCharacter, false);     
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        // Get the character
        cGameObject* pCharacter = NULL;
        pCharacter = g_characterManager.GetActiveCharacter()->GetCharacter();
        if (pCharacter == NULL)
            return;
        g_pCamera->lockOnCharacter(pCharacter, true);
    }

    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (g_lococalization.getState() == MAIN)
    //    {
    //        g_lococalization.setMenu(ENGLISH);
    //        return;
    //    }

    //    if (g_lococalization.getState() == LANGUAGE)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }

    //    if (g_lococalization.getState() == INSTRUCTIONS)
    //    {
    //        eSelectedLanguage lang = g_lococalization.getLanguage();
    //        g_lococalization.setMenu(lang);
    //        return;
    //    }

    //    if (g_lococalization.getState() == DISMISS)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }
    //}

    //if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    //{
    //    if (g_lococalization.getState() == MAIN)
    //    {
    //        g_lococalization.setMenu(FRENCH);
    //        return;
    //    }

    //    if (g_lococalization.getState() == LANGUAGE)
    //    {
    //        eSelectedLanguage lang = g_lococalization.getLanguage();
    //        g_lococalization.setMenu(lang, true);
    //        return;
    //    }

    //    if (g_lococalization.getState() == DISMISS)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }
    //}

    //if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    //{
    //    if (g_lococalization.getState() == MAIN)
    //    {
    //        g_lococalization.setMenu(SPANISH);
    //        return;
    //    }

    //    if (g_lococalization.getState() == LANGUAGE)
    //    {
    //        g_lococalization.dismiss();
    //        return;
    //    }

    //    if (g_lococalization.getState() == DISMISS)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }
    //}

    //if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    //{
    //    if (g_lococalization.getState() == MAIN)
    //    {
    //        g_lococalization.setMenu(PORTUGUESE);
    //        return;
    //    }

    //    if (g_lococalization.getState() == DISMISS)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }
    //}

    //if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    //{
    //    if(g_lococalization.getState() == MAIN)
    //    {
    //        g_lococalization.setMenu(CHINESE);
    //        return;
    //    }

    //    if (g_lococalization.getState() == DISMISS)
    //    {
    //        g_lococalization.setMainMenu();
    //        return;
    //    }
    //}

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        if (g_pCamera->getGameObject() != NULL)
        {
            cGameObject* curGO = g_pCamera->getGameObject();
            curGO->textureBlend[0] = 1.0f;
            curGO->textureBlend[2] = 0.0f;

            itGO++;
            if (itGO == g_vecGameObjects.end())
                itGO = g_vecGameObjects.begin();

            for (; itGO != g_vecGameObjects.end(); itGO++)
            {
                cGameObject* GO = *itGO;
                if (GO->typeOfObject == SPHERE)
                {
                    GO->textureBlend[0] = 0.0f;
                    GO->textureBlend[2] = 1.0f;
                    ::g_pCamera->lockOnGameObject(GO);
                    break;
                }
            }            
        }
        else
        {
            itGO = g_vecGameObjects.begin();
            for (; itGO != g_vecGameObjects.end(); itGO++)
            {
                cGameObject* GO = *itGO;
                if (GO->typeOfObject == SPHERE)
                {
                    GO->textureBlend[0] = 0.0f;
                    GO->textureBlend[2] = 1.0f;
                    ::g_pCamera->lockOnGameObject(GO);
                    break;
                }
            }
        }        
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (g_pCamera->getGameObject() != NULL)
        {
            if (g_pCamera->getCameraMode() != CONTROL_CAMERA_LOCK)
            {
                g_pCamera->setCameraMode(CONTROL_CAMERA_LOCK);
            }
            else
            {
                g_pCamera->setCameraMode(CONTROL_CAMERA);
            }
        }
    }

    //if(key == GLFW_KEY_2 && action == GLFW_PRESS)
    //{
    //    for(int i = 0; i < g_pTranspManager->transpObjects.size(); i++)
    //    {
    //        if(g_pTranspManager->transpObjects[i]->friendlyName == "Helicopter02")
    //        {
    //            ::g_pCamera->lockOnGameObject(g_pTranspManager->transpObjects[i]);
    //        }
    //    }
    //}

    //if(key == GLFW_KEY_3 && action == GLFW_PRESS)
    //{
    //    for(int i = 0; i < g_vecGameObjects.size(); i++)
    //    {
    //        if(g_vecGameObjects[i]->friendlyName == "Delorean")
    //        {
    //            ::g_pCamera->lockOnGameObject(g_vecGameObjects[i]);
    //            g_vecGameObjects[i]->vel.z = 0.0f;
    //        }
    //    }
    //}

    //if(key == GLFW_KEY_F && action == GLFW_PRESS)
    //{
    //    cGameObject* theGO = NULL;
    //    theGO = g_pCamera->getGameObject();
    //    if(theGO != NULL)
    //    {
    //        theGO->isDebugAABBActive = !theGO->isDebugAABBActive;
    //    }
    //}

    if(key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        g_clothDebug = !g_clothDebug;
        /*for (size_t i = 0; i < g_vecGameObjects.size() ; i++)
        {
            g_vecGameObjects[i]->bIsWireFrame = !g_vecGameObjects[i]->bIsWireFrame;
        }*/
    }

    //if (key == GLFW_KEY_P && action == GLFW_PRESS)
    //{
    //    switch (g_physicsSwitcher.gPhysicsEngine)
    //    {
    //    case g_physicsSwitcher.SUPERDUPER:
    //    {
    //        //g_physicsSwitcher.SetBullet();
    //        std::vector<std::wstring> ws;
    //        ws.push_back(L"Physics with Bullet");
    //        ws.push_back(L"[P] to change");
    //        g_textManager.setText(ws, glm::vec3(1.0f, 0.0f, 0.0f));
    //    }            
    //        break;
    //    case g_physicsSwitcher.BULLET:
    //    {
    //        //g_physicsSwitcher.SetSuperDuper();
    //        std::vector<std::wstring> ws;
    //        ws.push_back(L"Physics with SuperDuper");
    //        ws.push_back(L"[P] to change");
    //        g_textManager.setText(ws, glm::vec3(0.0f, 0.0f, 1.0f));
    //    }            
    //        break;
    //    default:
    //        break;
    //    }
    //}

    //if(key == GLFW_KEY_F && action == GLFW_PRESS)
    //{
    //    enableDebugGrid = !enableDebugGrid;
    //    g_AiManager.showDebugGrid(enableDebugGrid);
    //}

    switch(::g_pCamera->getCameraMode())
    {
    case MANUAL:
    {                
        // Camera movements
        switch(key)
        {
        case GLFW_KEY_W:       // Move camera forward along local Z axis 
            g_pCamera->moveCameraBackNForth(-CAMSPEED);
            break;
        case GLFW_KEY_S:       // Move camera backward along local Z axis 
            g_pCamera->moveCameraBackNForth(CAMSPEED);
            break;
        case GLFW_KEY_A:        // Move camera right along local X axis
            g_pCamera->moveCameraLeftNRight(-CAMSPEED);
            break;
        case GLFW_KEY_D:        // Move camera right along local x axis
            g_pCamera->moveCameraLeftNRight(CAMSPEED);
            break;
        case GLFW_KEY_Z:        // rotate around local camera Z axis +
            g_pCamera->setCameraOrientationZ(ROTANGLE);
            break;
        case GLFW_KEY_C:        // rotate around local camera Z axis -
            g_pCamera->setCameraOrientationZ(-ROTANGLE);
            break;
        case GLFW_KEY_LEFT:     // rotate around local camera Y axis +
            g_pCamera->setCameraOrientationY(ROTANGLE);
            break;
        case GLFW_KEY_RIGHT:    // rotate around local camera Y axis -
            g_pCamera->setCameraOrientationY(-ROTANGLE);
            break;
        case GLFW_KEY_Q:        // Increase high along Y axis
            g_pCamera->changeAlongY(CAMSPEED);
            break;
        case GLFW_KEY_E:        // Decrease high along Y axis
            g_pCamera->changeAlongY(-CAMSPEED);
            break;
        case GLFW_KEY_UP:       // rotate around local camera X axis +
            g_pCamera->setCameraOrientationX(ROTANGLE);
            break;
        case GLFW_KEY_DOWN:     // rotate around local camera X axis -
            g_pCamera->setCameraOrientationX(-ROTANGLE);
            break;
        case GLFW_KEY_H:        // Increase constant light attenuation
            ::g_pLightManager->vecLights[0].attenuation.x += 0.1f;
            break;
        //case GLFW_KEY_B:        // Decrease constant light attenuation
        //    if(::g_pLightManager->vecLights[0].attenuation.x < 0.1f)
        //        ::g_pLightManager->vecLights[0].attenuation.x = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.x -= 0.1f;
        //    break;
        //case GLFW_KEY_J:        // Increase linear light attenuation
        //    ::g_pLightManager->vecLights[0].attenuation.y += 0.01f;
        //    break;
        //case GLFW_KEY_N:        // Decrease linear light attenuation
        //    if(::g_pLightManager->vecLights[0].attenuation.y < 0.01f)
        //        ::g_pLightManager->vecLights[0].attenuation.y = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.y -= 0.01f;
        //    break;
        //case GLFW_KEY_K:        // Increase quadratic light attenuation
        //    ::g_pLightManager->vecLights[0].attenuation.z += 0.01f;
        //    break;
        //case GLFW_KEY_M:        // Decrease quadratic light attenuation
        //    if(::g_pLightManager->vecLights[0].attenuation.z < 0.01f)
        //        ::g_pLightManager->vecLights[0].attenuation.z = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.z -= 0.01f;
        //    break;
        case GLFW_KEY_COMMA:
            g_FOV -= 0.01;
            break;
        case GLFW_KEY_PERIOD:
            g_FOV += 0.01;
            break;
        }// switch ( key ) 
    }
        break;
    
    case CONTROL_CAMERA:
    {
        // Camera movements
        switch(key)
        {
        case GLFW_KEY_W:       // Increase speed
        {
            cGameObject* theGO = g_pCamera->getGameObject();
            
            glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

            // Camera Z axis
            glm::vec3 CamZ = camOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

            // Ortho axis only
            glm::vec3 orthoZ = glm::normalize(glm::vec3(CamZ.x, 0.0f, CamZ.z));

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel -= orthoZ * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }          

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel -= orthoZ * 0.1f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
            break;
        case GLFW_KEY_S:       // Decrease speed 
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

            // Camera Z axis
            glm::vec3 CamZ = camOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

            // Ortho axis only
            glm::vec3 orthoZ = glm::normalize(glm::vec3(CamZ.x, 0.0f, CamZ.z));

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel += orthoZ * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel += orthoZ * 0.1f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
            break;
        case GLFW_KEY_A:        // Move camera right along local X axis
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

            // Camera X axis
            glm::vec3 CamX = camOrientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

            // Ortho axis only
            // glm::vec3 orthoX = glm::normalize(glm::vec3(CamX.x, 0.0f, CamX.z));

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel -= CamX * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel -= CamX * 0.1f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }            
            break;
        case GLFW_KEY_D:        // Move camera right along local x axis
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

            // Camera X axis
            glm::vec3 CamX = camOrientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

            // Ortho axis only
            // glm::vec3 orthoX = glm::normalize(glm::vec3(CamX.x, 0.0f, CamX.z));

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel += CamX * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel += CamX * 0.1f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
            break;
        case GLFW_KEY_Q:        // Increase high along Y axis
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel.y += 0.5f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel.y += 0.5f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
            break;
        case GLFW_KEY_E:        // Decrease high along Y axis
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel.y -= 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel;
                theGO->bt_rigidBody->GetVelocity(vel);
                vel.y -= 0.1f;
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
            break;
        case GLFW_KEY_R:        // Resets object's velocities
        {
            cGameObject* theGO = g_pCamera->getGameObject();

            if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel(0.0f);
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
            {
                glm::vec3 vel(0.0f);
                theGO->bt_rigidBody->SetVelocity(vel);
            }
        }
        break;
        case GLFW_KEY_Z:        // rotate around local GameObject Z axis +
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rotateZ(-ROTANGLE);
        }
            break;
        case GLFW_KEY_C:        // rotate around local GameObject Z axis -
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rotateZ(+ROTANGLE);
        }
            break;
        case GLFW_KEY_LEFT:     // rotate around local GameObject Y axis +
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            //theGO->rateOfTurnZ -= 30.0f;
            theGO->rotateY(ROTANGLE);
        }
            break;
        case GLFW_KEY_RIGHT:    // rotate around local GameObject Y axis -
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            //theGO->rateOfTurnZ += 30.0f;
            theGO->rotateY(-ROTANGLE);
        }
            break;
        case GLFW_KEY_UP:       // rotate around local GameObject X axis +
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rotateX(ROTANGLE);
            //theGO->rateOfTurnX += 30.0f;
        }
            break;
        case GLFW_KEY_DOWN:     // rotate around local GameObject X axis -
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rotateX(-ROTANGLE);
            //theGO->rateOfTurnX -= 30.0f;
        }
            break;
        case GLFW_KEY_MINUS:
            if (g_FOV <= 2.0f)
                g_FOV += 0.01;
            break;
        case GLFW_KEY_EQUAL:
            if (g_FOV >= 0.1f)
                g_FOV -= 0.01;
            break;
        }// switch ( key )
    }
    break;

    case CONTROL_CAMERA_LOCK:
    {
            // Camera movements
            switch (key)
            {
            case GLFW_KEY_W:       // Increase speed
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

                // Camera Z axis
                glm::vec3 CamZ = camOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

                // Ortho axis only
                glm::vec3 orthoZ = glm::normalize(glm::vec3(CamZ.x, 0.0f, CamZ.z));

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel -= orthoZ * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel -= orthoZ * 0.1f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_S:       // Decrease speed 
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

                // Camera Z axis
                glm::vec3 CamZ = camOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

                // Ortho axis only
                glm::vec3 orthoZ = glm::normalize(glm::vec3(CamZ.x, 0.0f, CamZ.z));

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel += orthoZ * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel += orthoZ * 0.1f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_A:        // Move camera right along local X axis
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

                // Camera X axis
                glm::vec3 CamX = camOrientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

                // Ortho axis only
                // glm::vec3 orthoX = glm::normalize(glm::vec3(CamX.x, 0.0f, CamX.z));

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel -= CamX * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel -= CamX * 0.1f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_D:        // Move camera right along local x axis
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                glm::mat4x4 camOrientation = g_pCamera->getCameraOrientation();

                // Camera X axis
                glm::vec3 CamX = camOrientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

                // Ortho axis only
                // glm::vec3 orthoX = glm::normalize(glm::vec3(CamX.x, 0.0f, CamX.z));

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel += CamX * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel += CamX * 0.1f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_Q:        // Increase high along Y axis
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel.y += 0.5f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel.y += 0.5f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_E:        // Decrease high along Y axis
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel.y -= 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel;
                    theGO->bt_rigidBody->GetVelocity(vel);
                    vel.y -= 0.1f;
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_R:        // Resets object's velocities
            {
                cGameObject* theGO = g_pCamera->getGameObject();

                if (theGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel(0.0f);
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    glm::vec3 vel(0.0f);
                    theGO->bt_rigidBody->SetVelocity(vel);
                }
            }
            break;
            case GLFW_KEY_Z:        // rotate around local GameObject Z axis +
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                theGO->rotateZ(-ROTANGLE);
            }
            break;
            case GLFW_KEY_C:        // rotate around local GameObject Z axis -
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                theGO->rotateZ(+ROTANGLE);
            }
            break;
            case GLFW_KEY_LEFT:     // rotate around local GameObject Y axis +
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                //theGO->rateOfTurnZ -= 30.0f;
                theGO->rotateY(ROTANGLE);
            }
            break;
            case GLFW_KEY_RIGHT:    // rotate around local GameObject Y axis -
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                //theGO->rateOfTurnZ += 30.0f;
                theGO->rotateY(-ROTANGLE);
            }
            break;
            case GLFW_KEY_UP:       // rotate around local GameObject X axis +
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                theGO->rotateX(ROTANGLE);
                //theGO->rateOfTurnX += 30.0f;
            }
            break;
            case GLFW_KEY_DOWN:     // rotate around local GameObject X axis -
            {
                cGameObject * theGO = g_pCamera->getGameObject();
                theGO->rotateX(-ROTANGLE);
                //theGO->rateOfTurnX -= 30.0f;
            }
            break;
            case GLFW_KEY_MINUS:
                if (g_FOV <= 2.0f)
                    g_FOV += 0.01;
                break;
            case GLFW_KEY_EQUAL:
                if (g_FOV >= 0.1f)
                    g_FOV -= 0.01;
                break;
            }// switch ( key )
        }
    break;

    case FOLLOW_CAMERA:
    {
        // Camera movements
        switch (key)
        {
        case GLFW_KEY_W:       // Increase speed
        {
            cGameObject* theGO = g_pCamera->getGameObject();
            theGO->vel.z += 0.1f;
        }
        break;
        case GLFW_KEY_S:       // Decrease speed 
        {
            cGameObject* theGO = g_pCamera->getGameObject();
            theGO->vel.z -= 0.1f;
        }
        break;
        case GLFW_KEY_A:        // Move camera right along local X axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnY += 30.0f;
        }
        break;
        case GLFW_KEY_D:        // Move camera right along local x axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnY -= 30.0f;
        }
        break;
        case GLFW_KEY_Q:        // Increase high along Y axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->vel.y += 0.1f;
        }
        break;
        case GLFW_KEY_E:        // Decrease high along Y axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->vel.y -= 0.1f;
        }
        break;
        case GLFW_KEY_Z:        // rotate around local GameObject Z axis +
        {
            //cGameObject * theGO = g_pCamera->getGameObject();
            //theGO->rotateZ(-ROTANGLE);
        }
        break;
        case GLFW_KEY_C:        // rotate around local GameObject Z axis -
        {
            //cGameObject * theGO = g_pCamera->getGameObject();
            //theGO->rotateZ(+ROTANGLE);
        }
        break;
        case GLFW_KEY_LEFT:     // rotate around local GameObject Y axis +
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnZ -= 30.0f;
        }
        break;
        case GLFW_KEY_RIGHT:    // rotate around local GameObject Y axis -
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnZ += 30.0f;
        }
        break;
        case GLFW_KEY_UP:       // rotate around local GameObject X axis +
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnX += 30.0f;
        }
        break;
        case GLFW_KEY_DOWN:     // rotate around local GameObject X axis -
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->rateOfTurnX -= 30.0f;
        }
        break;
        }// switch ( key )
    }
    break;

    case FLY_CAMERA:
    {

    }
    break;

    case STADIUM_CAMERA:
    {
        // Camera movements        
        switch (key)
        {
        case GLFW_KEY_W:       // Move camera forward along local Z axis 
            g_pCamera->moveCameraBackNForth_Stadium(-CAMSPEED);
            break;
        case GLFW_KEY_S:       // Move camera backward along local Z axis 
            g_pCamera->moveCameraBackNForth_Stadium(CAMSPEED);
            break;
        case GLFW_KEY_A:        // Move camera right along local X axis
            g_pCamera->moveCameraLeftNRight_Stadium(-CAMSPEED);
            break;
        case GLFW_KEY_D:        // Move camera right along local x axis
            g_pCamera->moveCameraLeftNRight_Stadium(CAMSPEED);
            break;
        //case GLFW_KEY_Z:        // rotate around local camera Z axis +
        //    g_pCamera->setCameraOrientationZ(ROTANGLE);
        //    break;
        //case GLFW_KEY_C:        // rotate around local camera Z axis -
        //    g_pCamera->setCameraOrientationZ(-ROTANGLE);
            break;
        case GLFW_KEY_Q:        // Increase high along Y axis
            g_pCamera->changeAlongY(CAMSPEED);
            break;
        case GLFW_KEY_E:        // Decrease high along Y axis
            g_pCamera->changeAlongY(-CAMSPEED);
            break;
        case GLFW_KEY_R:        // Resets object's velocities
        {
            for (size_t i = 0; i < g_vecGameObjects.size(); i++)
            {
                if (g_vecGameObjects[i]->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
                {
                    g_vecGameObjects[i]->rigidBody->SetVelocity(glm::vec3(0.0f));
                }
                if (g_vecGameObjects[i]->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
                {
                    g_vecGameObjects[i]->bt_rigidBody->SetVelocity(glm::vec3(0.0f));
                }
            }
        }
        break;
        case GLFW_KEY_LEFT:     // rotate around local camera Y axis +
            g_pCamera->setCameraOrientationY_Stadium(ROTANGLE);
            break;
        case GLFW_KEY_RIGHT:    // rotate around local camera Y axis -
            g_pCamera->setCameraOrientationY_Stadium(-ROTANGLE);
            break;
        case GLFW_KEY_UP:       // rotate around local camera X axis +
            g_pCamera->setCameraOrientationX_Stadium(ROTANGLE);
            break;
        case GLFW_KEY_DOWN:     // rotate around local camera X axis -
            g_pCamera->setCameraOrientationX_Stadium(-ROTANGLE);
            break;
        //case GLFW_KEY_H:        // Increase constant light attenuation
        //    ::g_pLightManager->vecLights[0].attenuation.x += 0.1f;
        //    break;
        //case GLFW_KEY_B:        // Decrease constant light attenuation
        //    if (::g_pLightManager->vecLights[0].attenuation.x < 0.1f)
        //        ::g_pLightManager->vecLights[0].attenuation.x = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.x -= 0.1f;
        //    break;
        //case GLFW_KEY_J:        // Increase linear light attenuation
        //    ::g_pLightManager->vecLights[0].attenuation.y += 0.01f;
        //    break;
        //case GLFW_KEY_N:        // Decrease linear light attenuation
        //    if (::g_pLightManager->vecLights[0].attenuation.y < 0.01f)
        //        ::g_pLightManager->vecLights[0].attenuation.y = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.y -= 0.01f;
        //    break;
        //case GLFW_KEY_K:        // Increase quadratic light attenuation
        //    ::g_pLightManager->vecLights[0].attenuation.z += 0.01f;
        //    break;
        //case GLFW_KEY_M:        // Decrease quadratic light attenuation
        //    if (::g_pLightManager->vecLights[0].attenuation.z < 0.01f)
        //        ::g_pLightManager->vecLights[0].attenuation.z = 0.0f;
        //    else
        //        ::g_pLightManager->vecLights[0].attenuation.z -= 0.01f;
        //    break;
        case GLFW_KEY_MINUS:
            if (g_FOV <= 2.0f)
                g_FOV += 0.01;
            break;
        case GLFW_KEY_EQUAL:
            if (g_FOV >= 0.1f)
                g_FOV -= 0.01;
            break;
        }// switch ( key ) 
    }
    break;

    case CHARACTER_CAMERA:
    {
        // Get the character
        cCharacterControl* pCharacterControl = NULL;
        pCharacterControl = g_characterManager.GetActiveCharacter();
        if (pCharacterControl == NULL)
            return;

        cGameObject* pCharacter = NULL;
        pCharacter = pCharacterControl->GetCharacter();

        if (pCharacter == NULL)
            return;

        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS)
        {
            SHIFT_Pressed = true;
            if (W_Pressed)
                pCharacterControl->ForwardRun();
            if (LA_Pressed)
                pCharacterControl->LeftStrafeRun();
            if (RA_Pressed)
                pCharacterControl->RightStrafeRun();
        }

        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_W)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                    pCharacterControl->Forward();
                W_Pressed = true;
            }

            if (key == GLFW_KEY_S)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->Backwards();
                S_Pressed = true;
            }

            if (key == GLFW_KEY_LEFT)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->LeftStrafe();
                LA_Pressed = true;
            }

            if (key == GLFW_KEY_RIGHT)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->RightStrafe();
                RA_Pressed = true;
            }
        }

        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
        {
            SHIFT_Pressed = false;
            if (W_Pressed)
                pCharacterControl->Forward();
            if (LA_Pressed)
                pCharacterControl->LeftStrafe();
            if (RA_Pressed)
                pCharacterControl->RightStrafe();
        }

        if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            W_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            LA_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            RA_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            S_Pressed = false;
        }
        
        if (W_Pressed)
        {
            if (key == GLFW_KEY_A && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(ROTANGLE);
                if (SHIFT_Pressed)
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));
                }
                else
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));
                }
            }
            if (key == GLFW_KEY_D && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(-ROTANGLE);
                if (SHIFT_Pressed)
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));
                }
                else
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));
                }
            }
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            {
                if (SHIFT_Pressed)
                {
                    pCharacterControl->ForwardJump();
                }
            }
        }
        else if (S_Pressed)
        {
            if (key == GLFW_KEY_A && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(-ROTANGLE);
                pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
            }
            if (key == GLFW_KEY_D && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(ROTANGLE);
                pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
            }
        }
        else
        {
            if (key == GLFW_KEY_A && action == GLFW_PRESS)
                pCharacterControl->TurnLeft90();
            if (key == GLFW_KEY_D && action == GLFW_PRESS)
                pCharacterControl->TurnRight90();
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
                pCharacterControl->Jump();
            if (key == GLFW_KEY_T && action == GLFW_PRESS)
                pCharacterControl->Trick();
            if (key == GLFW_KEY_V && action == GLFW_PRESS)
                pCharacterControl->ViolentTrick();
            if (key == GLFW_KEY_P && action == GLFW_PRESS)
                pCharacterControl->RightCrossPunch();
            if (key == GLFW_KEY_K && action == GLFW_PRESS)
                pCharacterControl->RightKicking();
            if (key == GLFW_KEY_A && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_D && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
                pCharacterControl->Idle();
        }

        
        
        //if (key == GLFW_KEY_W && action == GLFW_PRESS)
        //{
        //    pCharacterControl->Forward();
        //    
        //    if (key == GLFW_KEY_A)
        //        pCharacter->rotateY(90.0f);
        //}
        //if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        //    pCharacterControl->Idle();
        
        
        switch (key)
        {
        //case GLFW_KEY_W:       
        //{
        //    W_Pressed = true;
        //    if (action == GLFW_PRESS)
        //        pCharacterControl->Forward();
        //    if (action == GLFW_RELEASE)
        //    {
        //        pCharacterControl->Idle();
        //        W_Pressed = false;
        //    }
        //}
        //break;
        case GLFW_KEY_S:
        {        
            if (action == GLFW_PRESS)
                pCharacterControl->Backwards();
            if (action == GLFW_RELEASE)
                pCharacterControl->Idle();
        }
            break;
        //case GLFW_KEY_A:
        //{
        //    if (W_Pressed)
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->ForwardLeft();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Forward();
        //    }
        //    else
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->TurnLeft90();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Idle();
        //    }            
        //}
        //    break;
        //case GLFW_KEY_D:
        //{
        //    if (W_Pressed)
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->ForwardRight();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Forward();
        //    }
        //    else
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->TurnRight90();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Idle();
        //    }
        //}
        //    break;
        case GLFW_KEY_LEFT:
            break;
        case GLFW_KEY_RIGHT:
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_DOWN:
            break;
        case GLFW_KEY_MINUS:
            if (g_FOV <= 2.0f)
                g_FOV += 0.01;
            break;
        case GLFW_KEY_EQUAL:
            if (g_FOV >= 0.1f)
                g_FOV -= 0.01;
            break;
        }// switch ( key ) 
    }
    break;

    case STADIUM_CHARACTER_CONTROL:
    {
        // Get the character
        cCharacterControl* pCharacterControl = NULL;
        pCharacterControl = g_characterManager.GetActiveCharacter();
        if (pCharacterControl == NULL)
            return;

        cGameObject* pCharacter = NULL;
        pCharacter = pCharacterControl->GetCharacter();

        if (pCharacter == NULL)
            return;

        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS)
        {
            SHIFT_Pressed = true;
            if (W_Pressed)
                pCharacterControl->ForwardRun();
            if (LA_Pressed)
                pCharacterControl->LeftStrafeRun();
            if (RA_Pressed)
                pCharacterControl->RightStrafeRun();
        }

        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_W)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->Forward();
                W_Pressed = true;
            }

            if (key == GLFW_KEY_S)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->Backwards();
                S_Pressed = true;
            }

            if (key == GLFW_KEY_LEFT)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->LeftStrafe();
                LA_Pressed = true;
            }

            if (key == GLFW_KEY_RIGHT)
            {
                //if (SHIFT_Pressed)
                //    pCharacterControl->ForwardRun();
                //else
                pCharacterControl->RightStrafe();
                RA_Pressed = true;
            }
        }

        if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
        {
            SHIFT_Pressed = false;
            if (W_Pressed)
                pCharacterControl->Forward();
            if (LA_Pressed)
                pCharacterControl->LeftStrafe();
            if (RA_Pressed)
                pCharacterControl->RightStrafe();
        }

        if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            W_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            LA_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            RA_Pressed = false;
            SHIFT_Pressed = false;
        }

        if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        {
            pCharacterControl->Idle();
            S_Pressed = false;
        }

        if (W_Pressed)
        {
            if (key == GLFW_KEY_A && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(ROTANGLE);
                if (SHIFT_Pressed)
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));
                }
                else
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));
                }
            }
            if (key == GLFW_KEY_D && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(-ROTANGLE);
                if (SHIFT_Pressed)
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));
                }
                else
                {
                    pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));
                }
            }
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            {
                if (SHIFT_Pressed)
                {
                    pCharacterControl->ForwardJump();
                }
            }
        }
        else if (S_Pressed)
        {
            if (key == GLFW_KEY_A && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(-ROTANGLE);
                pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
            }
            if (key == GLFW_KEY_D && action == GLFW_REPEAT)
            {
                pCharacter->rigidBody->rotateY(ROTANGLE);
                pCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
            }
        }
        else
        {
            if (key == GLFW_KEY_A && action == GLFW_PRESS)
                pCharacterControl->TurnLeft90();
            if (key == GLFW_KEY_D && action == GLFW_PRESS)
                pCharacterControl->TurnRight90();
            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
                pCharacterControl->Jump();
            if (key == GLFW_KEY_T && action == GLFW_PRESS)
                pCharacterControl->Trick();
            if (key == GLFW_KEY_V && action == GLFW_PRESS)
                pCharacterControl->ViolentTrick();
            if (key == GLFW_KEY_P && action == GLFW_PRESS)
                pCharacterControl->RightCrossPunch();
            if (key == GLFW_KEY_K && action == GLFW_PRESS)
                pCharacterControl->RightKicking();
            if (key == GLFW_KEY_A && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_D && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
                pCharacterControl->Idle();
            if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
                pCharacterControl->Idle();
        }



        //if (key == GLFW_KEY_W && action == GLFW_PRESS)
        //{
        //    pCharacterControl->Forward();
        //    
        //    if (key == GLFW_KEY_A)
        //        pCharacter->rotateY(90.0f);
        //}
        //if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        //    pCharacterControl->Idle();


        switch (key)
        {
            //case GLFW_KEY_W:       
            //{
            //    W_Pressed = true;
            //    if (action == GLFW_PRESS)
            //        pCharacterControl->Forward();
            //    if (action == GLFW_RELEASE)
            //    {
            //        pCharacterControl->Idle();
            //        W_Pressed = false;
            //    }
            //}
            //break;
        case GLFW_KEY_S:
        {
            if (action == GLFW_PRESS)
                pCharacterControl->Backwards();
            if (action == GLFW_RELEASE)
                pCharacterControl->Idle();
        }
        break;
        //case GLFW_KEY_A:
        //{
        //    if (W_Pressed)
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->ForwardLeft();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Forward();
        //    }
        //    else
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->TurnLeft90();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Idle();
        //    }            
        //}
        //    break;
        //case GLFW_KEY_D:
        //{
        //    if (W_Pressed)
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->ForwardRight();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Forward();
        //    }
        //    else
        //    {
        //        if (action == GLFW_PRESS)
        //            pCharacterControl->TurnRight90();
        //        if (action == GLFW_RELEASE)
        //            pCharacterControl->Idle();
        //    }
        //}
        //    break;
        case GLFW_KEY_LEFT:
            break;
        case GLFW_KEY_RIGHT:
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_DOWN:
            break;
        case GLFW_KEY_MINUS:
            if (g_FOV <= 2.0f)
                g_FOV += 0.01;
            break;
        case GLFW_KEY_EQUAL:
            if (g_FOV >= 0.1f)
                g_FOV -= 0.01;
            break;
        }// switch ( key ) 
    }
    break;

    default:
        break;
    }


    return;
}

// Helper functions
bool isShiftKeyDown( int mods, bool bByItself /*=true*/ )
{
	if ( bByItself )
	{	// shift by itself?
		if ( mods == GLFW_MOD_SHIFT )	{ return true; }
		else							{ return false; }
	}
	else
	{	// shift with anything else, too
		if ( ( mods && GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )	{	return true;	}
		else												{	return false;	}
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}

//bool isCtrlKeyDown( int mods, bool bByItself = true );
//bool isAltKeyDown( int mods, bool bByItself = true );