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

		virtual void GetTransform(glm::mat4& transformOut) = 0;
		virtual void GetPostion(glm::vec3& positionOut) = 0;
		virtual void GetRotation(glm::vec3& rotationOut) = 0;
        virtual void SetRotation(glm::quat& rotationIn) = 0;
        virtual void GetMatOrientation(glm::mat4& orientationOut) = 0;
        virtual void SetMatOrientation(const glm::mat4& orientationIn) = 0;
        virtual void GetVelocity(glm::vec3& velocity) = 0;
        virtual void SetVelocity(const glm::vec3& velocity) = 0;
        virtual void SetVelocityLocal(const glm::vec3& velocity) = 0;
        virtual void SetRateOfTurnX(float rotX) = 0;
        virtual void SetRateOfTurnY(float rotY) = 0;
        virtual void SetRateOfTurnZ(float rotZ) = 0;
	};
}