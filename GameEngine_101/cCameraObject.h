#ifndef _cCameraObject_HG_
#define _cCameraObject_HG_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class cGameObject;

enum eCameraMode {
    MANUAL,			// Move along the axes (lookat)
    FOLLOW_CAMERA,	// Follows a target (lookat)
    CONTROL_CAMERA,
    FLY_CAMERA		// Movement based on direction of gaze                    
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

    // Return the Game Object
    cGameObject* getGameObject();

    // Releases the camera from controlling a gameObject
    void releaseGameObject();

    // Updates the camera
    void update();

    void moveCameraBackNForth(float speed);
    void moveCameraLeftNRight(float speed);
    void moveCameraUpNDown(float speed);
    void changeAlongX(float change);
    void changeAlongY(float change);
    void changeAlongZ(float change);
    void setCameraOrientationX(float degrees);
    void setCameraOrientationY(float degrees);
    void setCameraOrientationZ(float degrees);
    void setCameraMode(eCameraMode);
    void getCameraInfo(glm::vec3 &camPosition, glm::vec3 &lookAtPosition);
    eCameraMode getCameraMode();
    glm::vec3 getCameraPosition();
    glm::vec3 getLookAtPosition();
    glm::vec3 getCameraUpVector();
    void setVelocity(float newVelocity);
    glm::vec3 getVelocity();

private:
    glm::vec3 camPosition;
    glm::vec3 lookAtPosition;
    glm::vec3 camUpVector;
    glm::mat4x4 camOrientation;
    float camVelocity;
    eCameraMode cameraMode;
    cGameObject* controlledGameObject;
};

#endif // !_cCameraObject_HG_
