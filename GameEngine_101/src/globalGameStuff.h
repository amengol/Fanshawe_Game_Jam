/**
 * This is all the global 'game' stuff, so:
 * - not OpenGL rendering stuff (unless we can't help it)
 * - just 'engine' or game play stuff
 */
#pragma once

#include "cGameObject.h"
#include <vector>
#include "cLightManager.h"
#include "cModelAssetLoader.h"
#include "Assimp/cAnimationCollection.h"
#include "cVAOMeshManager.h"
#include "cShaderManager.h"
#include "Texture/cTextureManager.h"
#include "cUniLocHandler.h"
#include <iPhysicsFactory.h>
#include <cPhysicsWorld.h>
#include "cTransparencyManager.h"
#include "AI/cCharacterManager.h"
#include "cCamera.h"
#include "cEnvironment.h"
#include "cSceneManager.h"
#include "AI/cNPCManager.h"

/**
 * \brief Finds a GameObject by a friendly name. Returns nullptr if not found
 */
cGameObject* FindObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*>& gameObjects);

// Finds a GameObject by a unique ID. Returns nullptr if not found
cGameObject* FindObjectByUniqueId(unsigned int id, std::vector<cGameObject*>& gameObjects);

// Super basic physics update function
void PhysicsStep(double deltaTime);

extern cSceneManager* g_pSeceneManager;                 // (GE101_Main.cpp)
extern bool g_propsEnabled;                             // (GLFW_callbacks.cpp)
extern bool g_isCharacterMoving;                        // (GLFW_callbacks.cpp)
extern bool gDebugEnable;                              // (GE101_Main.cpp)
extern float g_gamaCorrection;                          // (DrawCalls.cpp)
extern int g_scrWidth;                                  // (GE101_Main.cpp)
extern int g_scrHeight;                                 // (GE101_Main.cpp)
extern cCamera g_camera;                                // (GE101_Main.cpp)
extern bool g_clothDebug;                               // (DrawCalls.cpp)
extern cEnvironment g_environment;                      // (GE101_Main.cpp)
//extern cCameraObject* g_pCamera;                      // (GE101_Main.cpp)
extern cNPCManager g_NPCManager;                        // (GE101_Main.cpp)
//extern cTextManager g_textManager;                    // (GE101_Main.cpp)
extern cGameObject* g_pSkyBoxObject;                    // (GE101_Main.cpp)
//extern cCameraManger g_CameraManager;
//extern cLocalization g_lococalization;                  // (GE101_Main.cpp)
extern cUniLocHandler g_uniLocHandler;                  // (GE101_Main.cpp)
extern cLightManager* g_pLightManager;	                // (GE101_Main.cpp)
extern cVAOMeshManager* g_pVaoManager;		            // (GE101_Main.cpp)
//extern cDebugRenderer* g_pDebugRenderer;	            // (GE101_Main.cpp)
extern cShaderManager* g_pShaderManager;	            // (GE101_Main.cpp)
extern CTextureManager*	g_pTextureManager;	            // (GE101_Main.cpp)
extern cCharacterManager g_characterManager;            // (cSceneLoader.cpp)
extern cTransparencyManager* g_pTranspManager;          // (GE101_Main.cpp)
extern cModelAssetLoader* g_pModelAssetLoader;	        // (ModelUtilities.cpp)
extern nPhysics::iPhysicsWorld* g_pPhysicsWorld;        // (GE101_Main.cpp)
extern cAnimationCollection g_animationCollection;      // (ModelUtilities.cpp)
extern std::vector<cGameObject*> g_vecGameObjects;      // (GE101_Main.cpp)
extern nPhysics::iPhysicsFactory* g_pPhysicsFactory;    // (GE101_Main.cpp)
