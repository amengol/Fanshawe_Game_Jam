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


//// Here, the scene is rendered in 3 passes:
//// 1. Render geometry to G buffer
//// 2. Perform deferred pass, rendering to Deferred buffer
//// 3. Then post-pass ("2nd pass" to the scree)
////    Copying from the Pass2_Deferred buffer to the final screen
//cFBO g_FBO_Pass1_G_Buffer;
//cFBO g_FBO_Pass2_Deferred;
//cFBO g_FBO_Pass1_G_Buffer_Screen;
//cFBO g_FBO_Pass2_Deferred_Screen;

// Fade Control
sFade g_Fade;


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

cPhysics_Switcher g_physicsSwitcher;

bool InitPhysics()
{
    g_pPhysicsFactory = new nPhysics::cPhysicsFactory();
    g_pPhysicsWorld = g_pPhysicsFactory->CreateWorld();
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
unsigned int g_scrWidth = 1260;
unsigned int g_scrHeight = 768;
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
//cSoundManager* g_pSoundManager = NULL;


float g_ChromaticAberrationOffset = 0.0f;
float g_CR_Max = 0.1f;
double g_CA_CountDown = 0.0f;

const int RENDER_PASS_0_G_BUFFER_PASS = 0;
const int RENDER_PASS_1_DEFERRED_RENDER_PASS = 1;
const int RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS = 2;
const int RENDER_PASS_3_FULL_SCREEN_EFFECT_PASS_2 = 3;

// ----------------------------------------------------------------------------

int main()
{
    InitPhysics();

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
    // Load Rigid Bodies
    for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    {
        cGameObject* GO = g_vecGameObjects[i];

        if (GO->rigidBody != NULL)
        {
            g_pPhysicsWorld->AddRigidBody(GO->rigidBody);
        }

        if (GO->bt_rigidBody != NULL)
        {
            //gbt_PhysicsWorld->AddRigidBody(GO->bt_rigidBody);
        }
    }

    
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
     //g_pSoundManager = new cSoundManager();
     //g_pSoundManager->initSoundScene();
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
    g_environment.initLights(currentProgID, g_pLightManager);
    g_environment.setTimeOfDay(cEnvironment::DaySkyLight::NOON);
    //g_environment.setMode(cEnvironment::Mode::CONTINUOUS);
    //-------------------------------------------------------------------------
    // Texture 
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

    //// Create the FBOs
    //g_FBO_Pass1_G_Buffer.init(width, height, error);
    //g_FBO_Pass2_Deferred.init(width, height, error);
    //g_FBO_Pass1_G_Buffer_Screen.init(width, height, error);
    //g_FBO_Pass2_Deferred_Screen.init(width, height, error);

    // Will be used in the physics step
    double lastTimeStep = glfwGetTime();

    //g_pTranspManager->InitSortingThread();

    // Main game or application loop
    while (!glfwWindowShouldClose(window))
    {
        // Now many seconds that have elapsed since we last checked
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTimeStep;

        g_environment.update(deltaTime);
        g_camera.updateCameraVectors();

        //printf("%f frames per second\n", 1.0f / deltaTime);

        // Physics step
        g_pPhysicsWorld->TimeStep(deltaTime);

        //PhysicsStep(deltaTime);
        lastTimeStep = curTime;

        //=====================================================================
        //Sound
        //g_pSoundManager->updateSoundScene(g_pCamera->getCameraPosition());
        //=====================================================================

        ::processCameraInput(window, deltaTime);
        //g_CameraManager.Update(glfwGetTime() - lastTimeStep);        

        //g_NPCManager.Evaluate(glfwGetTime() - lastTimeStep);

        //// Will lock the fixed camera on the main character
        //g_pFixedCamera->lockOnCharacter(g_characterManager.GetActiveCharacter()->GetCharacter(), true);
         
        int curWidth, curHeight;
        glfwGetFramebufferSize(window, &curWidth, &curHeight);

        //// This is to make sure the resolution will be the right one,  
        //// in case we change the screen size
        //if (curWidth != g_FBO_Pass1_G_Buffer.width || curHeight != g_FBO_Pass1_G_Buffer.height)
        //{
        //    g_FBO_Pass1_G_Buffer.shutdown();
        //    g_FBO_Pass2_Deferred.shutdown();
        //    g_FBO_Pass1_G_Buffer.init(curWidth, curHeight, error);
        //    g_FBO_Pass2_Deferred.init(curWidth, curHeight, error);
        //    g_FBO_Pass1_G_Buffer_Screen.shutdown();
        //    g_FBO_Pass2_Deferred_Screen.shutdown();
        //    g_FBO_Pass1_G_Buffer_Screen.init(curWidth, curHeight, error);
        //    g_FBO_Pass2_Deferred_Screen.init(curWidth, curHeight, error);
        //}

        ::g_pShaderManager->useShaderProgram("GE101_Shader");
        GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

        // Direct everything to the FBO
        GLint renderPassNumber_LocID = glGetUniformLocation(shaderID, "renderPassNumber");
        glUniform1i(renderPassNumber_LocID, RENDER_PASS_0_G_BUFFER_PASS);

        //glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass1_G_Buffer_Screen.ID);
        //g_FBO_Pass1_G_Buffer_Screen.clearBuffers();


        //==== Not using FBOs==================================================
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderScene(::g_vecGameObjects, window, g_camera, glfwGetTime() - lastTimeStep);
        //=====================================================================

        //// Render to the Screen FBO (1st pass)
        //RenderScene(::g_vecGameObjects, window, g_CameraManager.GetAcitveCamera(), glfwGetTime() - lastTimeStep);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// Now we point it to the FBO texture
        //glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass2_Deferred_Screen.ID );
        //g_FBO_Pass2_Deferred_Screen.clearBuffers();

        ////glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //// Second pass (Deferred)
        //glUniform1i(renderPassNumber_LocID, RENDER_PASS_1_DEFERRED_RENDER_PASS);

        //GLint texFBOColour2DTextureUnitID = 10;
        //GLint texFBOColour2DLocID = glGetUniformLocation(shaderID, "texFBOColour2D");
        //GLint texFBONormal2DTextureUnitID = 11;
        //GLint texFBONormal2DLocID = glGetUniformLocation(shaderID, "texFBONormal2D");
        //GLint texFBOWorldPosition2DTextureUnitID = 12;
        //GLint texFBOWorldPosition2DLocID = glGetUniformLocation(shaderID, "texFBOVertexWorldPos2D");

        //// Pick a texture unit... 
        //glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer_Screen.colourTexture_0_ID);
        //glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

        //glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer_Screen.normalTexture_1_ID);
        //glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);

        //glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer_Screen.vertexWorldPos_2_ID);
        //glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);

        //glfwGetFramebufferSize(window, &width, &height);

        //GLint screenWidthLocID = glGetUniformLocation(shaderID, "screenWidth");
        //GLint screenHeightLocID = glGetUniformLocation(shaderID, "screenHeight");
        //glUniform1f(screenWidthLocID, (float)width);
        //glUniform1f(screenHeightLocID, (float)height);

        //// Drawing the skybox because it fills the entire screen
        //// Also, we are using the Fixed Camera
        //std::vector< cGameObject* >  vecSkyBox;
        //vecSkyBox.push_back(::g_pSkyBoxObject);
        //RenderScene(vecSkyBox, window, pCirclyingCamera, glfwGetTime() - lastTimeStep);


        //// We now have a "Screen texture" to be used in the screen
        ////-----------------------------------------------------------------------
        //
        ////Now we want to do the process again, for the "normal" scene
        //glUniform1i(renderPassNumber_LocID, RENDER_PASS_0_G_BUFFER_PASS);

        //glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass1_G_Buffer.ID);
        //g_FBO_Pass1_G_Buffer.clearBuffers();

        //// This time o render with the "normal" camera. Also we want to render just the
        //// skybox and the TV Set
        //RenderScene(vecSecondPass, window, g_pCamera, glfwGetTime() - lastTimeStep);

        //// Render it again, but point the the FBO texture... 
        //glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass2_Deferred.ID );
        //g_FBO_Pass2_Deferred.clearBuffers();

        //glUniform1i(renderPassNumber_LocID, RENDER_PASS_1_DEFERRED_RENDER_PASS);

        //texFBOColour2DTextureUnitID = 20;
        //texFBONormal2DTextureUnitID = 21;
        //texFBOWorldPosition2DTextureUnitID = 22;

        //// Pick a texture unit... 
        //glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.colourTexture_0_ID);
        //glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

        //glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.normalTexture_1_ID);
        //glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);

        //glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
        //glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.vertexWorldPos_2_ID);
        //glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);

        //// Set the sampler in the shader to the same texture unit (20)

        //glfwGetFramebufferSize(window, &width, &height);

        //glUniform1f(screenWidthLocID, (float)width);
        //glUniform1f(screenHeightLocID, (float)height);

        //std::vector< cGameObject* >  vecCopy2ndPass2;
        //vecCopy2ndPass2.push_back(::g_pSkyBoxObject);
        //RenderScene(vecCopy2ndPass2, window, g_pCamera, glfwGetTime() - lastTimeStep);

        //// Now we are going to draw to the screen itself. There is going to be 2 draws:
        //// 1. We draw the full scene using the regular FBO texture from "g_FBO_Pass2_Deferred"
        //// 2. We draw the screen scene, using a dummy mesh at the right place.
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glUniform1i(renderPassNumber_LocID, RENDER_PASS_3_FULL_SCREEN_EFFECT_PASS_2);

        //// The "deferred pass" FBO has a colour texture with the entire rendered scene
        //// (including lighting, etc.)
        //GLint fullRenderedImage2D_LocID = glGetUniformLocation(ShaderID, "fullRenderedImage2D");

        //// Pick a texture unit... 
        //unsigned int pass2unit = 50;
        //glActiveTexture(GL_TEXTURE0 + pass2unit);
        //glBindTexture(GL_TEXTURE_2D, ::g_FBO_Pass2_Deferred.colourTexture_0_ID);
        //glUniform1i(fullRenderedImage2D_LocID, pass2unit);

        //// Render the "Full scene
        //RenderScene(vecSkyBox, window, g_pCamera, glfwGetTime() - lastTimeStep);

        //// Now we render to the screen again, but with the dummy "screen" mesh
        //glUniform1i(renderPassNumber_LocID, RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS);

        //// Pick a texture unit... 
        //unsigned int pass2unit2 = 55;
        //glActiveTexture(GL_TEXTURE0 + pass2unit2);
        //glBindTexture(GL_TEXTURE_2D, ::g_FBO_Pass2_Deferred_Screen.colourTexture_0_ID);
        //glUniform1i(fullRenderedImage2D_LocID, pass2unit2);

        //std::vector< cGameObject* >  vecCopySingleLonelyQuad2;
        //cGameObject* screen = new cGameObject();
        //screen->meshName = "tv_screen";
        //
        //// This uniform is used to put and overlay on top of the screen
        //GLint fullRenderedImage2D_Overlay_LocID = glGetUniformLocation(ShaderID, "fullRenderedImage2D_Overlay");

        //// Pick a texture unit... 
        //pass2unit2 = 56;
        //glActiveTexture(GL_TEXTURE0 + pass2unit2);
        //glBindTexture(GL_TEXTURE_2D, ::g_pTextureManager->getTextureIDFromTextureName("static.bmp"));
        //glUniform1i(fullRenderedImage2D_Overlay_LocID, pass2unit2);


        //// Static Effect
        //GLint static_LocID = glGetUniformLocation(ShaderID, "staticEffect");
        //float noiseFactor = getRandInRange(0.0f, 1.0f);
        //// And the noise
        //glUniform1f(static_LocID, noiseFactor);

        //// Static Fade
        //g_Fade.Update(glfwGetTime() - lastTimeStep);
        //GLint static_Fade_LocID = glGetUniformLocation(ShaderID, "staticFade");
        //// And the fade
        //glUniform1f(static_Fade_LocID, g_Fade.mFade);

        //vecCopySingleLonelyQuad2.push_back(screen);
        //RenderScene(vecCopySingleLonelyQuad2, window, g_pCamera, glfwGetTime() - lastTimeStep);


        //// Clean things
        //delete screen;

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
    //delete ::g_pSoundManager;
    delete ::g_pTranspManager;

    return 0;

}