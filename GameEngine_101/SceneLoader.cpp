// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"
#include <glm/glm.hpp>

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pSkyBoxObject;

void LoadModelsIntoScene(void)
{
	//{
	//	// Fractal Terrain
	//	cGameObject* theGO = new cGameObject();		
 //       theGO->diffuseColour = glm::vec4( 1.4f, 1.4f, 1.4f, 1.0f );
 //       theGO->meshName = "FractalTerrain";
 //       theGO->bIsUpdatedInPhysics = false;
 //       //theGO->textureBlend[0] = 1.0f;
 //       //theGO->textureNames[0] = "delorean.bmp";
 //       theGO->bIsWireFrame = false;
 //       theGO->position = glm::vec3(0.0f, 0.0f, 0.0f);
 //       ::g_vecGameObjects.push_back(theGO);
	//}

    //{
    //    // Delorean
    //    cGameObject* theGO = new cGameObject();
    //    theGO->meshName = "Delorean";
    //    theGO->bIsUpdatedInPhysics = true;
    //    theGO->bIsWireFrame = false;
    //    theGO->hasColour = true;
    //    theGO->position = glm::vec3(0.0f, 48.0f, 294.0f);
    //    theGO->rotateY(180.0f);
    //    ::g_vecGameObjects.push_back(theGO);
    //}

    
    // Our skybox object
    {
        //cGameObject* pTempGO = new cGameObject();
        ::g_pSkyBoxObject = new cGameObject();
        g_pSkyBoxObject->scale = 100000.0f;
        g_pSkyBoxObject->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_pSkyBoxObject->meshName = "SkyBox";
        g_pSkyBoxObject->bIsSkyBoxObject = true;
        ::g_vecGameObjects.push_back(::g_pSkyBoxObject);		// Fastest way to add
    }

    {
        // Sphere
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Sphere";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = SPHERE;
        theGO->radius = 1.0f;
        theGO->diffuseColour = glm::vec4(1.4f, 0.4f, 0.4f, 1.0f);
        theGO->position = glm::vec3(100.0f, 100.0f, 5.0f);
        theGO->isDebugAABBActive = true;
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Facade Sets
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "FacadeSets";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "FacadeSets01.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "RoofsEtc";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "RoofsEtc.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Asphalt";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "Asphalt_rue.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Concrete";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "trottoir.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Curbs";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "curb.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Ground1";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "herbe1.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "Ground2";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "sol8.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Roof tops and etc
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "StreetPart";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "StreetPart.bmp";
        ::g_vecGameObjects.push_back(theGO);
    }

    {
        // Side Walk Tree
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "SideWalkTree";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->typeOfObject = TERRAIN;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "tree15.bmp";
        theGO->textureBlend[1] = 0.0f;
        theGO->textureNames[1] = "tree15_alpha.bmp";
        theGO->hasAlpha = true;
        theGO->useDiscardAlpha = true;
        theGO->cullFace = false;
        ::g_vecGameObjects.push_back(theGO);
    }

    //{
    //    // Side Walk Tree
    //    cGameObject* theGO = new cGameObject();
    //    theGO->meshName = "Cloud1";
    //    theGO->position = glm::vec3(-337.824f, 150.0f, 147.835f);
    //    theGO->bIsUpdatedInPhysics = false;
    //    theGO->bIsWireFrame = false;
    //    theGO->typeOfObject = TERRAIN;
    //    theGO->textureBlend[0] = 1.0f;
    //    theGO->textureNames[0] = "clouds.bmp";
    //    theGO->textureBlend[1] = 0.0f;
    //    theGO->textureNames[1] = "clouds_alpha.bmp";
    //    theGO->hasAlpha = true;
    //    theGO->useDiscardAlpha = false;
    //    theGO->cullFace = false;
    //    ::g_vecGameObjects.push_back(theGO);
    //}

    {
        // md500
        cGameObject* theGO = new cGameObject();
        theGO->meshName = "md500";
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = "green.bmp";
        theGO->position = glm::vec3(-323.027f, 107.775f, 106.373f);
        theGO->rotateY(180.0f);
        ::g_vecGameObjects.push_back(theGO);
    }

	return;
}
