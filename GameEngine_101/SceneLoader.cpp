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
		::g_vecGameObjects.push_back(pGalactca);
	}

    //=========================================================================
    // Create and place 24 Vipers

    for (int i = 0; i < 16; i++)
    {
        // Viper1
        cGameObject* pViper = new cGameObject();
        pViper->diffuseColour = glm::vec4(2.36f, 2.29f, 1.99f, 1.0f);
        pViper->meshName = "Viper";
        pViper->bIsUpdatedInPhysics = false;
        pViper->bIsWireFrame = false;
        // Rotate the Viper 180 degrees around the Y axis
        pViper->orientation2.y = glm::radians(180.0f);
        glm::mat4x4 RotY180(1.0f);
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
    g_vecGameObjects.at(12)->position = glm::vec3(62.0f, -8.0f, -263.0f);
    g_vecGameObjects.at(13)->position = glm::vec3(-62.1f, -8.3f, -263.0f);
    g_vecGameObjects.at(14)->position = glm::vec3(70.0f, -9.0f, -261.0f);
    g_vecGameObjects.at(15)->position = glm::vec3(-69.0f, -8.8f, -263.0f);
    g_vecGameObjects.at(16)->position = glm::vec3(79.0f, -9.0f, -242.0f);
    /*g_vecGameObjects.at(17)->position = glm::vec3(-80.0f, -9.0f, -242.1f);
    g_vecGameObjects.at(18)->position = glm::vec3(85.0f, -9.7f, -232.0f);
    g_vecGameObjects.at(19)->position = glm::vec3(-84.0f, -10.0f, -232.0f);
    g_vecGameObjects.at(20)->position = glm::vec3(92.0f, -15.3f, -222.0f);
    g_vecGameObjects.at(21)->position = glm::vec3(-93.0f, -15.0f, -221.5f);
    g_vecGameObjects.at(22)->position = glm::vec3(100.0f, -15.5f, -210.2f);
    g_vecGameObjects.at(23)->position = glm::vec3(-102.0f, -15.8f, -208.0f);
    g_vecGameObjects.at(24)->position = glm::vec3(110.0f, -16.0f, -200.0f);*/

    // End of Vipers creation
    //=========================================================================
    
    // Create and place 24 Raiders

    for (int i = 0; i < 24; i++)
    {
        // Viper1
        cGameObject* pRaider = new cGameObject();
        pRaider->diffuseColour = glm::vec4(1.035f, 1.023f, 1.08f, 1.0f);
        pRaider->meshName = "Raider";
        pRaider->bIsUpdatedInPhysics = false;
        pRaider->bIsWireFrame = false;
        ::g_vecGameObjects.push_back(pRaider);
    }

    float alongX = -120.0f;
    for (int i = 17; i < g_vecGameObjects.size(); i++)
    {
        g_vecGameObjects.at(i)->position = glm::vec3(alongX, 0.0f, -400.0f);
        alongX += 10.0f;
    }
    

    // End of Vipers creation
    //=========================================================================

	return;
}
