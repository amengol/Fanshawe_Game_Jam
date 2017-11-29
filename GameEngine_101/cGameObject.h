#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
//#include "cDebugRenderer.h"

enum eTypeOfObject
{	
	SPHERE  = 0,
	PLANE   = 1,
	CAPSULE = 2,
	AABB    = 3,		// Axis-aligned box
	UNKNOWN = 99
};

// Sotores vec position, orientation and scale about a Game Object
class cGameObject
{
public:
	cGameObject();
	~cGameObject();
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 orientation2;
	float scale;
	// **********************************
	// Physics properties
	glm::vec3 vel;
	glm::vec3 accel;
	bool bIsUpdatedInPhysics;
	eTypeOfObject typeOfObject;
	float radius;
	bool bIsWireFrame;
	// **********************************
	glm::vec4 diffuseColour;
	std::string meshName;

	// Behaviour
 	void Update(double deltaTime);
 
 	void DebugUpdate(double deltaTime);
 
 private:
 	//cDebugRenderer* m_pTheDR;
};
#endif
