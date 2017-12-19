#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"

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

    if(key == GLFW_KEY_SPACE);

    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
        ::g_pCamera->releaseGameObject();

    if(key == GLFW_KEY_1 && action == GLFW_PRESS)
        ::g_pCamera->controlGameObject(::g_vecGameObjects[1]);

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
            ::g_pLightManager->vecLights[0].attenuation.x += 0.01f;
            break;
        case GLFW_KEY_B:        // Decrease constant light attenuation
            if(::g_pLightManager->vecLights[0].attenuation.x < 0.01f)
                ::g_pLightManager->vecLights[0].attenuation.x = 0.0f;
            else
                ::g_pLightManager->vecLights[0].attenuation.x -= 0.01f;
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
        }// switch ( key ) 
    }
        break;
    
    case FOLLOW_CAMERA:
    {

    }
    break;

    case FLY_CAMERA:
    {

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