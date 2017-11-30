#ifndef _cCameraObject_HG_
#define _cCameraObject_HG_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class cCameraObject
{
public:
    cCameraObject();

    // Set camera and also the unity Lookat position
    void setCameraPosition(glm::vec3 newPosition);

    // Reposition a camera according to a target and reset up vector
    void setCameraTarget(glm::vec3 target);

    void moveCameraBackNForth(float speed);
    void moveCameraLeftNRight(float speed);
    void changeAlongX(float change);
    void changeAlongY(float change);
    void changeAlongZ(float change);
    void setCameraOrientationX(float degrees);
    void setCameraOrientationY(float degrees);
    void setCameraOrientationZ(float degrees);
    void getCameraInfo(glm::vec3 &camPosition, glm::vec3 &lookAtPosition);
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
};

#endif // !_cCameraObject_HG_
