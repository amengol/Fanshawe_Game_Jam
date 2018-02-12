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
		virtual void GetRotation(glm::vec3& rotationOut);
        virtual void GetMatOrientation(glm::mat4& orientationOut) { orientationOut = this->mOrientation; }
        virtual void GetVelocity(glm::vec3& velocity) { velocity = this->mVelocity; }
        virtual void SetVelocity(const glm::vec3& velocity) { this->mVelocity = velocity; }

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

        glm::mat4 mOrientation;
	};
}