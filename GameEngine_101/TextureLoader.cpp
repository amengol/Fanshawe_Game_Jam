#include "TextureLoader.h"
#include "Texture\cBasicTextureManager.h"
#include <iostream>

extern cBasicTextureManager* g_pTextureManager;

bool loadTextures()
{
    ::g_pTextureManager = new cBasicTextureManager();
    ::g_pTextureManager->SetBasePath("assets/textures");

    if(!::g_pTextureManager->Create2DTextureFromBMPFile("160_1.bmp", true))
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

    return true;
}