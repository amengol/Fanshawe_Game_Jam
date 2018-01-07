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
#include "cSimpleDebugRenderer.h"
#include "TextureLoader.h"

using namespace std;

// Function Prototypes
void DrawObject(cGameObject* pTheGO);
void DrawAABB(cGameObject* pTheGO, float size);
void DrawAABBforPoints(std::vector<glm::vec3> vertices, float AABBSize);

// Global variables
cVAOMeshManager* g_pVAOManager = NULL;
cCameraObject* g_pCamera = NULL;
cShaderManager*	g_pShaderManager = NULL;
cLightManager*	g_pLightManager = NULL;
//cDebugRenderer*	g_pDebugRenderer = NULL;
cSimpleDebugRenderer* g_simpleDebug = NULL;
cAABBsManager* g_pAABBsManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;
std::vector< cGameObject* >  g_vecGameObjects;
std::map<long long, miniVAOInfo> g_map_AABBID_miniVAO;
long long g_cubeID = -1;
long long g_lineID = -1;
float g_AABBSize = 5.0f;

// To deal with sounds
// Disabled for now
// cSoundManager* g_pSoundManager = NULL;


// Other uniforms:
GLint uniLoc_materialDiffuse = -1;
GLint uniLoc_materialAmbient = -1;
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;         // rgb = colour of HIGHLIGHT only | w = shininess of the 
GLint uniLoc_bIsDebugWireFrameObject = -1;
GLint uniLoc_HasColour = -1;
GLint uniLoc_HasAlpha = -1;

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

    ////-------------------------------------------------------------------------
    //// AABBs
    //::g_pAABBsManager = new cAABBsManager();
    //cMesh terrain;
    //::g_pVAOManager->lookupMeshFromName("FlatMesh", terrain);
    //::g_pAABBsManager->genAABBs(&terrain, g_AABBSize);
    ////::g_pAABBsManager->genAllAABBsDebugLines();
    ////-------------------------------------------------------------------------
    //// Simple Debug Renderer
    //::g_simpleDebug = new cSimpleDebugRenderer();
    //if(!::g_simpleDebug->genDebugGeometry(DEBUG_CUBE, g_AABBSize, g_cubeID))
    //{
    //    std::cout << "genDebugGeometry: There was en error generating a geometry!\n";
    //}
    //if(!::g_simpleDebug->genDebugGeometry(DEBUG_LINE, 1.0f, g_lineID))
    //{
    //    std::cout << "genDebugGeometry: There was en error generating a geometry!\n";
    //}

    //-------------------------------------------------------------------------
    // Debug render
    //::g_pDebugRenderer = new cDebugRenderer();
    //if (!::g_pDebugRenderer->initialize(error))
    //{
    //    std::cout << "Warning: couldn't init the debug renderer." << std::endl;
    //}

    //for (int i = 0; i < ::g_pAABBsManager->vDebugLines.size(); i++)
    //{
    //    glm::vec3 lineStart = ::g_pAABBsManager->vDebugLines[i].lineStart;
    //    glm::vec3 lineEnd = ::g_pAABBsManager->vDebugLines[i].lineEnd;
    //    glm::vec3 color = ::g_pAABBsManager->vDebugLines[i].color;

    //    ::g_pDebugRenderer->addLine(lineStart, lineEnd, color, true);
    //}

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
    uniLoc_HasColour = glGetUniformLocation(currentProgID, "hasColour");
    uniLoc_HasAlpha = glGetUniformLocation(currentProgID, "hasAlpha");
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
    // Texture 
    if(!loadTextures())
        std::cout << "Something went wrong while loading the textures!\n";

    //-------------------------------------------------------------------------
    // Camera

    g_pCamera = new cCameraObject();
    g_pCamera->setCameraPosition(glm::vec3(0.0f, 70.0f, 170.0f));
    g_pCamera->setCameraOrientationX(-10.0f);

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
        matProjection = glm::perspective(0.6f,			// FOV
                        ratio,		// Aspect ratio
                        1.0f,			// Near (as big as possible)
                        10000.0f);	// Far (as small as possible)

        g_pCamera->update();

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
            
            //if(pTheGO->typeOfObject == SPHERE)
            //{
            //    // Calculate all AABBs for the sphere
            //    // Put the sphere inside an axis-aligned box

            //    // Vertices
            //    float diameter = pTheGO->radius * 2;
            //    std::vector<glm::vec3> vertices;
            //    glm::vec3 vertex0 = glm::vec3(pTheGO->position - pTheGO->radius);
            //    vertices.push_back(vertex0);
            //    vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z));
            //    vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z));
            //    vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z));
            //    vertices.push_back(glm::vec3(vertex0.x, vertex0.y, vertex0.z + diameter));
            //    vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z + diameter));
            //    vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z + diameter));
            //    vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z + diameter));

            //    DrawAABBforPoints(vertices, g_AABBSize);
            //}
            //else
            //{
            //    DrawAABB(pTheGO, g_AABBSize);
            //}            
        }

        

        //::g_pDebugRenderer->RenderDebugObjects(matView, matProjection);

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
                
        // Now many seconds that have elapsed since we last checked
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTimeStep;

        // Physics step
        PhysicsStep(deltaTime);
        lastTimeStep = curTime;

        // Move this here before Physics step to be able to print 
        // the SimpleDebugRender
        // WARNING! ============================================== 
        // Should move this to before the physics step
        // if no debug render is being done
        glfwSwapBuffers(window);
        glfwPollEvents();

    }// Main Game Loop end

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete ::g_pShaderManager;
    delete ::g_pVAOManager;
    //delete ::g_pDebugRenderer;
    //delete ::g_pAABBsManager;
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
    
    if (pTheGO->hasColour)
    {
        glUniform1f(uniLoc_HasColour, 1.0f);
    }
    else
    {
        glUniform1f(uniLoc_HasColour, 0.0f);
    }

    if(pTheGO->hasAlpha)
    {
        glUniform1f(uniLoc_HasAlpha, 1.0f);
    } else
    {
        glUniform1f(uniLoc_HasAlpha, 0.0f);
    }
    

    //...and all the other object material colours

    if (pTheGO->bIsWireFrame)
    {
        glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);	// TRUE
    }
    else
    {
        glUniform1f(uniLoc_bIsDebugWireFrameObject, 0.0f);	// FALSE
    }

    // Set up the textures
    std::string textureName = pTheGO->textureNames[0];
    GLuint texture00Number
        = ::g_pTextureManager->getTextureIDFromName(textureName);
    // Texture binding... (i.e. set the 'active' texture
    GLuint texture00Unit = 0;							// Texture units go from 0 to 79 (at least)
    glActiveTexture(texture00Unit + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, texture00Number);

    // 0 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
        ::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[0]));
    // 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
        ::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[1]));
    // 2..  and so on... 

    // Set sampler in the shader
    // NOTE: You shouldn't be doing this during the draw call...
    GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    GLint textSampler00_ID = glGetUniformLocation(curShaderID, "myAmazingTexture00");
    GLint textSampler01_ID = glGetUniformLocation(curShaderID, "myAmazingTexture01");
    //// And so on (up to 10, or whatever number of textures)... 

    GLint textBlend00_ID = glGetUniformLocation(curShaderID, "textureBlend00");
    GLint textBlend01_ID = glGetUniformLocation(curShaderID, "textureBlend01");

    // This connects the texture sampler to the texture units... 
    glUniform1i( textSampler00_ID, 0  );		// Enterprise
    glUniform1i( textSampler01_ID, 1  );		// GuysOnSharkUnicorn
    // .. and so on

    // And the blending values
    glUniform1f(textBlend00_ID, pTheGO->textureBlend[0]);
    glUniform1f(textBlend01_ID, pTheGO->textureBlend[1]);
    // And so on...

    //			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    //			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    if (pTheGO->bIsWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }
    else if (pTheGO->cullFace)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    glCullFace(GL_BACK);


    glBindVertexArray(VAODrawInfo.VAO_ID);

    glDrawElements(GL_TRIANGLES, VAODrawInfo.numberOfIndices, GL_UNSIGNED_INT, 0);

    // Unbind that VAO
    glBindVertexArray(0);

    return;
}

// Helper function to draw a debug cube at an AABB location
void DrawAABB(cGameObject* pTheGO, float size)
{
    if(!pTheGO->isDebugAABBActive)
    {
        return;
    }
    
    // Calculate the ID of the GameObject
    long long GO_ID = -999;

    if(!g_pAABBsManager->calcID(pTheGO->position, GO_ID, size))
    {
        std::cout << "Problem generating AABB ID during DrawAABB() function!\n";
        return;
    }

    // Check if the ID exists
    if(!g_pAABBsManager->findAABB(GO_ID))
    {
        return;
    }

    glm::vec3 min = g_pAABBsManager->genVecFromID(GO_ID, size);

    glm::vec3 cubeColor = glm::vec3(1.0f, 0.0f, 0.0f);
    g_simpleDebug->drawDebugGeometry(min, g_cubeID, cubeColor, glm::mat4(1.0f));

    // Print the normals
    cAABB theAABB(0, 0.0f);
    if(!g_pAABBsManager->getAABB(GO_ID, theAABB))
    {
        //error
        return;
    }

    for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
    {
        glm::vec3 fn = theAABB.AABBsTriangles[i]->faceNormal;
        glm::vec3 cn = theAABB.AABBsTriangles[i]->Centroid;
        glm::quat qNormal = glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), fn);
        glm::mat4 matNormal = glm::toMat4(qNormal);
        glm::vec3 normalColor = glm::vec3(0.0f, 0.0f, 1.0f);
        g_simpleDebug->drawDebugGeometry(cn, g_lineID, normalColor, matNormal);
    }

}

void DrawAABBforPoints(std::vector<glm::vec3> vertices, float size)
{
    for(int i = 0; i < vertices.size(); i++)
    {
        // Calculate the ID of the GameObject
        long long GO_ID = -999;

        if(!g_pAABBsManager->calcID(vertices[i], GO_ID, size))
        {
            std::cout << "Problem generating AABB ID during DrawAABB() function!\n";
            return;
        }

        // Check if the ID exists
        if(!g_pAABBsManager->findAABB(GO_ID))
        {
            continue;
        }

        glm::vec3 min = g_pAABBsManager->genVecFromID(GO_ID, size);

        glm::vec3 cubeColor = glm::vec3(1.0f, 0.0f, 0.0f);
        g_simpleDebug->drawDebugGeometry(min, g_cubeID, cubeColor, glm::mat4(1.0f));

        // Print the normals
        cAABB theAABB(0, 0.0f);
        if(!g_pAABBsManager->getAABB(GO_ID, theAABB))
        {
            //error
            return;
        }

        for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
        {
            glm::vec3 fn = theAABB.AABBsTriangles[i]->faceNormal;
            glm::vec3 cn = theAABB.AABBsTriangles[i]->Centroid;
            glm::quat qNormal = glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), fn);
            glm::mat4 matNormal = glm::toMat4(qNormal);
            glm::vec3 normalColor = glm::vec3(0.0f, 0.0f, 1.0f);
            g_simpleDebug->drawDebugGeometry(cn, g_lineID, normalColor, matNormal);
        }
    }

}

