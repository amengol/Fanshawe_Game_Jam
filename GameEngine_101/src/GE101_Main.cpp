/**
   Game Engine 101!

   @author Jorge Amengol
   @date April, 2018
   @version 2.0
*/


#include <iostream>
#include <stdio.h>
#include <string>
#include "cGameObject.h"
#include "globalGameStuff.h"
#include "Texture/TextureLoader.h"
#include "Utilities.h"
#include "cSceneLoader.h"
#include "cSoundManager.h"
#include "DrawCalls.h"
#include "cFBO.h"
#include "AI/cCharacterControl.h"
#include "cCamera.h"
#include "GLFW_callBacks.h"
#include "cEnvironment.h"
#include "cFBO_Shadow.h"
#include <cPhysicsDebugDrawer.h>
#include "cSoundObject.h"

// The FBOs for the scene
cFBO g_fboFullScene;
cFBO g_fboDeferred;
cFBO_Shadow g_fboShadows;

// Shader Passes
const int DEPTH_RENDER_PASS = 0;
const int FULL_SCENE_RENDER_PASS = 1;
const int DEFERRED_RENDER_PASS = 2;
const int FINAL_RENDER_PASS = 99;

// Physics
nPhysics::iPhysicsFactory* g_pPhysicsFactory = nullptr;
nPhysics::iPhysicsWorld* g_pPhysicsWorld = nullptr;
nPhysics::iPhysicsDebugDrawer* g_pPhysicsDebug = nullptr;
bool gDebugEnable = false;

bool InitPhysics()
{
    g_pPhysicsFactory = new nPhysics::cPhysicsFactory();
    g_pPhysicsWorld = g_pPhysicsFactory->CreateWorld();
    g_pPhysicsDebug = new nPhysics::cPhysicsDebugDrawer();
    g_pPhysicsWorld->SetDebugDrawer(g_pPhysicsDebug);
    return true;
}

// Other Global variables
cCamera g_camera;
int g_scrWidth = 1260;
int g_scrHeight = 768;
cSceneManager* g_pSeceneManager = nullptr;
cVAOMeshManager* g_pVaoManager = nullptr;
cGameObject* g_pSkyBoxObject = nullptr;
cShaderManager*	g_pShaderManager = nullptr;
cLightManager*	g_pLightManager = nullptr;
cEnvironment g_environment;
CTextureManager* g_pTextureManager = nullptr;
cTransparencyManager* g_pTranspManager = nullptr;
std::vector<cGameObject*>  g_vecGameObjects;
cUniLocHandler g_uniLocHandler;
long long g_cubeID = -1;
long long g_lineID = -1;
float g_AABBSize = 20.0f;
cSoundManager* g_pSoundManager = nullptr;

int main()
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    std::string title = "From the Graveyard to the Forest - Game Jam!";

    if (!initialConfig("GameConfig.ini", g_scrWidth, g_scrHeight, title))
    {
        std::cout << "Can't find config file" << std::endl;
        std::cout << "Using defaults" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    GLFWwindow* window = glfwCreateWindow(g_scrWidth, g_scrHeight, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSwapInterval(1);

    // Init Physics
    InitPhysics();

    std::cout << glGetString(GL_VENDOR) << " "
              << glGetString(GL_RENDERER) << ", "
              << glGetString(GL_VERSION) << std::endl;

    std::cout << "Shader language version: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    ::g_pShaderManager = new cShaderManager();

    // Main Shader
    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;
    vertShader.fileName = "newVert.glsl";
    fragShader.fileName = "newFrag.glsl";
    ::g_pShaderManager->setBasePath("assets//shaders//");
    if (!::g_pShaderManager->createProgramFromFile("GE101_Shader", vertShader, fragShader))
    {
        std::cout << "We couldn't create the Shader Program!" << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
        return -1;
    }

    // Bullet debug shader
    cShaderManager::cShader bulletVS;
    cShaderManager::cShader bulletFS;
    bulletVS.fileName = "debugVert.glsl";
    bulletFS.fileName = "debugFrag.glsl";
    if (!::g_pShaderManager->createProgramFromFile("Debug_Shader", bulletVS, bulletFS))
    {
        std::cout << "We couldn't create the Shader Program!" << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
        return -1;
    }

    // If we are here, the shaders comipled and linked OK
    std::cout << "The shaders comipled and linked OK" << std::endl;

    // Transparency Manager
    g_pTranspManager = new cTransparencyManager();

    // Load things
    g_pModelAssetLoader = new cModelAssetLoader();
    g_pModelAssetLoader->setBasePath("assets/models/");
    g_pVaoManager = new cVAOMeshManager();
    g_uniLocHandler.currentProgID = g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    g_uniLocHandler.InitShaderUniformLocations("GE101_Shader");
    // General error string, used for a number of items during start up
    std::string error;
    cSceneLoader sceneLoader;
    // Models
    if(!sceneLoader.loadModelsIntoScene(g_uniLocHandler.currentProgID, 
                                        g_pVaoManager, 
                                        g_pModelAssetLoader, 
                                        error))
    {
        std::cout << "Not all models were loaded..." << std::endl;
        std::cout << error << std::endl;
        system("pause");
        exit(-1);
    }
    // Lights
    g_pLightManager = new cLightManager();
    if (!sceneLoader.loadLightParams(g_uniLocHandler.currentProgID,
        g_pLightManager,
        error))
    {
        std::cout << "Not all lights were loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    // Camera
    if (!sceneLoader.loadCameraParams(g_camera, error))
    {
        std::cout << "The camera configuration was not loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    // Limit planes
    if (!sceneLoader.loadLimitPlanes(error))
    {
        std::cout << "The limit planes configuration was not loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    // Texture 
    g_pTextureManager = new CTextureManager();
    if (!loadTextures())

    {
        std::cout << "Something went wrong while loading the textures!\n";
    }

    // Sound things
    g_pSoundManager = new cSoundManager();
    g_pSoundManager->initSoundScene();
    
    // Environment initialization
    g_environment.initLights(g_uniLocHandler.currentProgID, 
                             g_pLightManager, 
                             g_pTranspManager);
    g_environment.setDayDuration(2);
    g_environment.setTimeOfDay(2.0);

    // Sets the default Game Character
    if (!g_characterManager.SetControlledCharacter("Hero01", error))
    {
        std::cout << "The Game Character was not found!\n";
    }

    // Set NPC Manager  
    cNPCManager g_NPCManager;
    g_NPCManager.SetPlayer(g_characterManager.GetActiveCharacter());
    g_NPCManager.SetNPCs(g_characterManager.GetNPCs());

    // Lock on the main character
    g_camera.lockOnGameObject(g_characterManager.GetActiveCharacter()->GetCharacter());

    // Init the FBOs
    g_fboFullScene.init(g_scrWidth, g_scrHeight, error);
    g_fboDeferred.init(g_scrWidth, g_scrHeight, error);
    g_fboShadows.init(8192, 8192, error);

    // We are going to need the "Knight" later
    // This is a workaround the props sync problem
    cGameObject* knight = nullptr;
    for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    {
        if (g_vecGameObjects[i]->friendlyName == "Hero01")
        {
            knight = g_vecGameObjects[i];
        }
    }

    //-------------------------------------------------------------------------
    // ScreenPlay
    g_pSeceneManager = new cSceneManager();
    g_pSeceneManager->addScreen("Disclaimer.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Disclaimer.bmp"),
                                4.0f, false, false);

    cSoundObject* sound = g_pSoundManager->getSoundFromName("Start_Menu");
    g_pSeceneManager->addScreen("Start_Menu.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Start_Menu_Loading.bmp"),
                                5.0f, true, false, sound);

    g_pSeceneManager->addScreen("Start_Menu.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Start_Menu.bmp"),
                                0.0f, true, false, sound, cSceneManager::START);

    g_pSeceneManager->addScreen("Ghosts_n_Goblins_01.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_01.bmp"),
                                3.0f, true, false);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_01");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_01.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_01.bmp"),
                                2.236f, false, false, sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_02");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_02.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_02.bmp"),
                                3.307f, false, false, sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_03");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_03.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_03.bmp"),
                                1.615f, false, false, sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_04");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_04.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_04.bmp"),
                                1.599f, false, false, sound);

    sound = g_pSoundManager->getSoundFromName("Stage_Introduction_Map");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_05.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_05.bmp"),
                                4.153f, false, false, sound);

    sound = g_pSoundManager->getSoundFromName("Ghosts_n_Goblins_Theme_Rock_Metal_BG");
    sound->setLoop();
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_05.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_05.bmp"),
                                1.0f, false, true, sound);

    g_pSeceneManager->addScreen("Main_Game_Screen",
                                g_pTextureManager->getTextureIDFromTextureName("Dummy_Alpha.bmp"),
                                0.0f, false, false, sound, cSceneManager::LAST);
    g_pSeceneManager->init();
    //-------------------------------------------------------------------------

    // Will be used in the physics step
    float lastTimeStep = static_cast<float>(glfwGetTime());

    // Camera initial position
    g_camera.setYaw(180.0f);
    g_camera.setPitch(5.0f);

    glEnable(GL_DEPTH_TEST);

    // Main Game Loop
    while (!glfwWindowShouldClose(window))
    {
        const float curTime = static_cast<float>(glfwGetTime());
        const float deltaTime = curTime - lastTimeStep;

#ifdef PRINT_FPS
        printf("%f frames per second\n", 1.0f / deltaTime);
#endif // PRINT_FPS

        // Physics step
        g_pPhysicsWorld->TimeStep(deltaTime);
        lastTimeStep = curTime;

        // Updates
        g_environment.update(deltaTime);
        g_pSeceneManager->update(deltaTime);
        g_pSoundManager->updateSoundScene(g_camera.m_position);
        g_NPCManager.Evaluate(deltaTime);
        ::processCameraInput(window, deltaTime);
        glfwGetFramebufferSize(window, &g_scrWidth, &g_scrHeight);
        if (g_scrHeight == 0)
        {
            g_scrHeight = 1;
        }

        // This is to make sure the resolution will be the right one
        if (g_scrWidth != g_fboFullScene.width || g_scrHeight != g_fboFullScene.height)
        {
            g_fboFullScene.reset(g_scrWidth, g_scrHeight, error);
            g_fboDeferred.reset(g_scrWidth, g_scrHeight, error);
        }

        g_pShaderManager->useShaderProgram("GE101_Shader");

        // This is a dummy pass with only the knight
        // to workaround props sync problem
        RenderScene(knight, window, g_camera, deltaTime);

        // Depth pass
        glUniform1i(g_uniLocHandler.renderPassNumber, DEPTH_RENDER_PASS);
        glBindFramebuffer(GL_FRAMEBUFFER, g_fboShadows.ID);
        g_fboShadows.clearBuffer();
        glViewport(0, 0, g_fboShadows.width, g_fboShadows.height);
        RenderShadow(g_vecGameObjects, g_uniLocHandler.currentProgID);
        glViewport(0, 0, g_scrWidth, g_scrHeight);

        // Full render pass
        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D, g_fboShadows.depthTexture_ID);
        glUniform1i(g_uniLocHandler.shadowMap, 20);
        glUniform1i(g_uniLocHandler.renderPassNumber, FULL_SCENE_RENDER_PASS);
        glBindFramebuffer(GL_FRAMEBUFFER, g_fboFullScene.ID);
        g_fboFullScene.clearBuffers();
        RenderScene(g_vecGameObjects, window, g_camera, deltaTime);

        // Render the DEBUG Physics
        if (gDebugEnable)
        {
            g_pShaderManager->useShaderProgram("Debug_Shader");
            g_pPhysicsWorld->DebugDrawWorld(g_pShaderManager->getIDFromFriendlyName("Debug_Shader"),
                                            g_camera.getViewMatrix(),
                                            g_camera.m_zoom,
                                            g_scrWidth,
                                            g_scrHeight);
            g_pShaderManager->useShaderProgram("GE101_Shader");
        }

        // Deferred Pass
        glBindFramebuffer(GL_FRAMEBUFFER, g_fboDeferred.ID);
        g_fboDeferred.clearBuffers();
        glUniform1i(g_uniLocHandler.renderPassNumber, DEFERRED_RENDER_PASS);

        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D, g_fboFullScene.colourTexture_0_ID);
        glUniform1i(g_uniLocHandler.texFBOColour2D, 20);

        glActiveTexture(GL_TEXTURE0 + 21);
        glBindTexture(GL_TEXTURE_2D, g_fboFullScene.normalTexture_1_ID);
        glUniform1i(g_uniLocHandler.texFBONormal2D, 21);

        glActiveTexture(GL_TEXTURE0 + 22);
        glBindTexture(GL_TEXTURE_2D, g_fboFullScene.vertexWorldPos_2_ID);
        glUniform1i(g_uniLocHandler.texFBOVertexWorldPos2D, 22);

        glUniform1f(g_uniLocHandler.screenWidth, static_cast<float>(g_scrWidth));
        glUniform1f(g_uniLocHandler.screenHeight, static_cast<float>(g_scrHeight));
        RenderScene(g_pSkyBoxObject, window, g_camera, deltaTime);

        // Final render pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(g_uniLocHandler.renderPassNumber, FINAL_RENDER_PASS);
        glUniform1f(g_uniLocHandler.sysTime, curTime);
        
        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D, g_fboDeferred.colourTexture_0_ID);
        glUniform1i(g_uniLocHandler.fullRenderedImage2D, 20);

        glActiveTexture(GL_TEXTURE0 + 21);
        glBindTexture(GL_TEXTURE_2D, g_pSeceneManager->getActiveScreenID());
        glUniform1i(g_uniLocHandler.fullRenderedImage2D_Overlay, 21);

        // Old TV effect
        if (g_pSeceneManager->inNoiseOn())
        {
            glUniform1f(g_uniLocHandler.noiseEffectOn, 1.0f);

            glActiveTexture(GL_TEXTURE0 + 22);
            glBindTexture(GL_TEXTURE_2D, g_pTextureManager->getTextureIDFromTextureName("Old_TV_alpha.bmp"));
            glUniform1i(g_uniLocHandler.fullRenderedImage2D_Alpha, 22);

            const float noiseFactor = getRandInRange(0.0f, 1.0f);
            glUniform1f(g_uniLocHandler.noise, noiseFactor);
            glUniform1f(g_uniLocHandler.fade, g_pSeceneManager->getFade());
        }
        else
        {
            glUniform1f(g_uniLocHandler.noiseEffectOn, 0.0f);
            glUniform1f(g_uniLocHandler.fade, g_pSeceneManager->getFade());
        }
        RenderScene(g_pSkyBoxObject, window, g_camera, deltaTime);

#ifdef PRINT_CAMERA_INFO

        // Prints camera information to the title
        std::stringstream ssTitle;
        glm::vec3 curCameraPosition = g_camera.m_position;
        glm::vec3 curCameraLookAt = g_camera.m_lookAt;
        ssTitle << title
            << "Camera (xyz): "
            << curCameraPosition.x << ", "
            << curCameraPosition.y << ", "
            << curCameraPosition.z << " | "
            << "Camera LookAt(xyz): "
            << curCameraLookAt.x << ", "
            << curCameraLookAt.y << ", "
            << curCameraLookAt.z;

        glfwSetWindowTitle(window, ssTitle.str().c_str());

#endif // PRINT_CAMERA_INFO

        glfwSwapBuffers(window);
        glfwPollEvents();

    }// Main Game Loop end

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete ::g_pShaderManager;
    delete ::g_pVaoManager;
    delete ::g_pSoundManager;
    delete ::g_pTranspManager;
    delete ::g_pTextureManager;
    delete ::g_pSeceneManager;

    return 0;

}