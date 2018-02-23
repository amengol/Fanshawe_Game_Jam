#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"
#include "cTransparencyManager.h"
#include "cPhysics_Switcher.h"
#include "cLocalization.h"

extern cPhysics_Switcher gPhysicsSwitcher;

extern cLocalization g_Lococalization;

//#include "AI\cSimpleAi_Manager.h"

//extern cSimpleAi_Manager g_AiManager;
//bool enableDebugGrid = false;

extern cTransparencyManager* g_pTranspManager;
extern float g_FOV;

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

    if(key == GLFW_KEY_SPACE)
        return;

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

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(ENGLIH);
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(FRENCH);
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(SPANISH);
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(PORTUGUESE);
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(CHINESE);
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        g_Lococalization.setMenu(INSTRUCTIONS);
    }

    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (g_pCamera->getGameObject() != NULL)
    //    {
    //        cGameObject* curGO = g_pCamera->getGameObject();
    //        curGO->textureBlend[0] = 1.0f;
    //        curGO->textureBlend[2] = 0.0f;

    //        itGO++;
    //        if (itGO == g_vecGameObjects.end())
    //            itGO = g_vecGameObjects.begin();

    //        for (; itGO != g_vecGameObjects.end(); itGO++)
    //        {
    //            cGameObject* GO = *itGO;
    //            if (GO->typeOfObject == SPHERE)
    //            {
    //                GO->textureBlend[0] = 0.0f;
    //                GO->textureBlend[2] = 1.0f;
    //                ::g_pCamera->lockOnGameObject(GO);
    //                break;
    //            }
    //        }            
    //    }
    //    else
    //    {
    //        itGO = g_vecGameObjects.begin();
    //        for (; itGO != g_vecGameObjects.end(); itGO++)
    //        {
    //            cGameObject* GO = *itGO;
    //            if (GO->typeOfObject == SPHERE)
    //            {
    //                GO->textureBlend[0] = 0.0f;
    //                GO->textureBlend[2] = 1.0f;
    //                ::g_pCamera->lockOnGameObject(GO);
    //                break;
    //            }
    //        }
    //    }        
    //}

    //if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    //{
    //    if (g_pCamera->getGameObject() != NULL)
    //    {
    //        if (g_pCamera->getCameraMode() != CONTROL_CAMERA_LOCK)
    //        {
    //            g_pCamera->setCameraMode(CONTROL_CAMERA_LOCK);
    //        }
    //        else
    //        {
    //            g_pCamera->setCameraMode(CONTROL_CAMERA);
    //        }
    //    }
    //}

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

    //if(key == GLFW_KEY_L && action == GLFW_PRESS)
    //{
    //    cGameObject* theGO = NULL;
    //    theGO = g_pCamera->getGameObject();
    //    if(theGO != NULL)
    //    {
    //        theGO->isDebugAABBActive = !theGO->isDebugAABBActive;
    //    }
    //}

    if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        for (size_t i = 0; i < g_vecGameObjects.size() ; i++)
        {
            g_vecGameObjects[i]->bIsWireFrame = !g_vecGameObjects[i]->bIsWireFrame;
        }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        switch (gPhysicsSwitcher.gPhysicsEngine)
        {
        case gPhysicsSwitcher.SUPERDUPER:
            gPhysicsSwitcher.SetBullet();
            break;
        case gPhysicsSwitcher.BULLET:
            gPhysicsSwitcher.SetSuperDuper();
            break;
        default:
            break;
        }
    }

    //if(key == GLFW_KEY_L && action == GLFW_PRESS)
    //{
    //    enableDebugGrid = !enableDebugGrid;
    //    g_AiManager.showDebugGrid(enableDebugGrid);
    //}

    switch(::g_pCamera->getCameraMode())
    {
    case MANUAL:
    {                
        // Camera movements
        const float ROTANGLE = 1.0f;
        const float CAMSPEED = 1.0f;
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
        case GLFW_KEY_B:        // Decrease constant light attenuation
            if(::g_pLightManager->vecLights[0].attenuation.x < 0.1f)
                ::g_pLightManager->vecLights[0].attenuation.x = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.x -= 0.1f;
            break;
        case GLFW_KEY_J:        // Increase linear light attenuation
            ::g_pLightManager->vecLights[0].attenuation.y += 0.01f;
            break;
        case GLFW_KEY_N:        // Decrease linear light attenuation
            if(::g_pLightManager->vecLights[0].attenuation.y < 0.01f)
                ::g_pLightManager->vecLights[0].attenuation.y = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.y -= 0.01f;
            break;
        case GLFW_KEY_K:        // Increase quadratic light attenuation
            ::g_pLightManager->vecLights[0].attenuation.z += 0.01f;
            break;
        case GLFW_KEY_M:        // Decrease quadratic light attenuation
            if(::g_pLightManager->vecLights[0].attenuation.z < 0.01f)
                ::g_pLightManager->vecLights[0].attenuation.z = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.z -= 0.01f;
            break;
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
        const float ROTANGLE = 1.0f;
        const float CAMSPEED = 1.0f;
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel -= orthoZ * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }          

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel += orthoZ * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel -= CamX * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel += CamX * 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel.y += 0.5f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel;
                theGO->rigidBody->GetVelocity(vel);
                vel.y -= 0.1f;
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

            if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
            {
                glm::vec3 vel(0.0f);
                theGO->rigidBody->SetVelocity(vel);
            }

            if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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
            const float ROTANGLE = 1.0f;
            const float CAMSPEED = 1.0f;
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel -= orthoZ * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel += orthoZ * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel -= CamX * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel += CamX * 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel.y += 0.5f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel;
                    theGO->rigidBody->GetVelocity(vel);
                    vel.y -= 0.1f;
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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

                if (theGO->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    glm::vec3 vel(0.0f);
                    theGO->rigidBody->SetVelocity(vel);
                }

                if (theGO->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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
        const float ROTANGLE = 1.0f;
        const float CAMSPEED = 1.0f;
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
        const float ROTANGLE = 1.0f;
        const float CAMSPEED = 1.0f;
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
                if (g_vecGameObjects[i]->rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.SUPERDUPER)
                {
                    g_vecGameObjects[i]->rigidBody->SetVelocity(glm::vec3(0.0f));
                }
                if (g_vecGameObjects[i]->bt_rigidBody != NULL && gPhysicsSwitcher.gPhysicsEngine == gPhysicsSwitcher.BULLET)
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