#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class iCollisionBody
{
public:
    virtual ~iCollisionBody() { }

    virtual void GetTransform(glm::mat4& transformOut) = 0;
    virtual void GetPostion(glm::vec3& positionOut) = 0;
    virtual void GetRotation(glm::vec3& rotationOut) = 0;
    virtual void GetMatOrientation(glm::mat4& orientationOut) = 0;
    virtual void GetVelocity(glm::vec3& velocity) = 0;
    virtual void SetPosition(glm::vec3& positionIn) = 0;
    virtual void SetRotation(glm::quat& rotationIn) = 0;
    virtual void SetMatOrientation(const glm::mat4& orientationIn) = 0;
    virtual void SetVelocity(const glm::vec3& velocity) = 0;
    virtual void SetVelocityLocal(const glm::vec3& velocity) = 0;
    virtual void rotateX(float degrees) = 0;
    virtual void rotateY(float degrees) = 0;
    virtual void rotateZ(float degrees) = 0;
};

