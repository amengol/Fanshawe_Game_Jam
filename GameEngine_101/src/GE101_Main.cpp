/**
   Game Engine 101

   @author Jorge Amengol
   @date April, 2018
   @version 2.0
*/

#include "globalOpenGL_GLFW.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include "cGameObject.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "Physics.h"
#include "globalGameStuff.h"
#include "cAABBsManager.h"
#include "Texture\TextureLoader.h"
#include "Utilities.h"
#include "cSceneLoader.h"
#include "cSoundManager.h"
#include "DrawCalls.h"
//#include "../Cloth.h"
#include "cFBO.h"
#include "AI\cCharacterControl.h"
#include "cCamera.h"
#include "GLFW_callBacks.h"
#include "cEnvironment.h"
#include "cFBO_Shadow.h"
#include <cPhysicsDebugDrawer.h>

// Here, the scene is rendered in 3 passes:
// 1. Render geometry to G buffer
// 2. Perform deferred pass, rendering to Deferred buffer
// 3. Then post-pass ("2nd pass" to the scree)
//    Copying from the Pass2_Deferred buffer to the final screen
cFBO g_FBO_fullScene;
cFBO g_FBO_deferred;
cFBO g_FBO_alpha_shadow;
cFBO_Shadow g_FBO_shadows;


//
////=============================================================================
//// Cloth thingy for now
//// Just below are three global variables holding the actual animated stuff; Cloth and Ball 
//Cloth cloth1(glm::vec3(-4.0f, 6.0f, 0.0f), 8.0f, 5.0f, 20, 20); // one Cloth object of the Cloth class
//glm::vec3 ball_pos(-2.0f, 4.0f, 0.0f); // the center of our one ball
//float ball_radius = 2.0f; // the radius of our one ball
//
//float ball_time = 0; // counter for used to calculate the z position of the ball below
//
//void ClothDraw(cGameObject*);
////=============================================================================

//#include "AI\cSimpleAi_Manager.h"


//nPhysics::iPhysicsFactory* gbt_PhysicsFactory = NULL;
//nPhysics::iPhysicsWorld* gbt_PhysicsWorld = NULL;
nPhysics::iPhysicsFactory* g_pPhysicsFactory = NULL;
nPhysics::iPhysicsWorld* g_pPhysicsWorld = NULL;
nPhysics::iPhysicsDebugDrawer* g_pPhysicsDebug = NULL;
bool g_debugEnable = false;

bool InitPhysics()
{
    g_pPhysicsFactory = new nPhysics::cPhysicsFactory();
    g_pPhysicsWorld = g_pPhysicsFactory->CreateWorld();
    g_pPhysicsDebug = new nPhysics::cPhysicsDebugDrawer();
    g_pPhysicsWorld->SetDebugDrawer(g_pPhysicsDebug);
    return true;
}


// Function Prototypes
//void DrawObject(cGameObject* pTheGO);
//void DrawAABB(cGameObject* pTheGO, float size);
//void DrawAABBforPoints(std::vector<glm::vec3> vertices, float AABBSize);
//bool compare(cGameObject* i, cGameObject* j);

// Global variables
// ----------------------------------------------------------------------------
cCamera g_camera;
//cCameraManger g_CameraManager;
int g_scrWidth = 1260;
int g_scrHeight = 768;
cSceneManager* g_pSeceneManager = NULL;
cVAOMeshManager* g_pVAOManager = NULL;
cCameraObject* g_pCamera = NULL;
cCameraObject* g_pFixedCamera = NULL;
cGameObject* g_pSkyBoxObject = NULL;
cShaderManager*	g_pShaderManager = NULL;
cLightManager*	g_pLightManager = NULL;
cEnvironment g_environment;
//cDebugRenderer*	g_pDebugRenderer = NULL;
cSimpleDebugRenderer* g_pSimpleDebug = NULL;
cAABBsManager* g_pAABBsManager = NULL;
CTextureManager* g_pTextureManager = NULL;
cTransparencyManager* g_pTranspManager = NULL;
std::vector< cGameObject* >  g_vecGameObjects;
std::map<long long, miniVAOInfo> g_mapAABBIDminiVAO;
cUniLocHandler g_uniLocHandler;
long long g_cubeID = -1;
long long g_lineID = -1;
float g_AABBSize = 20.0f;

//cNPCManager g_NPCManager;
//cSimpleAi_Manager g_AiManager;
//cLocalization g_lococalization;
//cTextManager g_textManager;

// To deal with sounds
cSoundManager* g_pSoundManager = NULL;


float g_ChromaticAberrationOffset = 0.0f;
float g_CR_Max = 0.1f;
double g_CA_CountDown = 0.0f;

const int FULL_SCENE_RENDER_PASS = 0;
const int DEFERRED_RENDER_PASS = 1;
const int SHADOW_ALPHA_PASS = 2;
const int DEPTH_RENDER_PASS = 3;
const int FINAL_RENDER_PASS = 99;

// ----------------------------------------------------------------------------

int main()
{
    

    GLFWwindow* window;
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    std::string title = "Game Engine 101!";

    if (!initialConfig("GameConfig.ini", g_scrWidth, g_scrHeight, title))
    {
        std::cout << "Can't find config file" << std::endl;
        std::cout << "Using defaults" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(g_scrWidth, g_scrHeight, title.c_str(), NULL, NULL);
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

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // Physics
    InitPhysics();
    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //// Localization and Text---------------------------------------------------
    //if (!g_lococalization.init())
    //{
    //    std::cout << "There was an error initiating the localization process!" << std::endl;
    //}
    //g_lococalization.loadLanguageFromXml("assets\\xml\\localization.xml");
    
    //if (!g_textManager.init())
    //{
    //    std::cout << "There was an error initiating the text manager!" << std::endl;
    //}
    //std::vector<std::wstring> ws;
    //ws.push_back(L"[1] Lock on Player");
    //ws.push_back(L"[2] First Person Mode");
    //ws.push_back(L"[0] Free Camera");
    //g_textManager.setText(ws, glm::vec3(0.0f, 0.0f, 1.0f));    
    ////-------------------------------------------------------------------------

    // General error string, used for a number of items during start up
    std::string error;

    std::cout << glGetString(GL_VENDOR) << " "
        << glGetString(GL_RENDERER) << ", "
        << glGetString(GL_VERSION) << std::endl;

    std::cout << "Shader language version: "
        << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    ::g_pShaderManager = new cShaderManager();

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;

    vertShader.fileName = "newVert.glsl";
    fragShader.fileName = "newFrag.glsl";

    ::g_pShaderManager->setBasePath("assets//shaders//");

    if (!::g_pShaderManager->createProgramFromFile("GE101_Shader", 
                                                   vertShader, 
                                                   fragShader))
    {
        std::cout << "We couldn't create the Shader Program!" << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;

        // Let's exit the program then...
        return -1;
    }

    // Bullet debug shader
    cShaderManager::cShader bulletVS;
    cShaderManager::cShader bulletFS;

    bulletVS.fileName = "debugVert.glsl";
    bulletFS.fileName = "debugFrag.glsl";

    ::g_pShaderManager->setBasePath("assets//shaders//");

    if (!::g_pShaderManager->createProgramFromFile("Debug_Shader",
        bulletVS,
        bulletFS))
    {
        std::cout << "We couldn't create the Shader Program!" << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;

        // Let's exit the program then...
        return -1;
    }

    // If we are here, the shaders comipled and linked OK
    std::cout << "The shaders comipled and linked OK" << std::endl;

    // Init transparency Manager
    ::g_pTranspManager = new cTransparencyManager();

    //-------------------------------------------------------------------------
    // Load models

    ::g_pModelAssetLoader = new cModelAssetLoader();
    ::g_pModelAssetLoader->setBasePath("assets/models/");
    ::g_pVAOManager = new cVAOMeshManager();
    GLint ShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    
    //=========================================================================
    // Cloth
    ::g_pVAOManager->genDynamicVAO("Cloth", ShaderID);
    //=========================================================================

    cSceneLoader sceneLoader;
    if(!sceneLoader.loadModelsIntoScene(ShaderID, g_pVAOManager, g_pModelAssetLoader, error))
    {
        std::cout << "Not all models were loaded..." << std::endl;
        std::cout << error << std::endl;
        system("pause");
        exit(-1);
    }

    ////-------------------------------------------------------------------------
    //// Load AI parameters
    //if (!sceneLoader.loadAiGrid(error))
    //{
    //    std::cout << "The AI configuration was not loaded..." << std::endl;
    //    std::cout << error << std::endl;
    //}

    
    //-------------------------------------------------------------------------
    // AABBs
    ::g_pAABBsManager = new cAABBsManager();
    cMesh meshWithAABBs;
    ::g_pVAOManager->lookupMeshFromName("Collision", meshWithAABBs);
    ::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("FacadeSets", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("RoofsEtc", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("Asphalt", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("Concrete", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("Ground1", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("Ground2", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);
    //::g_pVAOManager->lookupMeshFromName("StreetPart", meshWithAABBs);
    //::g_pAABBsManager->genAABBs(&meshWithAABBs, g_AABBSize);

    //-------------------------------------------------------------------------
    // Simple Debug Renderer
    ::g_pSimpleDebug = new cSimpleDebugRenderer();
    //if(!::g_pSimpleDebug->genDebugGeometry(DEBUG_CUBE, g_AABBSize, g_cubeID))
    //{
    //    std::cout << "genDebugGeometry: There was en error generating a geometry!\n";
    //}
    //if(!::g_pSimpleDebug->genDebugGeometry(DEBUG_LINE, 1.0f, g_lineID))
    //{
    //    std::cout << "genDebugGeometry: There was en error generating a geometry!\n";
    //}

    //for(int i = 0; i < g_vecGameObjects.size(); i++)
    //{
    //    if(g_vecGameObjects[i]->friendlyName == "Delorean")
    //        g_vecGameObjects[i]->vel.z = 20.0f;
    //}
    //=========================================================================
    // Sound things
     g_pSoundManager = new cSoundManager();
     g_pSoundManager->initSoundScene();
    //=========================================================================

    // End of loading models
    //-------------------------------------------------------------------------

    GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    g_uniLocHandler.InitShaderUniformLocations("GE101_Shader");

    //-------------------------------------------------------------------------
    // Lights

    ::g_pLightManager = new cLightManager();

    if(!sceneLoader.loadLightParams(currentProgID, g_pLightManager, error))
    {
        std::cout << "Not all lights were loaded..." << std::endl;
        std::cout << error << std::endl;
    }

    // Lights end
    
    // Environment initialization
    g_environment.initLights(currentProgID, g_pLightManager, g_pTranspManager);
    g_environment.setTimeOfDay(0);
    g_environment.setDayDuration(1);
    //g_environment.setMode(cEnvironment::Mode::CONTINUOUS);

    
    //-------------------------------------------------------------------------
    // Texture 
    g_pTextureManager = new CTextureManager();
    if(!loadTextures())
        std::cout << "Something went wrong while loading the textures!\n";
    //-------------------------------------------------------------------------
    // Camera

    //g_pCamera = new cCameraObject();

    if(!sceneLoader.loadCameraParams(g_camera, error))
    {
        std::cout << "The camera configuration was not loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    
    //g_CameraManager.SetActiveCamera(g_pCamera);

    //// Used by the deferred render
    //g_pFixedCamera = new cCameraObject();
    //g_pFixedCamera->setCameraPosition(glm::vec3(34.0f, 14.0f, 21.84f));
    //g_pFixedCamera->setCameraOrientationY(45.0f);
    //g_pFixedCamera->setCameraOrientationX(-20.0f);
    //g_pFixedCamera->setCameraOrientationZ(0.0f);
    //
    //// Camera manager
    //cGameObject* circleAround = NULL;
    //for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    //{
    //    if (g_vecGameObjects[i]->friendlyName == "curiosity_rover")
    //    {
    //        circleAround = g_vecGameObjects[i];
    //        break;
    //    }
    //}

    //// Create all the scene cameras
    //cCameraObject* pCirclyingCamera = new cCameraObject();
    //pCirclyingCamera->friendlyName = "circlying";
    //glm::vec3 camPos = glm::vec3(169.0f, 23.0f, 137.0f);
    //pCirclyingCamera->setCameraPosition(camPos);
    //pCirclyingCamera->setCameraOrientationY(105.0f);
    //pCirclyingCamera->setCameraOrientationX(-15.0f);
    //g_CameraManager.AddCamera(pCirclyingCamera);

    //g_CameraManager.CircleAroundObject(pCirclyingCamera,
    //                                   circleAround,
    //                                   5.0f,
    //                                   true);

    //cCameraObject* frontView = new cCameraObject();
    //frontView->friendlyName = "frontView";
    //camPos = glm::vec3(82.7017, -17.8f, 161.026f);
    //frontView->setCameraPosition(camPos);
    //frontView->setCameraOrientationY(-90.0f);
    //frontView->setCameraOrientationX(-17.0f);
    //g_CameraManager.AddCamera(frontView);

    //cCameraObject* leftView = new cCameraObject();
    //leftView->friendlyName = "leftView";
    //camPos = glm::vec3(87.3749f, -17.8f, 165.868f);
    //leftView->setCameraPosition(camPos);
    //leftView->setCameraOrientationY(0.0f);
    //leftView->setCameraOrientationX(-17.0f);
    //g_CameraManager.AddCamera(leftView);

    //cCameraObject* backView = new cCameraObject();
    //backView->friendlyName = "backView";
    //camPos = glm::vec3(92.0893, -17.8f, 161.026f);
    //backView->setCameraPosition(camPos);
    //backView->setCameraOrientationY(90.0f);
    //backView->setCameraOrientationX(-17.0f);
    //g_CameraManager.AddCamera(backView);

    //cCameraObject* rightView = new cCameraObject();
    //rightView->friendlyName = "rightView";
    //camPos = glm::vec3(87.3749f, -17.8f, 155.961f);
    //rightView->setCameraPosition(camPos);
    //rightView->setCameraOrientationY(180.0f);
    //rightView->setCameraOrientationX(-17.0f);
    //g_CameraManager.AddCamera(rightView);

    //g_CameraManager.SetActiveCamera(pCirclyingCamera);
    ////g_CameraManager.AutoCircling(4.0f);

    //// Camera end
    ////-------------------------------------------------------------------------
    //// Game objects to be used in the second pass
    //std::vector<cGameObject*> vecSecondPass;
    //cGameObject* tvSet = new cGameObject();
    //tvSet->meshName = "tv_set";
    //tvSet->friendlyName = "tv_set";
    //tvSet->typeOfObject = eTypeOfObject::PLANE;
    //tvSet->textureBlend[0] = 1.0f;
    //tvSet->textureNames[0] = "Steel.bmp";

    //vecSecondPass.push_back(tvSet);
    //vecSecondPass.push_back(g_pSkyBoxObject);
    
    //-------------------------------------------------------------------------
    // Limit planes
    if (!sceneLoader.loadLimitPlanes(error))
    {
        std::cout << "The limit planes configuration was not loaded..." << std::endl;
        std::cout << error << std::endl;
    }

    //// Debug Render
    ////-------------------------------------------------------------------------
    //// Triangle debug renderer test...
    //::g_pDebugRenderer = new cDebugRenderer();
    //if (!::g_pDebugRenderer->initialize(error))
    //{
    //    std::cout << "Warning: couldn't init the debug renderer." << std::endl;
    //    std::cout << "\t" << ::g_pDebugRenderer->getLastError() << std::endl;
    //}

    //::g_pDebugRenderer->addTriangle( glm::vec3( -50.0f, -25.0f, 0.0f ),
				//					 glm::vec3( 0.0, 50.0f, 100.0f ),
				//					 glm::vec3( 50.0f, -25.0, 0.0f),
				//					 glm::vec3( 1.0f, 1.0f, 0.0f ), 1000.0f );

    ////-------------------------------------------------------------------------
    //// Clouds
    //std::vector<GameObjectsInfo> transInfo;
    //for(int i = 0; i < 4; i++)
    //{
    //    GameObjectsInfo ti;
    //    ti.meshName = "Cloud" + std::to_string(i + 1);
    //    ti.texture = "clouds.bmp";
    //    ti.alpha = "clouds_alpha.bmp";
    //    transInfo.push_back(ti);
    //}

    //int numOfClouds = 1000;


    //createRamdomGameObjects(numOfClouds,
    //                        ::g_pTranspManager->transpObjects,
    //                        transInfo,
    //                        -1000.0f, 1000.0f,
    //                        150.0f, 180.0f,
    //                        -1000.0f, 1000.0f);
    //// ------------------------------------------------------------------------

    // Sets the default Game Character
    if (!g_characterManager.SetControlledCharacter("Hero01", error))
    {
        std::cout << "The Game Character was not found!\n";
    }

    // Lock on the main character
    g_camera.lockOnGameObject(g_characterManager.GetActiveCharacter()->GetCharacter());

    //// Set NPC Manager    
    //g_NPCManager.SetPlayer(g_characterManager.GetActiveCharacter());
    //g_NPCManager.SetNPCs(g_characterManager.GetNPCs());

    //glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);

    // Create the FBOs
    g_FBO_fullScene.init(g_scrWidth, g_scrHeight, error);
    g_FBO_deferred.init(g_scrWidth, g_scrHeight, error);
    g_FBO_alpha_shadow.init(128, 128, error);
    g_FBO_shadows.init(8192, 8192, error);

    //-------------------------------------------------------------------------
    // ScreenPlay
    g_pSeceneManager = new cSceneManager();
    g_pSeceneManager->addScreen("Disclaimer.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Disclaimer.bmp"),
                                0.0f);

    cSoundObject* sound = g_pSoundManager->getSoundFromName("Start_Menu");
    g_pSeceneManager->addScreen("Start_Menu.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Start_Menu.bmp"),
                                0.0f,
                                sound);

    g_pSeceneManager->addScreen("Ghosts_n_Goblins_01.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_01.bmp"),
                                3.0f);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_01");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_01.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_01.bmp"),
                                2.236f,
                                sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_02");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_02.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_02.bmp"),
                                3.307f,
                                sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_03");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_03.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_03.bmp"),
                                1.615f,
                                sound);

    sound = g_pSoundManager->getSoundFromName("Start_Demo_04");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_04.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_04.bmp"),
                                1.599f,
                                sound);

    sound = g_pSoundManager->getSoundFromName("Stage_Introduction_Map");
    g_pSeceneManager->addScreen("Ghosts_n_Goblins_05.bmp",
                                g_pTextureManager->getTextureIDFromTextureName("Ghosts_n_Goblins_05.bmp"),
                                4.153f,
                                sound,
                                true);

    sound = g_pSoundManager->getSoundFromName("Ghosts_n_Goblins_Theme_Rock_Metal_BG");
    g_pSeceneManager->addScreen("Main_Game_Screen",
                                g_pTextureManager->getTextureIDFromTextureName("Dummy_Alpha.bmp"),
                                0.0f,
                                sound,
                                false,
                                true);
    g_pSeceneManager->init();
    //-------------------------------------------------------------------------

    // Will be used in the physics step
    double lastTimeStep = glfwGetTime();

    //g_pTranspManager->InitSortingThread();

    // Main game or application loop
    while (!glfwWindowShouldClose(window))
    {
        // Now many seconds that have elapsed since we last checked
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTimeStep;

        //printf("%f frames per second\n", 1.0f / deltaTime);

        // Physics step
        g_pPhysicsWorld->TimeStep(deltaTime);
        
        //g_camera.updateCameraVectors();
        g_environment.update(deltaTime);

        //PhysicsStep(deltaTime);
        lastTimeStep = curTime;

        g_pSeceneManager->update(deltaTime);

        //=====================================================================
        //Sound
        g_pSoundManager->updateSoundScene(g_camera.m_position);
        //=====================================================================

        ::processCameraInput(window, deltaTime);
        //g_CameraManager.Update(glfwGetTime() - lastTimeStep);        

        //g_NPCManager.Evaluate(glfwGetTime() - lastTimeStep);

        //// Will lock the fixed camera on the main character
        //g_pFixedCamera->lockOnCharacter(g_characterManager.GetActiveCharacter()->GetCharacter(), true);
         
        glfwGetFramebufferSize(window, &g_scrWidth, &g_scrHeight);
        if (g_scrHeight == 0)
            g_scrHeight = 1;

        // This is to make sure the resolution will be the right one,  
        // in case we change the screen size
        if (g_scrWidth != g_FBO_fullScene.width || g_scrHeight != g_FBO_fullScene.height)
        {
            g_FBO_fullScene.reset(g_scrWidth, g_scrHeight, error);
            g_FBO_deferred.reset(g_scrWidth, g_scrHeight, error);
        }

        ::g_pShaderManager->useShaderProgram("GE101_Shader");

        // Shadow alpha pass
        glUniform1i(g_uniLocHandler.renderPassNumber, SHADOW_ALPHA_PASS);
        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_alpha_shadow.ID);
        g_FBO_alpha_shadow.clearBuffers();
        glViewport(0, 0, g_FBO_alpha_shadow.width, g_FBO_alpha_shadow.height);
        RenderScene(g_vecGameObjects, g_uniLocHandler.currentProgID);

        // Depth pass
        glUniform1i(g_uniLocHandler.renderPassNumber, DEPTH_RENDER_PASS);
        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_shadows.ID);
        g_FBO_shadows.clearBuffer();
        glViewport(0, 0, g_FBO_shadows.width, g_FBO_shadows.height);
        RenderScene(g_vecGameObjects, g_uniLocHandler.currentProgID);
        glViewport(0, 0, g_scrWidth, g_scrHeight);

        // Full render pass
        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D, g_FBO_shadows.depthTexture_ID);
        glUniform1i(g_uniLocHandler.shadowMap, 20);
        glActiveTexture(GL_TEXTURE0 + 21);
        glBindTexture(GL_TEXTURE_2D, g_FBO_alpha_shadow.colourTexture_0_ID);
        glUniform1i(g_uniLocHandler.shadowAlphaMap, 21);

        glUniform1i(g_uniLocHandler.renderPassNumber, FULL_SCENE_RENDER_PASS);

        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_fullScene.ID);
        g_FBO_fullScene.clearBuffers();

        // Render to the Screen FBO (1st pass)
        RenderScene(g_vecGameObjects, window, g_camera, deltaTime);

        // Render the DEBUG Physics
        if (g_debugEnable)
        {
            ::g_pShaderManager->useShaderProgram("Debug_Shader");
            g_pPhysicsWorld->DebugDrawWorld(g_pShaderManager->getIDFromFriendlyName("Debug_Shader"),
                                            g_camera.getViewMatrix(),
                                            g_camera.m_zoom,
                                            g_scrWidth,
                                            g_scrHeight);
            ::g_pShaderManager->useShaderProgram("GE101_Shader");
        }

        // Now we point it to the FBO texture
        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_deferred.ID);
        g_FBO_deferred.clearBuffers();

        // Second pass (Deferred)
        glUniform1i(g_uniLocHandler.renderPassNumber, DEFERRED_RENDER_PASS);

        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D, g_FBO_fullScene.colourTexture_0_ID);
        glUniform1i(g_uniLocHandler.texFBOColour2D, 20);

        glActiveTexture(GL_TEXTURE0 + 21);
        glBindTexture(GL_TEXTURE_2D, g_FBO_fullScene.normalTexture_1_ID);
        glUniform1i(g_uniLocHandler.texFBONormal2D, 21);

        glActiveTexture(GL_TEXTURE0 + 22);
        glBindTexture(GL_TEXTURE_2D, g_FBO_fullScene.vertexWorldPos_2_ID);
        glUniform1i(g_uniLocHandler.texFBOVertexWorldPos2D, 22);

        glUniform1f(g_uniLocHandler.screenWidth, (float)g_scrWidth);
        glUniform1f(g_uniLocHandler.screenHeight, (float)g_scrHeight);

        // Only drawing the skybox because it fills the entire screen
        RenderScene(g_pSkyBoxObject, window, g_camera, deltaTime);

        // Now we are going to draw to the screen itself. There is going to be 2 draws:
        // 1. We draw the full scene using the regulat FBO texture from "g_FBO_Pass2_Deferred"
        // 2. We draw the screen scene, using a dummy mesh at the right place.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(g_uniLocHandler.renderPassNumber, FINAL_RENDER_PASS);

        // The "deferred pass" FBO has a colour texture with the entire rendered scene
        // (including lighting, etc.)
        if (g_pSeceneManager->isScreenOn())
        {
            glActiveTexture(GL_TEXTURE0 + 20);
            glBindTexture(GL_TEXTURE_2D, g_FBO_deferred.colourTexture_0_ID);
            glUniform1i(g_uniLocHandler.fullRenderedImage2D, 20);

            glActiveTexture(GL_TEXTURE0 + 21);
            glBindTexture(GL_TEXTURE_2D, g_pSeceneManager->getActiveScreen());
            glUniform1i(g_uniLocHandler.fullRenderedImage2D_Overlay, 21);

            glUniform1f(g_uniLocHandler.fade, g_pSeceneManager->getFade());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + 20);
            glBindTexture(GL_TEXTURE_2D, g_FBO_deferred.colourTexture_0_ID);
            glUniform1i(g_uniLocHandler.fullRenderedImage2D, 20);

            glUniform1f(g_uniLocHandler.fade, g_pSeceneManager->getFade());
        }

        // Render the "Full scene
        RenderScene(g_pSkyBoxObject, window, g_camera, deltaTime);

        // End of the Deferred Render
        //=====================================================================================
        
        g_characterManager.UpdateCollisions(glfwGetTime() - lastTimeStep);

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
                
        

        //g_AiManager.updateAi();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }// Main Game Loop end

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete ::g_pShaderManager;
    delete ::g_pVAOManager;
    delete ::g_pSimpleDebug;
    //delete ::g_pDebugRenderer;
    delete ::g_pAABBsManager;
    delete ::g_pSoundManager;
    delete ::g_pTranspManager;
    delete ::g_pTextureManager;
    delete ::g_pSeceneManager;

    return 0;

}