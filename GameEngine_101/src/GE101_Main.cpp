/*
*
*   Game Engine 101
*   By Jorge Amengol
*   2018
*
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
#include <cCloth.h>
#include "DrawCalls.h"
#include "AI\cNPCManager.h"
//#include "../Cloth.h"
#include "cFBO.h"


// Here, the scene is rendered in 3 passes:
// 1. Render geometry to G buffer
// 2. Perform deferred pass, rendering to Deferred buffer
// 3. Then post-pass ("2nd pass" to the scree)
//    Copying from the Pass2_Deferred buffer to the final screen
cFBO g_FBO_Pass1_G_Buffer;
cFBO g_FBO_Pass2_Deferred;



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

    //gbt_PhysicsFactory = new nPhysics::bt_cPhysicsFactory();
    //gbt_PhysicsWorld = gbt_PhysicsFactory->CreateWorld();
    return true;
}


// Function Prototypes
//void DrawObject(cGameObject* pTheGO);
//void DrawAABB(cGameObject* pTheGO, float size);
//void DrawAABBforPoints(std::vector<glm::vec3> vertices, float AABBSize);
//bool compare(cGameObject* i, cGameObject* j);

// Global variables
cVAOMeshManager* g_pVAOManager = NULL;
cCameraObject* g_pCamera = NULL;
cGameObject* g_pSkyBoxObject = NULL;
cShaderManager*	g_pShaderManager = NULL;
cLightManager*	g_pLightManager = NULL;
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
float g_FOV = 0.6f;
//cSimpleAi_Manager g_AiManager;
//cLocalization g_lococalization;
//cTextManager g_textManager;

// To deal with sounds
//cSoundManager* g_pSoundManager = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float g_ChromaticAberrationOffset = 0.0f;
float g_CR_Max = 0.1f;
double g_CA_CountDown = 0.0f;

const int RENDER_PASS_0_G_BUFFER_PASS = 0;
const int RENDER_PASS_1_DEFERRED_RENDER_PASS = 1;
const int RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS = 2;

int main()
{
    InitPhysics();

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    int height = 480;
    int width = 640;
    std::string title = "Game Engine 101";

    std::ifstream infoFile("GameConfig.ini");

    if (!infoFile.is_open())
    {    // File didn't open...
        std::cout << "Can't find config file" << std::endl;
        std::cout << "Using defaults" << std::endl;
    }
    else
    {    // File DID open, so read it... 
        std::string a;

        infoFile >> a;    // "Game"
        infoFile >> a;    // "Config"
        infoFile >> a;    // "width"

        infoFile >> width;

        infoFile >> a;    // "height"

        infoFile >> height;

        infoFile >> a;    // Title_Start

        std::stringstream ssTitle;
        bool bKeepReading = true;
        do
        {
            infoFile >> a;
            if (a != "Title_End")
            {
                ssTitle << a << " ";
            }
            else
            {    // it IS the end! 
                bKeepReading = false;
                title = ssTitle.str();
            }
        } while (bKeepReading);

    }//if ( ! infoFile.is_open() )

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

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
    
    //-------------------------------------------------------------------------
    // Texture 
    if(!loadTextures())
        std::cout << "Something went wrong while loading the textures!\n";

    //-------------------------------------------------------------------------
    // Camera

    g_pCamera = new cCameraObject();

    if(!sceneLoader.loadCameraParams(g_pCamera, error))
    {
        std::cout << "The camera configuration was not loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    
    // Camera end
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

    //-------------------------------------------------------------------------
    // Clouds
    //std::vector<GameObjectsInfo> transInfo;
    //for(int i = 0; i < 4; i++)
    //{
    //    GameObjectsInfo ti;
    //    ti.meshName = "Cloud" + std::to_string(i + 1);
    //    ti.texture = "clouds.bmp";
    //    ti.alpha = "clouds_alpha.bmp";
    //    transInfo.push_back(ti);
    //}

    //createRamdomGameObjects(100,
    //                        ::g_pTranspManager->transpObjects,
    //                        transInfo,
    //                        -1000.0f, 1000.0f,
    //                        150.0f, 180.0f,
    //                        -1000.0f, 1000.0f);
    // ------------------------------------------------------------------------

    // Sets the default Game Character
    if (!g_characterManager.SetControlledCharacter("Hero", error))
    {
        std::cout << "The Game Character was not found!\n";
    }

    // Set NPC Manager
    cNPCManager NPCManager;
    NPCManager.SetPlayer(g_characterManager.GetActiveCharacter());
    NPCManager.SetNPCs(g_characterManager.GetNPCs());

    glEnable(GL_DEPTH);

    // Create an FBO
    if (!::g_FBO_Pass1_G_Buffer.init(width, height, error))
    {
        std::cout << "::g_FBO_Pass2_Deferred error: " << error << std::endl;
    }
    else
    {
        std::cout << "::g_FBO_Pass2_Deferred is good." << std::endl;
        std::cout << "\t::g_FBO_Pass1_G_Buffer ID = " << ::g_FBO_Pass1_G_Buffer.ID << std::endl;
        std::cout << "\tcolour texture ID = " << ::g_FBO_Pass1_G_Buffer.colourTexture_0_ID << std::endl;
        std::cout << "\tnormal texture ID = " << ::g_FBO_Pass1_G_Buffer.normalTexture_1_ID << std::endl;

        std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::g_FBO_Pass1_G_Buffer.getMaxColourAttachments() << std::endl;
        std::cout << "GL_MAX_DRAW_BUFFERS = " << ::g_FBO_Pass1_G_Buffer.getMaxDrawBuffers() << std::endl;

    }//if ( ! ::g_FBO_Pass1_G_Buffer.init

    if (!::g_FBO_Pass2_Deferred.init(width, height, error))
    {
        std::cout << "::g_FBO_Pass2_Deferred error: " << error << std::endl;
    }
    else
    {
        std::cout << "FBO is good." << std::endl;
        std::cout << "\t::g_FBO_Pass2_Deferred ID = " << ::g_FBO_Pass2_Deferred.ID << std::endl;
        std::cout << "\tcolour texture ID = " << ::g_FBO_Pass2_Deferred.colourTexture_0_ID << std::endl;
        std::cout << "\tnormal texture ID = " << ::g_FBO_Pass2_Deferred.normalTexture_1_ID << std::endl;

        std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::g_FBO_Pass2_Deferred.getMaxColourAttachments() << std::endl;
        std::cout << "GL_MAX_DRAW_BUFFERS = " << ::g_FBO_Pass2_Deferred.getMaxDrawBuffers() << std::endl;

    }//if ( ! ::g_FBO_Pass2_Deferred.init

    // Will be used in the physics step
    double lastTimeStep = glfwGetTime();

    // Main game or application loop
    while (!glfwWindowShouldClose(window))
    {
        //=====================================================================
        //Sound
        //g_pSoundManager->updateSoundScene(g_pCamera->getCameraPosition());
        //=====================================================================

        NPCManager.Evaluate(glfwGetTime() - lastTimeStep);

        //    ___                _             _           ___       _            __   __           
        //   | _ \ ___  _ _   __| | ___  _ _  | |_  ___   / __| ___ | |__  _  _  / _| / _| ___  _ _ 
        //   |   // -_)| ' \ / _` |/ -_)| '_| |  _|/ _ \ | (_ ||___|| '_ \| || ||  _||  _|/ -_)| '_|
        //   |_|_\\___||_||_|\__,_|\___||_|    \__|\___/  \___|     |_.__/ \_,_||_|  |_|  \___||_|  
        //                                                                        
        // In this pass, we render all the geometry to the "G buffer"
        // The lighting is NOT done here. 
         
        int curWidth, curHeight;
        glfwGetFramebufferSize(window, &curWidth, &curHeight);

        if (curWidth != g_FBO_Pass1_G_Buffer.width || curHeight != g_FBO_Pass1_G_Buffer.height)
        {
            g_FBO_Pass1_G_Buffer.shutdown();
            g_FBO_Pass2_Deferred.shutdown();
            g_FBO_Pass1_G_Buffer.init(curWidth, curHeight, error);
            g_FBO_Pass2_Deferred.init(curWidth, curHeight, error);
        }

        ::g_pShaderManager->useShaderProgram("GE101_Shader");
        GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

        // Direct everything to the FBO
        GLint renderPassNumber_LocID = glGetUniformLocation(shaderID, "renderPassNumber");
        glUniform1i(renderPassNumber_LocID, RENDER_PASS_0_G_BUFFER_PASS);

        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass1_G_Buffer.ID);
        // Clear colour AND depth buffer
        g_FBO_Pass1_G_Buffer.clearBuffers();

        RenderScene(::g_vecGameObjects, window, glfwGetTime() - lastTimeStep);

        //    ___         __                         _   ___                _             ___               
        //   |   \  ___  / _| ___  _ _  _ _  ___  __| | | _ \ ___  _ _   __| | ___  _ _  | _ \ __ _  ___ ___
        //   | |) |/ -_)|  _|/ -_)| '_|| '_|/ -_)/ _` | |   // -_)| ' \ / _` |/ -_)| '_| |  _// _` |(_-<(_-<
        //   |___/ \___||_|  \___||_|  |_|  \___|\__,_| |_|_\\___||_||_|\__,_|\___||_|   |_|  \__,_|/__//__/
        //   
        // In this pass, we READ from the G-buffer, and calculate the lighting. 
        // In this example, we are writing to another FBO, now. 
        // 


        // Render it again, but point the the FBO texture... 
        glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass2_Deferred.ID );
        g_FBO_Pass2_Deferred.clearBuffers();

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ::g_pShaderManager->useShaderProgram("GE101_Shader");

        glUniform1i(renderPassNumber_LocID, RENDER_PASS_1_DEFERRED_RENDER_PASS);

        //uniform sampler2D texFBONormal2D;
        //uniform sampler2D texFBOVertexWorldPos2D;

        GLint texFBOColour2DTextureUnitID = 10;
        GLint texFBOColour2DLocID = glGetUniformLocation(shaderID, "texFBOColour2D");
        GLint texFBONormal2DTextureUnitID = 11;
        GLint texFBONormal2DLocID = glGetUniformLocation(shaderID, "texFBONormal2D");
        GLint texFBOWorldPosition2DTextureUnitID = 12;
        GLint texFBOWorldPosition2DLocID = glGetUniformLocation(shaderID, "texFBOVertexWorldPos2D");

        // Pick a texture unit... 
        glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
        glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.colourTexture_0_ID);
        glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

        glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
        glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.normalTexture_1_ID);
        glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);

        glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
        glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.vertexWorldPos_2_ID);
        glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);

        // Set the sampler in the shader to the same texture unit (20)

        glfwGetFramebufferSize(window, &width, &height);

        GLint screenWidthLocID = glGetUniformLocation(shaderID, "screenWidth");
        GLint screenHeightLocID = glGetUniformLocation(shaderID, "screenHeight");
        glUniform1f(screenWidthLocID, (float)width);
        glUniform1f(screenHeightLocID, (float)height);

        //// Adjust the CA:
        ////float g_ChromaticAberrationOffset = 0.0f;
        //// 0.1
        //if (g_CA_CountDown > 0.0)
        //{
        //    float CRChange = getRandInRange((-g_CR_Max / 10.0f), (g_CR_Max / 10.0f));
        //    g_ChromaticAberrationOffset += CRChange;
        //    g_CA_CountDown -= glfwGetTime() - lastTimeStep;
        //}
        //else
        //{
        //    g_ChromaticAberrationOffset = 0.0f;
        //}

        //GLint CROffset_LocID = glGetUniformLocation(shaderID, "CAoffset");
        //glUniform1f(CROffset_LocID, g_ChromaticAberrationOffset);

        std::vector< cGameObject* >  vecCopy2ndPass;
        // Push back a SINGLE quad or GIANT triangle that fills the entire screen
        // Here we will use the skybox (as it fills the entire screen)
        vecCopy2ndPass.push_back(::g_pSkyBoxObject);
        RenderScene(vecCopy2ndPass, window, glfwGetTime() - lastTimeStep);


        //    ___  _              _   ___  ___    ___               
        //   | __|(_) _ _   __ _ | | |_  )|   \  | _ \ __ _  ___ ___
        //   | _| | || ' \ / _` || |  / / | |) | |  _// _` |(_-<(_-<
        //   |_|  |_||_||_|\__,_||_| /___||___/  |_|  \__,_|/__//__/
        //                                                          	
        // Here, we read from the off screen buffer, the one that 
        // has all the lighting, etc. 
        // This is where we can do the "2nd pass effects", so the 
        // full-screen 2D effects.
        //
        // NOTE: In this example, we are only using this to render to an offscreen object

        // Now the final pass (in this case, only rendering to a quad)
        //RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ::g_pShaderManager->useShaderProgram("GE101_Shader");

        glUniform1i(renderPassNumber_LocID, RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS);

        // The "deferred pass" FBO has a colour texture with the entire rendered scene
        // (including lighting, etc.)
        GLint fullRenderedImage2D_LocID = glGetUniformLocation(ShaderID, "fullRenderedImage2D");

        // Pick a texture unit... 
        unsigned int pass2unit = 50;
        glActiveTexture(GL_TEXTURE0 + pass2unit);
        glBindTexture(GL_TEXTURE_2D, ::g_FBO_Pass2_Deferred.colourTexture_0_ID);
        glUniform1i(fullRenderedImage2D_LocID, pass2unit);


        std::vector< cGameObject* >  vecCopySingleLonelyQuad;
        cGameObject* screen = new cGameObject();
        screen->meshName = "Stadium_Screen01";
        screen->friendlyName = "Stadium_Screen01";
        screen->typeOfObject = eTypeOfObject::PLANE;
        screen->textureBlend[0] = 1.0f;
        screen->textureNames[0] = "Dummy_Screen.bmp";
        screen->position = glm::vec3(0.0f, -15.0f, 0.0f);

        // Push back a SINGLE quad or GIANT triangle that fills the entire screen
        vecCopySingleLonelyQuad.push_back(screen);
        RenderScene(vecCopySingleLonelyQuad, window, glfwGetTime() - lastTimeStep);











        // Prints camera information to the title
        std::stringstream ssTitle;
        glm::vec3 curCameraPosition = g_pCamera->getCameraPosition();
        glm::vec3 curCameraLookAt = g_pCamera->getLookAtPosition();
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
                
        // Now many seconds that have elapsed since we last checked
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTimeStep;

        // Physics step
        switch (g_physicsSwitcher.gPhysicsEngine)
        {
        case g_physicsSwitcher.SUPERDUPER:
            g_pPhysicsWorld->TimeStep(deltaTime);
            break;
        case g_physicsSwitcher.BULLET:
            //gbt_PhysicsWorld->TimeStep(deltaTime);
            break;
        default:
            break;
        }
                
        //PhysicsStep(deltaTime);
        lastTimeStep = curTime;

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