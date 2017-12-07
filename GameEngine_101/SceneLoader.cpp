// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"
#include <glm/glm.hpp>

extern std::vector< cGameObject* >  g_vecGameObjects;

void LoadModelsIntoScene(void)
{
	{
		// Galactica
		cGameObject* pGalactca = new cGameObject();		
        pGalactca->diffuseColour = glm::vec4( 1.4f, 1.4f, 1.4f, 1.0f );
        pGalactca->meshName = "Galactica";
        pGalactca->bIsUpdatedInPhysics = false;
        pGalactca->bIsWireFrame = false;
        pGalactca->position = glm::vec3(0.0f, 0.0f, 0.0f);
        ::g_vecGameObjects.push_back(pGalactca);
	}

    //=========================================================================
    // Create and place 16 Vipers

    for (int i = 0; i < 16; i++)
    {
        // Vipers
        cGameObject* pViper = new cGameObject();
        pViper->diffuseColour = glm::vec4(2.36f, 2.29f, 1.99f, 1.0f);
        pViper->meshName = "Viper";
        pViper->bIsUpdatedInPhysics = true;
        pViper->bIsWireFrame = false;
        pViper->rotateY(180.0f);
        ::g_vecGameObjects.push_back(pViper);
    }

    g_vecGameObjects.at(1)->position = glm::vec3(0.0f, 0.0f, -264.0f);
    g_vecGameObjects.at(2)->position = glm::vec3(10.0f, -2.1f, -264.0f);
    g_vecGameObjects.at(3)->position = glm::vec3(-11.0f, -2.7f, -264.0f);
    g_vecGameObjects.at(4)->position = glm::vec3(20.0f, -4.0f, -265.0f);
    g_vecGameObjects.at(5)->position = glm::vec3(-21.0f, -4.2f, -265.0f);
    g_vecGameObjects.at(6)->position = glm::vec3(30.0f, -5.5f, -263.0f);
    g_vecGameObjects.at(7)->position = glm::vec3(-31.0f, -5.0f, -264.0f);
    g_vecGameObjects.at(8)->position = glm::vec3(42.0f, -6.0f, -264.0f);
    g_vecGameObjects.at(9)->position = glm::vec3(-40.0f, -6.2f, -264.0f);
    g_vecGameObjects.at(10)->position = glm::vec3(52.0f, -7.1f, -263.9f);
    g_vecGameObjects.at(11)->position = glm::vec3(-52.0f, -7.5f, -263.0f);
    g_vecGameObjects.at(12)->position = glm::vec3(62.0f, -8.0f, -230.0f);
    g_vecGameObjects.at(13)->position = glm::vec3(-62.1f, -8.3f, -222.0f);
    g_vecGameObjects.at(14)->position = glm::vec3(70.0f, -9.0f, -220.0f);
    g_vecGameObjects.at(15)->position = glm::vec3(-69.0f, -8.8f, -190.0f);
    g_vecGameObjects.at(16)->position = glm::vec3(79.0f, -9.0f, -180.0f);

    // End of Vipers creation
    //=========================================================================
    
    // Create and place 24 Raiders

    for (int i = 0; i < 24; i++)
    {
        // Raiders
        cGameObject* pRaider = new cGameObject();
        pRaider->diffuseColour = glm::vec4(2.07f, 2.046f, 2.16f, 1.0f);
        pRaider->meshName = "Raider";
        pRaider->bIsUpdatedInPhysics = true;
        pRaider->bIsWireFrame = false;
        pRaider->initSoundObject("Raider");
        ::g_vecGameObjects.push_back(pRaider);
    }

    float alongX = -120.0f;
    for (int i = 17; i < g_vecGameObjects.size(); i++)
    {
        g_vecGameObjects.at(i)->position = glm::vec3(alongX, 0.0f, -470.0f);  
        alongX += 10.0f;
    }
    
    {
        // Raider Dummy
        cGameObject* pRaider = new cGameObject();
        pRaider->diffuseColour = glm::vec4(2.07f, 2.046f, 2.16f, 1.0f);
        pRaider->meshName = "Raider";
        pRaider->bIsUpdatedInPhysics = true;
        pRaider->bIsWireFrame = false;
        ::g_vecGameObjects.push_back(pRaider);
        //g_vecGameObjects.at(g_vecGameObjects.size()-1)->position = glm::vec3(-68.3558f, 7.02644f, 198.422f);
        //g_vecGameObjects.at(g_vecGameObjects.size() - 1)->position = glm::vec3(22.399f, 70.50f, -287.0f);
    }

    // End of Raiders creation
    //=========================================================================

	return;
}
