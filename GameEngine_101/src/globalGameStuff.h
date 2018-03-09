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

// Remember to #include <vector>...
extern std::vector< cGameObject* >  g_vecGameObjects;

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects);
cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects);

extern cCameraObject* g_pCamera;            // (GE101_Main.cpp)

extern cLightManager*	g_pLightManager;	// (GE101_Main.cpp)

                                            // Super basic physics update function
void PhysicsStep( double deltaTime );

extern cModelAssetLoader* g_pModelAssetLoader;	// (ModelUtilies.cpp)

extern cAnimationCollection gAnimationCollection;   // ModelUtilities.cpp

// The teapots
extern cGameObject* pTP0;


#endif
