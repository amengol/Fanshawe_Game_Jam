#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include "cSoundObject.h"
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
    glm::mat4x4 orientation;
    glm::vec3 vecOrientation;
	float scale;
    float rateOfTurnX;
    float rateOfTurnY;
    float rateOfTurnZ;
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

    std::string friendlyName;
    inline unsigned int getUniqueID(void) { return this->m_UniqueID; }

	// Behaviour
 	void Update(float deltaTime, glm::vec3 GRAVITY);
 
 	void DebugUpdate(double deltaTime);

    void rotateX(float degreesX);
    void rotateY(float degreesY);
    void rotateZ(float degreesZ);

    bool initSoundObject(std::string name);
    bool hasSound();
    cSoudObject* getSoundObject();
    std::string getSoundName();
 
 private:
     unsigned int m_UniqueID;
     // Used when creating objects
     static unsigned int m_nextUniqueID;

 	//cDebugRenderer* m_pTheDR;
    // **********************************
    // Sound properties
     cSoudObject* mGOSound;
     bool mHasSound;
     // **********************************
};
#endif
