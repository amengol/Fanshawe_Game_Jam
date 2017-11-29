#include "cGameObject.h"

cGameObject::cGameObject()
{
	this->scale = 1.0f;
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->orientation2 = glm::vec3(0.0f);
	this->vel = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->bIsUpdatedInPhysics = true;
	this->radius = 0.0f;
	this->typeOfObject = eTypeOfObject::UNKNOWN;
	this->bIsWireFrame = false;

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
 
 void cGameObject::Update(double deltaTime)
 {
 	// Put frame update behaviour here...
 	return;
 }