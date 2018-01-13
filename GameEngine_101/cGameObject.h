#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <vector>

enum eTypeOfObject
{	
	SPHERE  = 0,
	PLANE   = 1,
	CAPSULE = 2,
	MESH    = 3,
    TERRAIN = 4,
    CONTACT_POINTS = 5,
    SKYBOX = 6,
	UNKNOWN = 99
};

enum LightType
{
    OMNI,
    SPOT
};

struct lightInfo
{
    int index;
    glm::vec3 offset;
    glm::vec3 focusDirection;
    LightType type;
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
    bool hasColour;
    bool hasAlpha;
    bool useDiscardAlpha;
    bool cullFace;
    bool isDebugAABBActive;
    bool hasReflection;
    bool rotateToCamera;
    glm::vec3 previousPosition;
	// **********************************
    //Lights
    bool hasLights;
    std::vector<lightInfo> vecLightsInfo;
    // **********************************
	glm::vec4 diffuseColour;
    glm::vec4 specular;
	std::string meshName;

    // AI
    bool hasAI;
    glm::vec3 target;
    unsigned int currentTargetIndex;
    std::vector<glm::vec3> wayPointList;

    // The texture of this object
    static const unsigned int NUMTEXTURES = 10;
    std::string textureNames[NUMTEXTURES];
    float textureBlend[NUMTEXTURES];			// 0 - 1

    std::string friendlyName;
    inline unsigned int getUniqueID(void) { return this->m_UniqueID; }

	// Behaviour
 	void Update(float deltaTime, glm::vec3 GRAVITY);
 
 	void DebugUpdate(double deltaTime);

    void rotateX(float degreesX);
    void rotateY(float degreesY);
    void rotateZ(float degreesZ);

 
 private:
     unsigned int m_UniqueID;
     // Used when creating objects
     static unsigned int m_nextUniqueID;
};
#endif
