#include "TextureLoader.h"
#include "Texture\cBasicTextureManager.h"
#include <iostream>

extern cBasicTextureManager* g_pTextureManager;

bool loadTextures()
{
    ::g_pTextureManager = new cBasicTextureManager();

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

    ::g_pTextureManager->SetBasePath("assets/textures/skybox");
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

    ::g_pTextureManager->SetBasePath("assets/textures");

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("delorean.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    //::g_pTextureManager->SetBasePath("assets/textures");

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("Marty.bmp", true)) {
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else {
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("delorean_reflection.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    } else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("FacadeSets01.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("RoofsEtc.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("Asphalt_rue.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("trottoir.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("curb.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("herbe1.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}
    //
    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("sol8.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("StreetPart.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("tree15.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("tree15_alpha.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("heli_green.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("heli_green_alpha.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("heli_green_reflection.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("clouds.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if(!::g_pTextureManager->Create2DTextureFromBMPFile("clouds_alpha.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //} else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Wood.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Concrete.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Steel.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Basket_Square.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("fence_alpha.bmp", true))
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