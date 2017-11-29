// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"
#include <glm/glm.hpp>

extern std::vector< cGameObject* >  g_vecGameObjects;

void LoadModelsIntoScene(void)
{
	{
		// FractalTerrain
		cGameObject* pTerrain = new cGameObject();		
		pTerrain->diffuseColour = glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f );
		pTerrain->meshName = "FractalTerrain";
		pTerrain->bIsUpdatedInPhysics = false;	
		pTerrain->bIsWireFrame = false;
		::g_vecGameObjects.push_back( pTerrain );
	}		

	return;
}
