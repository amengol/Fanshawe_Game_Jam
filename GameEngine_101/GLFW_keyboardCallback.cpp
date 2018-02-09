#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"
#include "cTransparencyManager.h"

extern cTransparencyManager* g_pTranspManager;
extern float g_FOV;

#include <iostream>

bool isShiftKeyDown( int mods, bool bByItself = true );
bool isCtrlKeyDown( int mods, bool bByItself = true );
bool isAltKeyDown( int mods, bool bByItself = true );

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

    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (::g_pCamera->getGameObject() != NULL)            
        {
            cGameObject* theGO = ::g_pCamera->getGameObject();
            theGO->vel = glm::vec3(0.0f);
            theGO->rateOfTurnX = 0.0f;
            theGO->rateOfTurnY = 0.0f;
            theGO->rateOfTurnZ = 0.0f;

            if (::g_pCamera->getCameraMode() != CONTROL_CAMERA)
            {
                ::g_pCamera->setCameraMode(CONTROL_CAMERA);
            } else
            {
                ::g_pCamera->setCameraMode(FOLLOW_CAMERA);
            }
        }
    }

    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (::g_pCamera->getCameraMode() != MANUAL)
        {
            ::g_pCamera->resetCamera();
            ::g_pCamera->setCameraMode(MANUAL);
        }
        else
        {
            ::g_pCamera->setCameraMode(STADIUM_CAMERA);
        }
    }

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
        for (int i = 0; i < g_vecGameObjects.size(); i++)
        {
            if (g_vecGameObjects[i]->friendlyName == "Basketball01")
            {
                ::g_pCamera->lockOnGameObject(g_vecGameObjects[i]);
                if (g_vecGameObjects[i]->bIsUpdatedInPhysics)
                {
                    g_vecGameObjects[i]->bIsUpdatedInPhysics = false;
                    g_vecGameObjects[i]->vel = glm::vec3(0.0f, 0.0f, 0.0f);
                    g_vecGameObjects[i]->position = glm::vec3(0.0f, 4.0f, 0.0f);
                }
                else
                {
                    g_vecGameObjects[i]->bIsUpdatedInPhysics = true;
                    g_vecGameObjects[i]->vel = glm::vec3(0.5f, 0.3f, 0.1f);
                }
            }
        }
    }

    if(key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        for(int i = 0; i < g_pTranspManager->transpObjects.size(); i++)
        {
            if(g_pTranspManager->transpObjects[i]->friendlyName == "Helicopter02")
            {
                ::g_pCamera->lockOnGameObject(g_pTranspManager->transpObjects[i]);
            }
        }
    }

    if(key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        for(int i = 0; i < g_vecGameObjects.size(); i++)
        {
            if(g_vecGameObjects[i]->friendlyName == "Delorean")
            {
                ::g_pCamera->lockOnGameObject(g_vecGameObjects[i]);
                g_vecGameObjects[i]->vel.z = 0.0f;
            }
        }
    }

    if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        cGameObject* theGO = NULL;
        theGO = g_pCamera->getGameObject();
        if(theGO != NULL)
        {
            theGO->isDebugAABBActive = !theGO->isDebugAABBActive;
        }
    }

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
            theGO->vel.x -= 0.1f;
            //glm::vec3 newPos = theGO->orientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            //theGO->position += newPos;
        }
            break;
        case GLFW_KEY_S:       // Decrease speed 
        {
            cGameObject* theGO = g_pCamera->getGameObject();
            theGO->vel.x += 0.1f;
            //glm::vec3 newPos = theGO->orientation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
            //theGO->position += newPos;
        }
            break;
        case GLFW_KEY_A:        // Move camera right along local X axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->vel.z += 0.1f;
           // theGO->rateOfTurnZ -= 30.0f;
            //glm::vec3 newPos = theGO->orientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
            //theGO->position += newPos;
        }            
            break;
        case GLFW_KEY_D:        // Move camera right along local x axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->vel.z -= 0.1f;
            //theGO->rateOfTurnZ += 30.0f;
            //glm::vec3 newPos = theGO->orientation * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
            //theGO->position += newPos;
        }
            break;
        case GLFW_KEY_Q:        // Increase high along Y axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->position.y += 1.0f;
        }
            break;
        case GLFW_KEY_E:        // Decrease high along Y axis
        {
            cGameObject * theGO = g_pCamera->getGameObject();
            theGO->position.y -= 1.0f;
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
        case GLFW_KEY_Z:        // rotate around local camera Z axis +
            g_pCamera->setCameraOrientationZ(ROTANGLE);
            break;
        case GLFW_KEY_C:        // rotate around local camera Z axis -
            g_pCamera->setCameraOrientationZ(-ROTANGLE);
            break;
        case GLFW_KEY_Q:        // Increase high along Y axis
            g_pCamera->changeAlongY(CAMSPEED);
            break;
        case GLFW_KEY_E:        // Decrease high along Y axis
            g_pCamera->changeAlongY(-CAMSPEED);
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
        case GLFW_KEY_H:        // Increase constant light attenuation
            ::g_pLightManager->vecLights[0].attenuation.x += 0.1f;
            break;
        case GLFW_KEY_B:        // Decrease constant light attenuation
            if (::g_pLightManager->vecLights[0].attenuation.x < 0.1f)
                ::g_pLightManager->vecLights[0].attenuation.x = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.x -= 0.1f;
            break;
        case GLFW_KEY_J:        // Increase linear light attenuation
            ::g_pLightManager->vecLights[0].attenuation.y += 0.01f;
            break;
        case GLFW_KEY_N:        // Decrease linear light attenuation
            if (::g_pLightManager->vecLights[0].attenuation.y < 0.01f)
                ::g_pLightManager->vecLights[0].attenuation.y = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.y -= 0.01f;
            break;
        case GLFW_KEY_K:        // Increase quadratic light attenuation
            ::g_pLightManager->vecLights[0].attenuation.z += 0.01f;
            break;
        case GLFW_KEY_M:        // Decrease quadratic light attenuation
            if (::g_pLightManager->vecLights[0].attenuation.z < 0.01f)
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