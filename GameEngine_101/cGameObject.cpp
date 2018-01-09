#include "cGameObject.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <fmod\fmod.hpp>
#include "cLightManager.h"

extern cLightManager* g_pLightManager;

cGameObject::cGameObject()
{
	this->scale = 1.0f;
    this->rateOfTurnX = 0.0f;
    this->rateOfTurnY = 0.0f;
    this->rateOfTurnZ = 0.0f;
	this->position = glm::vec3(0.0f);
    this->previousPosition = glm::vec3(0.0f);
	this->orientation = glm::mat4x4(1.0f);
    this->vecOrientation = glm::vec3(0.0f);
	this->vel = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->bIsUpdatedInPhysics = true;
	this->radius = 0.0f;
	this->typeOfObject = eTypeOfObject::UNKNOWN;
	this->bIsWireFrame = false;
    this->mGOSound = NULL;
    this->mHasSound = false;
    this->hasColour = false;
    this->hasAlpha = false;
    this->cullFace = true;
    this->bIsSkyBoxObject = false;
    this->isDebugAABBActive = false;
    this->hasLights = false;

#ifdef _DEBUG
 	//this->m_pTheDR = cDebugRenderer::getInstance();
 #endif

	return;
}

cGameObject::~cGameObject()
{
	return;
}

void cGameObject::DebugUpdate(double deltaTime)
 {
 #ifdef _DEBUG
 	/*this->m_pTheDR->AddLine( this->position, 
 							 this->position + this->vel, 
 							 this->diffuseColour );*/
 #endif
 	return;
 }
 
 void cGameObject::Update(float deltaTime, glm::vec3 GRAVITY)
 {
     // Remember the last position
     this->previousPosition = this->position;

     // Explicit Euler  (RK4)
     // New position is based on velocity over time
     // Velocity is based on local axis
     glm::vec3 newVel = this->orientation * glm::vec4(this->vel, 0.0f);
     glm::vec3 deltaPosition = (float)deltaTime * newVel;
     this->position += deltaPosition;

     // New velocity is based on acceleration over time
     // Acceleration is based on local axis
     glm::vec3 newAccel = this->orientation * glm::vec4(this->accel, 0.0f);
     glm::vec3 deltaVelocity = ((float)deltaTime * newAccel)
         + ((float)deltaTime * GRAVITY);
     this->vel += deltaVelocity;

     //------------------------------------------------------------------------
     // Change orientation according to the Rate Of Turn per minute     
     if (this->rateOfTurnX != 0.0f)
     {
         float theTurnX = (this->rateOfTurnX * deltaTime) / 60.0f;         
         this->orientation = glm::rotate(this->orientation, glm::radians(theTurnX), glm::vec3(1.0f, 0.0f, 0.0f));
     }
     if (this->rateOfTurnY != 0.0f)
     {
         float theTurnY = (this->rateOfTurnY * deltaTime) / 60.0f;
         this->orientation = glm::rotate(this->orientation, glm::radians(theTurnY), glm::vec3(0.0f, 1.0f, 0.0f));
     }
     if (this->rateOfTurnZ != 0.0f)
     {
         float theTurnZ = (this->rateOfTurnZ * deltaTime) / 60.0f;
         this->orientation = glm::rotate(this->orientation, glm::radians(theTurnZ), glm::vec3(0.0f, 0.0f, 1.0f));
     }
     //------------------------------------------------------------------------ 
     // Update sound Objects
     if (this->mHasSound)
         this->mGOSound->setPosition(this->position);
     //------------------------------------------------------------------------ 

     // Update lights
     if(this->hasLights)
     {
         for(int i = 0; i < this->vecLightsInfo.size(); i++)
         {
             lightInfo lightInfo = vecLightsInfo[i];
             glm::vec3 rotatedOffset = this->orientation * glm::vec4(lightInfo.offset, 0.0f);
             g_pLightManager->vecLights[lightInfo.index].position = this->position + rotatedOffset;
             glm::vec3 rotatedFocus = this->orientation * glm::vec4(lightInfo.focusDirection, 0.0f);
             g_pLightManager->vecLights[lightInfo.index].direction = rotatedFocus;
         }
     }
  }

 void cGameObject::rotateX(float degreesX)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesX), glm::vec3(1.0f, 0.0f, 0.0f));
 }

 void cGameObject::rotateY(float degreesY)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesY), glm::vec3(0.0f, 1.0f, 0.0f));
 }

 void cGameObject::rotateZ(float degreesZ)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesZ), glm::vec3(0.0f, 0.0f, 1.0f));
 }

 bool cGameObject::initSoundObject(std::string name)
 {
    if (!this->mHasSound)
    {
        cSoudObject* theSO = new cSoudObject();
        theSO->setFMODType(FMOD_3D);
        theSO->setPosition(this->position);
        theSO->setMovType(2);
        theSO->setfriendlyName(name);
        this->mGOSound = theSO;
        mHasSound = true;
        return true;
    }
    return false;
 }

 bool cGameObject::hasSound()
 {
     return this->mHasSound;
 }

 cSoudObject* cGameObject::getSoundObject()
 {
     if (this->mHasSound)
         return this->mGOSound;
     else
     {
         this->initSoundObject("Generic");
         return this->mGOSound;
     }
     
 }

 std::string cGameObject::getSoundName()
 {
     if (this->mHasSound)
         return this->mGOSound->getFriendlyName();
 }