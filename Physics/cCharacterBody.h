#pragma once
#include <iRigidBody.h>
#include <sRigidBodyDesc.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>

namespace nPhysics
{
	class cPhysicsWorld;
	class cCharacterBody : public iRigidBody
	{
	public:
        cCharacterBody(const sRigidBodyDesc& desc, iShape* shape);
		virtual ~cCharacterBody();

		virtual iShape* GetShape();

		virtual void GetTransform(glm::mat4& transformOut);
		virtual void GetPostion(glm::vec3& positionOut);
        virtual void SetPosition(glm::vec3& positionIn) { this->mPosition = positionIn; }
		virtual void GetRotation(glm::vec3& rotationOut);
        virtual void SetRotation(glm::quat& rotationIn) { this->mRotation = rotationIn; }
        virtual void GetMatOrientation(glm::mat4& orientationOut) { orientationOut = this->mOrientation; }
        virtual void SetMatOrientation(const glm::mat4& orientationIn) { this->mOrientation = orientationIn; }
        virtual void GetLinearVelocity(glm::vec3& velocity) { velocity = this->mVelocity; }
        virtual void SetLinearVelocity(const glm::vec3& velocity) { this->mVelocity = velocity; }
        virtual void SetLinearVelocityLocal(const glm::vec3& velocity);
        virtual void GetAngularVelocity(glm::vec3& velocity) { return; }
        virtual void SetAngularVelocity(const glm::vec3& velocity) { return; }
        virtual void SetHit(bool hit) { return; }
        virtual bool GetHit() { return false; }
        virtual void rotateX(float degrees);
        virtual void rotateY(float degrees);
        virtual void rotateZ(float degrees);
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