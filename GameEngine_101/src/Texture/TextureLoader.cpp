#include "TextureLoader.h"
#include <iostream>
#include "..\globalGameStuff.h"


bool loadTextures()
{
    ::g_pTextureManager = new CTextureManager();

    //::g_pTextureManager->SetBasePath("assets/textures/skybox");
    //if(!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
    //    "space",
    //    "NightSky_Right.bmp",
    //    "NightSky_Left.bmp",
    //    "NightSky_Bottom.bmp",
    //    "NightSky_Top.bmp",
    //    "NightSky_Front.bmp",
    //    "NightSky_Back.bmp", true, true))
    //{
    //    std::cout << "Didn't load skybox" << std::endl;
    //}

    ::g_pTextureManager->setBasePath("assets/textures/skybox");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "space",
        "DaySky_Right.bmp",
        "DaySky_Left.bmp",
        "DaySky_Bottom.bmp",
        "DaySky_Top.bmp",
        "DaySky_Front.bmp",
        "DaySky_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures");

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground03_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Tomb01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    return true;
}