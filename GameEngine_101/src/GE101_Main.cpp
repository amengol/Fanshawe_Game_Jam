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
cTextManager g_textManager;

// To deal with sounds
//cSoundManager* g_pSoundManager = NULL;




static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

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
    
    if (!g_textManager.init())
    {
        std::cout << "There was an error initiating the text manager!" << std::endl;
    }
    std::vector<std::wstring> ws;
    ws.push_back(L"[1] Lock on Player");
    ws.push_back(L"[2] First Person Mode");
    ws.push_back(L"[0] Free Camera");
    g_textManager.setText(ws, glm::vec3(0.0f, 0.0f, 1.0f));    
    //-------------------------------------------------------------------------

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

    // Will be used in the physics step
    double lastTimeStep = glfwGetTime();

    // Main game or application loop
    while (!glfwWindowShouldClose(window))
    {
        //=====================================================================
        //Sound
        //g_pSoundManager->updateSoundScene(g_pCamera->getCameraPosition());
        //=====================================================================

        double timeForNPCs = glfwGetTime() - lastTimeStep;
        NPCManager.Evaluate(timeForNPCs);

        float ratio;
        int width, height;        
        glfwGetFramebufferSize(window, &width, &height);

        // Prevent division by zero!
        if (height == 0)
            height = 1;

        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        // Clear colour AND depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);	   
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ::g_pShaderManager->useShaderProgram("GE101_Shader");
        GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

        // Update all the light uniforms...
        // (for the whole scene)
        ::g_pLightManager->CopyLightInformationToCurrentShader();

        //---------------------------------------------------------------------
        // Camera block

        glm::mat4x4 matProjection;

        // Projection and view don't change per scene (maybe)
        matProjection = glm::perspective(g_FOV,			// FOV
                                         ratio,		    // Aspect ratio
                                         1.0f,		   	// Near (as big as possible)
                                         200000.0f);    // Far (as small as possible)

        g_pCamera->update();

        ::g_pSkyBoxObject->position = g_pCamera->getCameraPosition();

        // View or "camera" matrix
        glm::mat4 matView = glm::mat4(1.0f);

        matView = glm::lookAt(g_pCamera->getCameraPosition(),		// "eye" or "camera" position
                  g_pCamera->getLookAtPosition(),				// "At" or "target" 
                  g_pCamera->getCameraUpVector());	// "up" vector

        glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matView));
        glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matProjection));

        //---------------------------------------------------------------------
        // "Draw scene" loop

        unsigned int sizeOfVector = (unsigned int)::g_vecGameObjects.size();
        for (int index = 0; index != sizeOfVector; index++)
        {
            cGameObject* pTheGO = ::g_vecGameObjects[index];

            if (pTheGO->typeOfObject == CLOTH)
            {
                ClothDraw(pTheGO);
            }
            else
            {
                DrawObject(pTheGO);
            }
            
            // For the AABBs
            //if(pTheGO->isDebugAABBActive)
            //{
            //    if(pTheGO->typeOfObject == SPHERE)
            //    {
            //        // Calculate all AABBs for the sphere
            //        // Put the sphere inside an axis-aligned box

            //        // Vertices
            //        float diameter = pTheGO->radius * 2;
            //        std::vector<glm::vec3> vertices;
            //        glm::vec3 vertex0 = glm::vec3(pTheGO->position - pTheGO->radius);
            //        vertices.push_back(vertex0);
            //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y, vertex0.z + diameter));
            //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z + diameter));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z + diameter));
            //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z + diameter));

            //        DrawAABBforPoints(vertices, g_AABBSize);
            //    } 
            //    else if(pTheGO->typeOfObject == MESH)
            //    {
            //        // Calculate all AABBs for the mesh
            //        // Put the mesh inside an axis-aligned box

            //        // Take the mesh extents
            //        cMesh theMesh;
            //        if (!g_pVAOManager->lookupMeshFromName(pTheGO->meshName, theMesh))
            //        {
            //            // Can't find the mesh
            //            continue;
            //        }

            //        float extent = theMesh.maxExtent;

            //        // Vertices
            //        std::vector<glm::vec3> vertices;
            //        glm::vec3 vertex0(0.0f);
            //        vertex0 = pTheGO->position - (extent / 2.0f);
            //        vertices.push_back(vertex0);
            //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + extent, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y + extent, vertex0.z));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y, vertex0.z + extent));
            //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y, vertex0.z + extent));
            //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + extent, vertex0.z + extent));
            //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y + extent, vertex0.z + extent));

            //        DrawAABBforPoints(vertices, g_AABBSize);
            //    }
            //    else
            //    {
            //        DrawAABB(pTheGO, g_AABBSize);
            //    }
            //}
          
        }

        //// Now Draw the transparent objects
        //::g_pTranspManager->sortObjects();
        //int numTransObjects = ::g_pTranspManager->transpObjects.size();
        //for(int i = 0; i < numTransObjects; i++)
        //{
        //    if(::g_pTranspManager->transpObjects[i]->rotateToCamera)
        //    {
        //        // Orient the cloud to the camera
        //        turnGameObjectToCamera(::g_pTranspManager->transpObjects[i], g_pCamera->getCameraPosition());
        //    }
        //    
        //    DrawObject(::g_pTranspManager->transpObjects[i]);

        //    // For the AABBs
        //    if(::g_pTranspManager->transpObjects[i]->isDebugAABBActive)
        //    {
        //        DrawAABB(::g_pTranspManager->transpObjects[i], g_AABBSize);
        //    }
        //}
        
        ////// Draw localization text ---------------------------------------------
        //g_lococalization.draw(width, height);
        g_textManager.draw(width, height);

        double timeForDebugRender = glfwGetTime() - lastTimeStep;

        //::g_pDebugRenderer->RenderDebugObjects(matView, matProjection, timeForDebugRender);

        // "Draw scene" loop end
        //---------------------------------------------------------------------

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