// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"
#include <glm/glm.hpp>

extern std::vector< cGameObject* >  g_vecGameObjects;

void LoadModelsIntoScene(void)
{
	{
		// Fractal Terrain
		cGameObject* theGO = new cGameObject();		
        theGO->diffuseColour = glm::vec4( 1.4f, 1.4f, 1.4f, 1.0f );
        theGO->meshName = "FractalTerrain";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->position = glm::vec3(0.0f, 0.0f, 0.0f);
        ::g_vecGameObjects.push_back(theGO);
	}

	return;
}
