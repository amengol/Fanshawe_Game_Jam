#include "TextureLoader.h"
#include "Texture\cBasicTextureManager.h"
#include <iostream>

extern cBasicTextureManager* g_pTextureManager;

bool loadTextures()
{
    ::g_pTextureManager = new cBasicTextureManager();

    ::g_pTextureManager->SetBasePath("assets/textures/skybox");
    if(!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "space",
        "NightSky_Right.bmp",
        "NightSky_Left.bmp",
        "NightSky_Bottom.bmp",
        "NightSky_Top.bmp",
        "NightSky_Front.bmp",
        "NightSky_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }


    ::g_pTextureManager->SetBasePath("assets/textures");

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("FacadeSets01.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("RoofsEtc.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("Asphalt_rue.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("trottoir.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("curb.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("herbe1.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }
    
    if(!::g_pTextureManager->Create2DTextureFromBMPFile("sol8.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("StreetPart.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("tree15.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("tree15_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("green.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("reflection.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("clouds.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("clouds_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    return true;
}