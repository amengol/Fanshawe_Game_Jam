#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include "iShape.h"

namespace nPhysics
{
	class iRigidBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual iShape* GetShape() = 0;

        virtual void Activate() = 0;
		virtual void GetTransform(glm::mat4& transformOut) = 0;
		virtual void GetPostion(glm::vec3& positionOut) = 0;
        virtual void SetPosition(glm::vec3& positionIn) = 0;
		virtual void GetRotation(glm::vec3& rotationOut) = 0;
        virtual void SetRotation(glm::quat& rotationIn) = 0;
        virtual void GetMatOrientation(glm::mat4& orientationOut) = 0;
        virtual void SetMatOrientation(const glm::mat4& orientationIn) = 0;
        virtual void GetLinearVelocity(glm::vec3& velocity) = 0;
        virtual void SetLinearVelocity(const glm::vec3& velocity) = 0;
        virtual void SetLinearVelocityLocal(const glm::vec3& velocity) = 0;
        virtual void GetAngularVelocity(glm::vec3& velocity) = 0;
        virtual void SetAngularVelocity(const glm::vec3& velocity) = 0;
        virtual void SetHit(bool hit) = 0;
        virtual bool GetHit() = 0;
        virtual void rotateX(float degrees) = 0;
        virtual void rotateY(float degrees) = 0;
        virtual void rotateZ(float degrees) = 0;
        virtual void SetRateOfTurnX(float rotX) = 0;
        virtual void SetRateOfTurnY(float rotY) = 0;
        virtual void SetRateOfTurnZ(float rotZ) = 0;
        virtual bool IsInWorld() = 0;
	};
}