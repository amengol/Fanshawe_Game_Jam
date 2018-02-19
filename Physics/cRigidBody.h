#pragma once
#include <iRigidBody.h>
#include <sRigidBodyDesc.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>

namespace nPhysics
{
	class cPhysicsWorld;
	class cRigidBody : public iRigidBody
	{
	public:
		cRigidBody(const sRigidBodyDesc& desc, iShape* shape);
		virtual ~cRigidBody();

		virtual iShape* GetShape();

		virtual void GetTransform(glm::mat4& transformOut);
		virtual void GetPostion(glm::vec3& positionOut);
        virtual void SetPosition(glm::vec3& positionIn) { this->mPosition = positionIn; }
		virtual void GetRotation(glm::vec3& rotationOut);
        virtual void SetRotation(glm::quat& rotationIn) { this->mRotation = rotationIn; }
        virtual void GetMatOrientation(glm::mat4& orientationOut) { orientationOut = this->mOrientation; }
        virtual void SetMatOrientation(const glm::mat4& orientationIn) { this->mOrientation = orientationIn; }
        virtual void GetVelocity(glm::vec3& velocity) { velocity = this->mVelocity; }
        virtual void SetVelocity(const glm::vec3& velocity) { this->mVelocity = velocity; }
        virtual void SetVelocityLocal(const glm::vec3& velocity);
        virtual void SetRateOfTurnX(float rotX) { this->rateOfTurnX = rotX; }
        virtual void SetRateOfTurnY(float rotY) { this->rateOfTurnY = rotY; }
        virtual void SetRateOfTurnZ(float rotZ) { this->rateOfTurnZ = rotZ; }

        inline void setLastPosition(glm::vec3 lastPos) { this->mLastPos = lastPos; }
        inline glm::vec3 getLastPosition() { return this->mLastPos; }

	private:
		friend class cPhysicsWorld;
		iShape* mShape;
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::quat mRotation;
		float mMass;
        glm::vec3 mLastPos;

        float rateOfTurnX;
        float rateOfTurnY;
        float rateOfTurnZ;

        glm::mat4 mOrientation;
	};
}