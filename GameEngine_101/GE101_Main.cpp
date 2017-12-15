/*
*
*   Game Engine 101
*   By Jorge Amengol
*   2017
*
*/

#include "globalOpenGL_GLFW.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include "cCameraObject.h"
#include "cShaderManager.h"
#include "cVAOMeshManager.h"
#include "ModelUtilities.h"
#include "cGameObject.h"
#include "cLightManager.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cSoundManager.h"
#include "Physics.h"
#include "globalGameStuff.h"
#include "cAABBsManager.h"

using namespace std;

// Function Prototypes
void DrawObject(cGameObject* pTheGO);

// Global variables
cVAOMeshManager* g_pVAOManager = NULL;
cCameraObject* g_pCamera = NULL;
cShaderManager*	g_pShaderManager = NULL;
cLightManager*	g_pLightManager = NULL;
cDebugRenderer*	g_pDebugRenderer = NULL;
cAABBsManager* g_pAABBsManager = NULL;
std::vector< cGameObject* >  g_vecGameObjects;

// To deal with sounds
// Disabled for now
// cSoundManager* g_pSoundManager = NULL;


// Other uniforms:
GLint uniLoc_materialDiffuse = -1;
GLint uniLoc_materialAmbient = -1;
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;         // rgb = colour of HIGHLIGHT only | w = shininess of the 
GLint uniLoc_bIsDebugWireFrameObject = -1;

GLint uniLoc_eyePosition = -1;	            // Camera position
GLint uniLoc_mModel = -1;
GLint uniLoc_mView = -1;
GLint uniLoc_mProjection = -1;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
    GLFWwindow* window;
    GLint mvp_location;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    int height = 480;
    int width = 640;
    string title = "Game Engine 101";

    ifstream infoFile("GameConfig.ini");

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

    vertShader.fileName = "simpleVert.glsl";
    fragShader.fileName = "simpleFrag.glsl";

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
        
    //-------------------------------------------------------------------------
    // Load models
        
    ::g_pModelAssetLoader = new cModelAssetLoader();
    ::g_pModelAssetLoader->setBasePath("assets/models/");

    ::g_pVAOManager = new cVAOMeshManager();

    GLint ShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    if (!Load3DModelsIntoMeshManager(ShaderID, ::g_pVAOManager, ::g_pModelAssetLoader, error))
    {
        std::cout << "Not all models were loaded..." << std::endl;
        std::cout << error << std::endl;
    }
    LoadModelsIntoScene();

    //-------------------------------------------------------------------------
    // AABBs
    ::g_pAABBsManager = new cAABBsManager();
    cMesh terrain;
    ::g_pVAOManager->lookupMeshFromName("FractalTerrain", terrain);
    ::g_pAABBsManager->genAABBs(&terrain, 1.0f);
    ::g_pAABBsManager->genDebugLines();

    //-------------------------------------------------------------------------
    // Debug render
    ::g_pDebugRenderer = new cDebugRenderer();
    if (!::g_pDebugRenderer->initialize(error))
    {
        std::cout << "Warning: couldn't init the debug renderer." << std::endl;
    }

    for (int i = 0; i < ::g_pAABBsManager->vDebugLines.size(); i++)
    {
        glm::vec3 lineStart = ::g_pAABBsManager->vDebugLines[i].lineStart;
        glm::vec3 lineEnd = ::g_pAABBsManager->vDebugLines[i].lineEnd;
        glm::vec3 color = ::g_pAABBsManager->vDebugLines[i].color;

        ::g_pDebugRenderer->addLine(lineStart, lineEnd, color, true);
    }

    //=========================================================================
    // Sound things
    // g_pSoundManager = new cSoundManager();
    // g_pSoundManager->initSoundScene();
    //=========================================================================

    // End of loading models
    //-------------------------------------------------------------------------

    GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    mvp_location = glGetUniformLocation(currentProgID, "MVP");
    uniLoc_materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");
    uniLoc_materialAmbient = glGetUniformLocation(currentProgID, "materialAmbient");
    uniLoc_ambientToDiffuseRatio = glGetUniformLocation(currentProgID, "ambientToDiffuseRatio");
    uniLoc_materialSpecular = glGetUniformLocation(currentProgID, "materialSpecular");
    uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(currentProgID, "bIsDebugWireFrameObject");
    uniLoc_eyePosition = glGetUniformLocation(currentProgID, "eyePosition");
    uniLoc_mModel = glGetUniformLocation(currentProgID, "mModel");
    uniLoc_mView = glGetUniformLocation(currentProgID, "mView");
    uniLoc_mProjection = glGetUniformLocation(currentProgID, "mProjection");

    //-------------------------------------------------------------------------
    // Lights

    ::g_pLightManager = new cLightManager();

    ::g_pLightManager->CreateLights(9);	// There are 10 lights in the shader
    ::g_pLightManager->LoadShaderUniformLocations(currentProgID);

    // Change ZERO (the SUN) light position
    ::g_pLightManager->vecLights[0].position = glm::vec3(0.0f, 5000.0f, 0.0f);
    ::g_pLightManager->vecLights[0].attenuation.x = 2.5f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[0].attenuation.y = 0.0f;		// Change the linear attenuation
    
    // Lights end
    //-------------------------------------------------------------------------
    // Camera

    g_pCamera = new cCameraObject();
    g_pCamera->setCameraPosition(glm::vec3(0.0f, 50.0f, 300.0f)); // Question5

    // Camera end
    //-------------------------------------------------------------------------

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

        float ratio;
        int width, height;        
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        // Clear colour AND depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ::g_pShaderManager->useShaderProgram("GE101_Shader");
        GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

        // Update all the light uniforms...
        // (for the whole scene)
        ::g_pLightManager->CopyLightInformationToCurrentShader();

        glm::mat4x4 matProjection;

        // Projection and view don't change per scene (maybe)
        matProjection = glm::perspective(0.6f,			// FOV
                        ratio,		// Aspect ratio
                        1.0f,			// Near (as big as possible)
                        10000.0f);	// Far (as small as possible)

        // View or "camera" matrix
        glm::mat4 matView = glm::mat4(1.0f);

        matView = glm::lookAt(g_pCamera->getCameraPosition(),		// "eye" or "camera" position
                  g_pCamera->getLookAtPosition(),				// "At" or "target" 
                  g_pCamera->getCameraUpVector());	// "up" vector

        glUniformMatrix4fv(uniLoc_mView, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matView));
        glUniformMatrix4fv(uniLoc_mProjection, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matProjection));

        //---------------------------------------------------------------------
        // "Draw scene" loop

        unsigned int sizeOfVector = (unsigned int)::g_vecGameObjects.size();
        for (int index = 0; index != sizeOfVector; index++)
        {
            cGameObject* pTheGO = ::g_vecGameObjects[index];

            DrawObject(pTheGO);
        }

        ::g_pDebugRenderer->RenderDebugObjects(matView, matProjection);

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
            // For lights information
            //<< " Light attenuation - Constant:"
            //<< ::g_pLightManager->vecLights[0].attenuation.x
            //<< " Linear:"
            //<< ::g_pLightManager->vecLights[0].attenuation.y
            //<< " Quadratic:"
            //<< ::g_pLightManager->vecLights[0].attenuation.z;

        glfwSetWindowTitle(window, ssTitle.str().c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Now many seconds that have elapsed since we last checked
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTimeStep;

        // Physics step
        PhysicsStep(deltaTime);
        lastTimeStep = curTime;

    }// Main Game Loop end

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete ::g_pShaderManager;
    delete ::g_pVAOManager;
    delete g_pDebugRenderer;
    //delete ::g_pSoundManager;

    return 0;

}

// Draw a single object
void DrawObject(cGameObject* pTheGO)
{
    // Is there a game object? 
    if (pTheGO == 0)
    {
        return;
    }

    // Was near the draw call, but we need the mesh name
    std::string meshToDraw = pTheGO->meshName;

    sVAOInfo VAODrawInfo;
    if (::g_pVAOManager->lookupVAOFromName(meshToDraw, VAODrawInfo) == false)
    {	// Didn't find mesh
        return;
    }

    // There IS something to draw

    glm::mat4x4 mModel = glm::mat4x4(1.0f);

    

    glm::mat4 trans = glm::mat4x4(1.0f);
    trans = glm::translate(trans, pTheGO->position);
    mModel = mModel * trans;    

    mModel = mModel * pTheGO->orientation;
    //glm::mat4 matRotX = glm::mat4x4(1.0f);
    //matRotX = glm::rotate(matRotX, pTheGO->orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    //mModel = mModel * matRotX;

    //glm::mat4 matRotY = glm::mat4x4(1.0f);
    //matRotY = glm::rotate(matRotY, pTheGO->orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //mModel = mModel * matRotY;

    //glm::mat4 matRotZ = glm::mat4x4(1.0f);
    //matRotZ = glm::rotate(matRotZ, pTheGO->orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    //mModel = mModel * matRotZ;

    float finalScale = pTheGO->scale;

    glm::mat4 matScale = glm::mat4x4(1.0f);
    matScale = glm::scale(matScale, glm::vec3(finalScale, finalScale, finalScale));
    mModel = mModel * matScale;

    glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mModel));

    glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

    glUniform4f(uniLoc_materialDiffuse,
        pTheGO->diffuseColour.r,
        pTheGO->diffuseColour.g,
        pTheGO->diffuseColour.b,
        pTheGO->diffuseColour.a);

    //...and all the other object material colours

    if (pTheGO->bIsWireFrame)
    {
        glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);	// TRUE
    }
    else
    {
        glUniform1f(uniLoc_bIsDebugWireFrameObject, 0.0f);	// FALSE
    }



    //			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    //			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    if (pTheGO->bIsWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    glCullFace(GL_BACK);


    glBindVertexArray(VAODrawInfo.VAO_ID);

    glDrawElements(GL_TRIANGLES, VAODrawInfo.numberOfIndices, GL_UNSIGNED_INT, 0);

    // Unbind that VAO
    glBindVertexArray(0);

    return;
}

