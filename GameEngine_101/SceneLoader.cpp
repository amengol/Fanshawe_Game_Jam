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
        //theGO->textureBlend[0] = 1.0f;
        //theGO->textureNames[0] = "delorean.bmp";
        theGO->bIsWireFrame = false;
        theGO->position = glm::vec3(0.0f, 0.0f, 0.0f);
        ::g_vecGameObjects.push_back(theGO);
	}

    {
        // Delorean
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Delorean";
        theGO->bIsUpdatedInPhysics = true;
        theGO->bIsWireFrame = false;
        theGO->hasColour = true;
        theGO->position = glm::vec3(0.0f, 48.0f, 294.0f);
        theGO->rotateY(180.0f);
        ::g_vecGameObjects.push_back(theGO);
    }

	return;
}
