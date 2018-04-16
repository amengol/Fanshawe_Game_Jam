#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <vector>
#include <iRigidBody.h>
#include "Assimp\sAnimations.h"
#include "AI\cCharacterControl.h"

class cMesh;
class cSoudObject;
class sCollisionTriangle;
class cAnimationState;
class cSkinnedMesh;

enum eTypeOfObject
{
    SPHERE,
    PLANE,
    CAPSULE,
    MESH,
    TERRAIN,
    CONTACT_POINTS,
    SKYBOX,
    SKINNED_MESH,
    CLOTH,
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

struct sCollisionGeometry
{
    std::vector<sCollisionTriangle*> collisionTriangles;
    std::string meshName;
};


// Sotores vec position, orientation and scale about a Game Object
class cGameObject
{
public:
	cGameObject();
	~cGameObject();

    void SetPostiion(glm::vec3 pos);
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
    bool hadAlreadyCollided; // Used to free the object from collision stuck
	eTypeOfObject typeOfObject;
	float radius;
    std::vector<sCollisionGeometry> contacPoints;
    void setCollisionGeometry(cMesh);

	bool bIsWireFrame;
    bool renderable;
    bool hasColour;
    bool hasAlpha;
    bool useDiscardAlpha;
    bool cullFace;
    bool isDebugAABBActive;
    bool hasReflection;
    bool rotateToCamera;
    bool receiveShadow;
    bool selfLight;
    bool hasNormalMap;
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
    bool hasMultiLayerTextures;

    std::string friendlyName;
    inline unsigned int getUniqueID(void) { return this->m_UniqueID; }

	// Behaviour
 	//void Update(float deltaTime, glm::vec3 GRAVITY);
 
 	void DebugUpdate(double deltaTime);

    void rotateX(float degreesX);
    void rotateY(float degreesY);
    void rotateZ(float degreesZ);

    // Sound
    bool initSoundObject(std::string name);
    bool hasSound();
    cSoudObject* getSoundObject();
    std::string getSoundName();
    void setGOSoundPos();

    // Rigid Body
    nPhysics::iRigidBody* rigidBody;

    // Rigid Body from bullet 
    nPhysics::iRigidBody* bt_rigidBody;

    // If NULL, then object ISN'T a skinned mesh
    cSkinnedMesh* pSimpleSkinnedMesh;
    cAnimationState* pAniState;
    glm::vec3 startDisplacement;
    sAnimations animations;
    // When the animation is for a character, we also need this
    eCharacterAnim characterAnim;

    // Will init all animation names in "animations" into the cSkinnedMesh
    bool InitCharacterAnimations(std::string& error);
    bool animationsInitiated;   // Control if the animations were already initiated

    struct sContactSpheres
    {
        sContactSpheres() :
            radius(1.0f),
            position(glm::vec3(0.0f)),
            colour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
            timeToFade(1.8f),
            hasCollided(false)
        { }

        std::string meshName;
        float radius;
        glm::vec3 position;
        glm::vec4 colour;
        bool hasCollided;
        float timeToFade;
        float elapseTime;
        glm::vec3 collisionPosition;
    };

    std::map<std::string, sContactSpheres*> mMapBoneNameTOMeshName;

 private:
     unsigned int m_UniqueID;
     // Used when creating objects
     static unsigned int m_nextUniqueID;

     // Sound properties
     cSoudObject* mGOSound;
     bool mHasSound;
};
#endif
