#include "TextureLoader.h"
#include <iostream>
#include "..\globalGameStuff.h"


bool loadTextures()
{
    ::g_pTextureManager->setBasePath("assets/textures/sb_morning");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "morning",
        "SkyMorning_Right.bmp",
        "SkyMorning_Left.bmp",
        "SkyMorning_Bottom.bmp",
        "SkyMorning_Top.bmp",
        "SkyMorning_Front.bmp",
        "SkyMorning_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures/sb_day");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "day",
        "SkyNoon_Right.bmp",
        "SkyNoon_Left.bmp",
        "SkyNoon_Bottom.bmp",
        "SkyNoon_Top.bmp",
        "SkyNoon_Front.bmp",
        "SkyNoon_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures/sb_sunset");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "sunset",
        "SkySunSet_Right.bmp",
        "SkySunSet_Left.bmp",
        "SkySunSet_Bottom.bmp",
        "SkySunSet_Top.bmp",
        "SkySunSet_Front.bmp",
        "SkySunSet_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures/sb_dusk");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "night",
        "SkyEarlyDusk_Right.bmp",
        "SkyEarlyDusk_Left.bmp",
        "SkyEarlyDusk_Bottom.bmp",
        "SkyEarlyDusk_Top.bmp",
        "SkyEarlyDusk_Front.bmp",
        "SkyEarlyDusk_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures/sb_night");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
        "deep_night",
        "SkyMidNight_Right.bmp",
        "SkyMidNight_Left.bmp",
        "SkyMidNight_Bottom.bmp",
        "SkyMidNight_Top.bmp",
        "SkyMidNight_Front.bmp",
        "SkyMidNight_Back.bmp", true, true))
    {
        std::cout << "Didn't load skybox" << std::endl;
    }

    ::g_pTextureManager->setBasePath("assets/textures");

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("bear_diffuse.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("bear_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("bear_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Concrete.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Concrete_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Old_TV_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Disclaimer.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Start_Menu.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Start_Menu_Loading.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ghosts_n_Goblins_01.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ghosts_n_Goblins_02.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ghosts_n_Goblins_03.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ghosts_n_Goblins_04.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ghosts_n_Goblins_05.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Dust01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Dust01_D_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Dummy_Alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Campfire01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Campfire01_D_specular.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Campfire01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Bloom01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Bloom02_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Gravestone01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Gravestone01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Coffin01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Coffin01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Plant01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Plant01_D_alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Plant01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Table01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Table01_D_specular.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Statue01_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Statue01_D_specular.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Statue01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground_Overlay.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ground_Overlay is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("brick_wall.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("brick_wall_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Sun.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Sun_Alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Moon.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Moon_Alpha.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground03_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground03_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground05_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground05_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground07_D.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Ground07_D_normal.bmp", true))
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

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Tomb01_D_normal.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Paladin_diffuse.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Paladin_specular.bmp", true))
    {
        std::cout << "Didn't load the texture. Oh no!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Texture is loaded!" << std::endl;
    }

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Paladin_normal.bmp", true))
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