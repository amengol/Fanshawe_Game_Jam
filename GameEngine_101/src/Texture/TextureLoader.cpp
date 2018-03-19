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

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("mars_terrain_LLC.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("mars_terrain_LUC.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("mars_terrain_RLC.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("mars_terrain_RUC.bmp", true))
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

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("rubber.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("static.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("survivor.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("prisoner.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Portal.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("grass.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Dark_Blue.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Dark_Green.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Concrete.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("news.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("parasite.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("delorean.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("delorean_reflection.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Fanshawe.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Marshaller_Male_color.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    // if (!::g_pTextureManager->Create2DTextureFromBMPFile("Wood.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Basket_Square.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("fence_alpha.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Basketball.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Beachball.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Sockerball.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Bush.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    //if (!::g_pTextureManager->Create2DTextureFromBMPFile("Bugs.bmp", true))
    //{
    //    std::cout << "Didn't load the texture. Oh no!" << std::endl;
    //    return false;
    //}
    //else
    //{
    //    std::cout << "Texture is loaded!" << std::endl;
    //}

    return true;
}