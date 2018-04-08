#pragma once
#include <iRigidBody.h>
#include <sRigidBodyDesc.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>
#include <btBulletDynamicsCommon.h>

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
        virtual void SetPosition(glm::vec3& positionIn);
		virtual void GetRotation(glm::vec3& rotationOut);
        virtual void SetRotation(glm::quat& rotationIn) { this->mRotation = rotationIn; }
        virtual void GetMatOrientation(glm::mat4& orientationOut);
        virtual void SetMatOrientation(const glm::mat4& orientationIn);
        virtual void GetVelocity(glm::vec3& velocityOut);
        virtual void SetVelocity(const glm::vec3& velocityIn);
        virtual void SetVelocityLocal(const glm::vec3& velocity);

        inline void setLastPosition(glm::vec3 lastPos) { this->mLastPos = lastPos; }
        inline glm::vec3 getLastPosition() { return this->mLastPos; }

        // Bullet
        inline btRigidBody* getBulletRigidBody() { return this->bullet_RigidBody; }

        virtual void SetHit(bool hit) { this->mHit = hit; }
        virtual bool GetHit() { return this->mHit; }

	private:
        friend class cPhysicsWorld;
		iShape* mShape;
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::quat mRotation;
		float mMass;
        glm::vec3 mLastPos;

        glm::mat4 mOrientation;

        bool mHit;   // It wiil flag whether the RigidBody was hit or not

        // Bullet
        btRigidBody* bullet_RigidBody;
	};
}