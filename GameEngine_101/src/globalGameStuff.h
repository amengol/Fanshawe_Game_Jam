#ifndef _globalGameStuff_HG_
#define _globalGameStuff_HG_

// This is all the global 'game' stuff, so:
// - not OpenGL rendering stuff (unless we can't help it)
// - just 'engine' or game play stuff

#include "cGameObject.h"
#include <vector>
#include <glm/vec3.hpp>
#include "cLightManager.h"
#include "cModelAssetLoader.h"
#include "cCameraObject.h"
#include "Assimp\cAnimationCollection.h"
#include "cVAOMeshManager.h"
#include "cDebugRenderer.h"
#include "cShaderManager.h"
#include "Texture/cTextureManager.h"
#include "cUniLocHandler.h"
#include <iPhysicsFactory.h>
#include <cPhysicsWorld.h>
#include <cPhysicsFactory.h>
#include <cPhysicsWorld.h>
#include <cPhysicsFactory.h>
#include "cSimpleDebugRenderer.h"
#include "cTransparencyManager.h"
//#include "cTextManager.h"
//#include "cLocalization.h"
#include "AI\cCharacterManager.h"
//#include "AI\cNPCManager.h"
#include "cCameraManger.h"
#include "cCamera.h"
#include "cEnvironment.h"
#include "cSceneManager.h"


// Finds a GameObject by a friendly name. Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects);

// Finds a GameObject by a unique ID
cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects);

// Super basic physics update function
void PhysicsStep(double deltaTime);

extern cSceneManager* g_pSeceneManager;                 // (GE101_Main.cpp)
extern bool g_propsEnabled;                             // (GLFW_callbacks.cpp)
extern bool g_debugEnable;                              // (GE101_Main.cpp)
extern float g_gamaCorrection;                          // (DrawCalls.cpp)
extern int g_scrWidth;                                  // (GE101_Main.cpp)
extern int g_scrHeight;                                 // (GE101_Main.cpp)
extern cCamera g_camera;                                // (GE101_Main.cpp)
extern bool g_clothDebug;                               // (DrawCalls.cpp)
extern cEnvironment g_environment;                      // (GE101_Main.cpp)
//extern cCameraObject* g_pCamera;                        // (GE101_Main.cpp)
//extern cNPCManager g_NPCManager;
//extern cTextManager g_textManager;                      // (GE101_Main.cpp)
extern cGameObject* g_pSkyBoxObject;                    // (GE101_Main.cpp)
//extern cCameraManger g_CameraManager;
//extern cLocalization g_lococalization;                  // (GE101_Main.cpp)
extern cUniLocHandler g_uniLocHandler;                  // (GE101_Main.cpp)
extern cLightManager* g_pLightManager;	                // (GE101_Main.cpp)
extern cVAOMeshManager* g_pVAOManager;		            // (GE101_Main.cpp)
//extern cDebugRenderer* g_pDebugRenderer;	            // (GE101_Main.cpp)
extern cShaderManager* g_pShaderManager;	            // (GE101_Main.cpp)
extern CTextureManager*	g_pTextureManager;	            // (GE101_Main.cpp)
extern cCharacterManager g_characterManager;            // (cSceneLoader.cpp)
extern cSimpleDebugRenderer* g_pSimpleDebug;            // (GE101_Main.cpp)
extern cTransparencyManager* g_pTranspManager;          // (GE101_Main.cpp)
extern cModelAssetLoader* g_pModelAssetLoader;	        // (ModelUtilities.cpp)
extern nPhysics::iPhysicsWorld* g_pPhysicsWorld;        // (GE101_Main.cpp)
extern cAnimationCollection g_animationCollection;      // (ModelUtilities.cpp)
extern std::vector<cGameObject*> g_vecGameObjects;      // (GE101_Main.cpp)
extern nPhysics::iPhysicsFactory* g_pPhysicsFactory;    // (GE101_Main.cpp)

#endif
