#include "TextureLoader.h"
#include <iostream>
#include "..\globalGameStuff.h"


bool loadTextures()
{
    ::g_pTextureManager = new CTextureManager();

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

    if (!::g_pTextureManager->Create2DTextureFromBMPFile("Paladin_diffuse.bmp", true))
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