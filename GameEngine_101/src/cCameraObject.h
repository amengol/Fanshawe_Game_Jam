#ifndef _cCameraObject_HG_
#define _cCameraObject_HG_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class cGameObject;

enum eCameraMode {
    MANUAL,			// Move along the axes (lookat)
    FOLLOW_CAMERA,	// Follows a target (lookat)
    CONTROL_CAMERA,
    CONTROL_CAMERA_LOCK,
    FLY_CAMERA,		// Movement based on direction of gaze                    
    STADIUM_CAMERA,
    CHARACTER_CAMERA,
    STADIUM_CHARACTER_CONTROL
};

class cCameraObject
{
public:
    cCameraObject();

    // Set camera and also the unity Lookat position
    void setCameraPosition(glm::vec3 newPosition);

    // Reposition a camera according to a target and reset up vector
    void setCameraTarget(glm::vec3 target);

    // Let the camera be a follow_camera and controls a gameObject
    void lockOnGameObject(cGameObject*, bool control = true);

    // Let the camera be a character_camera and controls a gameObject
    void lockOnCharacter(cGameObject*, bool control = true);

    // Return the Game Object
    cGameObject* getGameObject();

    // Releases the camera from controlling a gameObject
    void releaseGameObject();

    // Updates the camera
    void update();

    void moveCameraBackNForth(float speed);
    void moveCameraBackNForth_Stadium(float speed);
    void moveCameraLeftNRight(float speed);
    void moveCameraLeftNRight_Stadium(float speed);
    void moveCameraUpNDown(float speed);
    void changeAlongX(float change);
    void changeAlongY(float change);
    void changeAlongZ(float change);
    void setCameraOrientationX(float degrees);
    void setCameraOrientationX_Stadium(float degrees);
    void setCameraOrientationY(float degrees);
    void setCameraOrientationY_Stadium(float degrees);
    void setCameraOrientationZ(float degrees);
    void setCameraMode(eCameraMode);
    void getCameraInfo(glm::vec3 &camPosition, glm::vec3 &lookAtPosition);
    void resetCamera();
    eCameraMode getCameraMode();
    glm::vec3 getCameraPosition();
    glm::vec3 getLookAtPosition();
    glm::vec3 getCameraUpVector();
    void setVelocity(float newVelocity);
    glm::vec3 getVelocity();
    glm::mat4x4 getCameraOrientation() { return this->camOrientation; }

    glm::vec3 camLastPos;
    float mFOV; // Field Of View


private:
    glm::vec3 camPosition;
    glm::vec3 lookAtPosition;
    glm::vec3 camUpVector;
    glm::mat4x4 camOrientation;
    float camVelocity;
    eCameraMode cameraMode;
    cGameObject* controlledGameObject;
    cGameObject* controlledCharacter;
};

#endif // !_cCameraObject_HG_
