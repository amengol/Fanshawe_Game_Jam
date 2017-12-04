/*
*   Game Engine 101
*   By Jorge Amengol
*   2017
*
*   Version 0.0.1
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

using namespace std;

// Function Prototypes
void DrawObject(cGameObject* pTheGO);
void PhysicsStep(double deltaTime);
static void key_callback(GLFWwindow* window, 
                         int key, 
                         int scancode, 
                         int action, 
                         int mods);

// Global variables
cVAOMeshManager* g_pVAOManager = NULL;
cCameraObject* g_pCamera = NULL;
cShaderManager*	g_pShaderManager = NULL;
cLightManager*	g_pLightManager = NULL;
std::vector< cGameObject* >  g_vecGameObjects;
glm::vec3 g_cameraXYZ = glm::vec3(0.0f, 51.0f, 122.0f);
glm::vec3 g_cameraTarget_XYZ = glm::vec3(0.0f, 0.0f, 0.0f);

// Variable to store the camera target through all objects
int g_objectTurn = 1;


// Other uniforms:
GLint uniLoc_materialDiffuse = -1;
GLint uniLoc_materialAmbient = -1;
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;         // rgb = colour of HIGHLIGHT only | w = shininess of the 
GLint uniLoc_bIsDebugWireFrameObject = -1;
GLint uniLoc_bUseTheVertexColourAsDiffuse = -1;

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

    ::g_pVAOManager = new cVAOMeshManager();

    GLint ShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    std::string error;
    if (!Load3DModelsIntoMeshManager(ShaderID, ::g_pVAOManager, error))
    {
        std::cout << "Not all models were loaded..." << std::endl;
        std::cout << error << std::endl;
    }    

    extern void LoadModelsIntoScene(void);
    LoadModelsIntoScene();

    // End of loading models
    //-------------------------------------------------------------------------

    GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    mvp_location = glGetUniformLocation(currentProgID, "MVP");
    uniLoc_materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");
    uniLoc_materialAmbient = glGetUniformLocation(currentProgID, "materialAmbient");
    uniLoc_ambientToDiffuseRatio = glGetUniformLocation(currentProgID, "ambientToDiffuseRatio");
    uniLoc_materialSpecular = glGetUniformLocation(currentProgID, "materialSpecular");
    uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(currentProgID, "bIsDebugWireFrameObject");
    uniLoc_bUseTheVertexColourAsDiffuse = glGetUniformLocation(currentProgID, "bUseTheVertexColourAsDiffuse");
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
    ::g_pLightManager->vecLights[0].position = glm::vec3(0.0f, 400.0f, 0.0f);
    ::g_pLightManager->vecLights[0].attenuation.x = 1.6f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[0].attenuation.y = 0.005f;		// Change the linear attenuation

    // Engine1 light
    ::g_pLightManager->vecLights[1].position = glm::vec3(-32.0799f, 21.81f, 241.454f);
    ::g_pLightManager->vecLights[1].diffuse = glm::vec3( 1.74f, 2.15f, 3.0f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[1].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[1].attenuation.y = 0.05f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[1].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Engine2 light
    ::g_pLightManager->vecLights[2].position = glm::vec3(32.0799f, 21.81f, 241.454f);
    ::g_pLightManager->vecLights[2].diffuse = glm::vec3( 1.74f, 2.15f, 3.0f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[2].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[2].attenuation.y = 0.05f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[2].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Engine3 light
    ::g_pLightManager->vecLights[3].position = glm::vec3(-28.4753f, -13.04f, 223.99f);
    ::g_pLightManager->vecLights[3].diffuse = glm::vec3( 1.74f, 2.15f, 3.0f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[3].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[3].attenuation.y = 0.05f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[3].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Engine4 light
    ::g_pLightManager->vecLights[4].position = glm::vec3(28.4753f, -13.04f, 223.99f);
    ::g_pLightManager->vecLights[4].diffuse = glm::vec3(1.74f, 2.15f, 3.0f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[4].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[4].attenuation.y = 0.05f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[4].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Back Left Landing Pod light
    ::g_pLightManager->vecLights[5].position = glm::vec3(-69.3394f, 5.96f, 97.5041f);
    ::g_pLightManager->vecLights[5].diffuse = glm::vec3(0.9176f, 0.76f, 0.054f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[5].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[5].attenuation.y = 0.01f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[5].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Back Right Landing Pod light
    ::g_pLightManager->vecLights[6].position = glm::vec3(69.3394f, 5.96f, 97.5041f);
    ::g_pLightManager->vecLights[6].diffuse = glm::vec3(0.9176f, 0.76f, 0.054f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[6].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[6].attenuation.y = 0.01f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[6].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Front Left Landing Pod light
    ::g_pLightManager->vecLights[7].position = glm::vec3(-69.3394f, 5.96f, -66.681f);
    ::g_pLightManager->vecLights[7].diffuse = glm::vec3(0.9176f, 0.76f, 0.054f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[7].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[7].attenuation.y = 0.01f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[7].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Front Right Landing Pod light
    ::g_pLightManager->vecLights[8].position = glm::vec3(69.3394f, 5.96f, -66.681f);
    ::g_pLightManager->vecLights[8].diffuse = glm::vec3(0.9176f, 0.76f, 0.054f);		// Change the costant attenuation
    ::g_pLightManager->vecLights[8].attenuation.x = 1.0f;		// Change the costant attenuation
    ::g_pLightManager->vecLights[8].attenuation.y = 0.01f;		// Change the linear attenuation
    ::g_pLightManager->vecLights[8].attenuation.z = 0.04f;		// Change the quadratic attenuation

    // Lights end
    //-------------------------------------------------------------------------
    // Camera

    g_pCamera = new cCameraObject();
    g_pCamera->setCameraPosition(glm::vec3(-148.048f, 20.427f, 266.741f)); // Question5
    //g_pCamera->setCameraPosition(glm::vec3(-206.226, 79.9552f, -606.804f)); // Question4
    //g_pCamera->setCameraPosition(glm::vec3(0.0f, 90.0f, -46.0f)); // Bridge position
    //g_pCamera->setCameraPosition(glm::vec3(22.399f, -0.50f, -287.0f)); // Near the viper
    g_pCamera->setCameraOrientationY(-30.0f);
    g_pCamera->setCameraOrientationX(-10.0f);

    // Camera end
    //-------------------------------------------------------------------------

    glEnable(GL_DEPTH);

    // Will be used in the physics step
    double lastTimeStep = glfwGetTime();

    // Main game or application loop
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4x4 p, mvp;
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

        // Projection and view don't change per scene (maybe)
        p = glm::perspective(0.6f,			// FOV
            ratio,		// Aspect ratio
            0.1f,			// Near (as big as possible)
            1000.0f);	// Far (as small as possible)

        // View or "camera" matrix
        glm::mat4 v = glm::mat4(1.0f);

        v = glm::lookAt(g_pCamera->getCameraPosition(),		// "eye" or "camera" position
            g_pCamera->getLookAtPosition(),				// "At" or "target" 
            g_pCamera->getCameraUpVector());	// "up" vector

        glUniformMatrix4fv(uniLoc_mView, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(v));
        glUniformMatrix4fv(uniLoc_mProjection, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(p));

        //---------------------------------------------------------------------
        // "Draw scene" loop

        unsigned int sizeOfVector = ::g_vecGameObjects.size();
        for (int index = 0; index != sizeOfVector; index++)
        {
            cGameObject* pTheGO = ::g_vecGameObjects[index];

            DrawObject(pTheGO);
        }

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
        PhysicsStep(deltaTime);
        lastTimeStep = curTime;

        glfwSwapBuffers(window);
        glfwPollEvents();

    }// Main Game Loop end

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete ::g_pShaderManager;
    delete ::g_pVAOManager;

    return 0;

}

static void key_callback(GLFWwindow* window, 
                         int key, 
                         int scancode, 
                         int action, 
                         int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Switch camera targets
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (g_objectTurn < g_vecGameObjects.size())    
        {
            g_pCamera->setCameraTarget(g_vecGameObjects.at(g_objectTurn)->position);
            g_objectTurn++;
        }
        else
        {
            g_objectTurn = 1; // Because interested objects start at 1
            g_pCamera->setCameraTarget(g_vecGameObjects.at(g_objectTurn)->position);
            g_objectTurn++;
        }
    }

    // The Raiders move toward the Galactica
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        // Raiders initial object
        int firstRaider = 17;
        for (int i = firstRaider; i < g_vecGameObjects.size(); i++)
        {
            g_vecGameObjects.at(i)->vel.z = 10.0f;
        }
    }

    // Prepare one viper to land
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        g_vecGameObjects.at(1)->position = glm::vec3(-68.3558f, 7.02644f, 198.422f);
        //g_vecGameObjects.at(1)->vel.z = 10.0f;
      
    }

    if (key == GLFW_KEY_SPACE);

    // Camera movements
    const float ROTANGLE = 1.0f;
    const float CAMSPEED = 1.0f;
    switch (key)
    {
    case GLFW_KEY_W:       // Move camera forward along local Z axis 
        g_pCamera->moveCameraBackNForth(-CAMSPEED);
        break;
    case GLFW_KEY_S:       // Move camera backward along local Z axis 
        g_pCamera->moveCameraBackNForth(CAMSPEED);
        break;
    case GLFW_KEY_A:        // Move camera right along local X axis
        g_pCamera->moveCameraLeftNRight(-CAMSPEED);
        break;
    case GLFW_KEY_D:        // Move camera right along local x axis
        g_pCamera->moveCameraLeftNRight(CAMSPEED);
        break;
    case GLFW_KEY_Z:        // rotate around local camera Z axis +
        g_pCamera->setCameraOrientationZ(ROTANGLE);
        break;
    case GLFW_KEY_C:        // rotate around local camera Z axis -
        g_pCamera->setCameraOrientationZ(-ROTANGLE);
        break;
    case GLFW_KEY_LEFT:     // rotate around local camera Y axis +
        g_pCamera->setCameraOrientationY(ROTANGLE);
        break;
    case GLFW_KEY_RIGHT:    // rotate around local camera Y axis -
        g_pCamera->setCameraOrientationY(-ROTANGLE);
        break;
    case GLFW_KEY_Q:        // Increase high along Y axis
        g_pCamera->changeAlongY(CAMSPEED);
        break;
    case GLFW_KEY_E:        // Decrease high along Y axis
        g_pCamera->changeAlongY(-CAMSPEED);
        break;
    case GLFW_KEY_UP:       // rotate around local camera X axis +
        g_pCamera->setCameraOrientationX(ROTANGLE);
        break;
    case GLFW_KEY_DOWN:     // rotate around local camera X axis -
        g_pCamera->setCameraOrientationX(-ROTANGLE);
        break;
    }// switch ( key )

    // // Press SHIFT to change the lighting parameters
    //if (bIsShiftPressedAlone(mods))
    //{
    //    switch (key)
    //    {
    //    case GLFW_KEY_A:        // Left
    //        ::g_pLightManager->vecLights[0].position.x -= CAMERASPEED;
    //        break;
    //    case GLFW_KEY_D:        // Right
    //        ::g_pLightManager->vecLights[0].position.x += CAMERASPEED;
    //        break;
    //    case GLFW_KEY_W:        // Forward (along z)
    //        ::g_pLightManager->vecLights[0].position.z += CAMERASPEED;
    //        break;
    //    case GLFW_KEY_S:        // Backwards (along z)
    //        ::g_pLightManager->vecLights[0].position.z -= CAMERASPEED;
    //        break;
    //    case GLFW_KEY_Q:        // "Down" (along y axis)
    //        ::g_pLightManager->vecLights[0].position.y -= CAMERASPEED;
    //        break;
    //    case GLFW_KEY_E:        // "Up" (along y axis)
    //        ::g_pLightManager->vecLights[0].position.y += CAMERASPEED;
    //        break;
    //    case GLFW_KEY_1:
    //        ::g_pLightManager->vecLights[0].attenuation.y *= 0.99f;    // less 1%
    //        break;
    //    case GLFW_KEY_2:
    //        ::g_pLightManager->vecLights[0].attenuation.y *= 1.01f; // more 1%
    //        if (::g_pLightManager->vecLights[0].attenuation.y <= 0.0f)
    //        {
    //            ::g_pLightManager->vecLights[0].attenuation.y = 0.001f;    // Some really tiny value
    //        }
    //        break;
    //    case GLFW_KEY_3:    // Quad
    //        ::g_pLightManager->vecLights[0].attenuation.z *= 0.99f;    // less 1%
    //        break;
    //    case GLFW_KEY_4:    //  Quad
    //        ::g_pLightManager->vecLights[0].attenuation.z *= 1.01f; // more 1%
    //        if (::g_pLightManager->vecLights[0].attenuation.z <= 0.0f)
    //        {
    //            ::g_pLightManager->vecLights[0].attenuation.z = 0.001f;    // Some really tiny value
    //        }
    //        break;

    //    case GLFW_KEY_9:
    //        ::g_bDrawDebugLightSpheres = true;
    //        break;
    //    case GLFW_KEY_0:
    //        ::g_bDrawDebugLightSpheres = false;
    //        break;

    //    }// switch ( key )

    //}//if ( bIsShiftPressedAlone(mods) )

    // // HACK: print output to the console
    //std::cout << "Light[0] linear atten: "
    //    << ::g_pLightManager->vecLights[0].attenuation.y << ", "
    //    << ::g_pLightManager->vecLights[0].attenuation.z << std::endl;

    return;
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

    glUniform1f(uniLoc_bUseTheVertexColourAsDiffuse, 0.0f);

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

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{    
    const glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, 0.0f);

    // Identical to the 'render' (drawing) loop
    for (int index = 0; index != ::g_vecGameObjects.size(); index++)
    {
        cGameObject* pCurGO = ::g_vecGameObjects[index];

        // Is this object to be updated?
        if (!pCurGO->bIsUpdatedInPhysics)
        {
            continue;
        }

        pCurGO->Update(deltaTime, GRAVITY);
              

    }//for ( int index... 

    return;
}