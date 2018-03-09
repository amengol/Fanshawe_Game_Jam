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
#include "cPhysics_Switcher.h"
#include "cVAOMeshManager.h"
#include "cDebugRenderer.h"

// Finds a GameObject by a friendly name. Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects);

// Finds a GameObject by a unique ID
cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects);

// Super basic physics update function
void PhysicsStep(double deltaTime);

extern cCameraObject* g_pCamera;                    // (GE101_Main.cpp)
extern cLightManager* g_pLightManager;	            // (GE101_Main.cpp)
extern cPhysics_Switcher gPhysicsSwitcher;          // (GE101_Main.cpp)
extern cModelAssetLoader* g_pModelAssetLoader;	    // (ModelUtilities.cpp)
extern cAnimationCollection gAnimationCollection;   // (ModelUtilities.cpp)
extern std::vector<cGameObject*> g_vecGameObjects;  // (GE101_Main.cpp)
extern cVAOMeshManager* g_pVAOManager;		// (theMain.cpp)
extern cDebugRenderer* g_pDebugRenderer;	// (theMain.cpp)

#endif
