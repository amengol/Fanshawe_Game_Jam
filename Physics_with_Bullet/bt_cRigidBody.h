#pragma once
#include <iRigidBody.h>
#include <sRigidBodyDesc.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>
#include <btBulletDynamicsCommon.h>

namespace nPhysics
{
	class bt_cPhysicsWorld;
	class bt_cRigidBody : public iRigidBody
	{
	public:
		bt_cRigidBody(const sRigidBodyDesc& desc, iShape* shape);
		virtual ~bt_cRigidBody();

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
        virtual void SetRateOfTurnX(float rotX) { this->rateOfTurnX = rotX; }
        virtual void SetRateOfTurnY(float rotY) { this->rateOfTurnY = rotY; }
        virtual void SetRateOfTurnZ(float rotZ) { this->rateOfTurnZ = rotZ; }

        inline void setLastPosition(glm::vec3 lastPos) { this->mLastPos = lastPos; }
        inline glm::vec3 getLastPosition() { return this->mLastPos; }

        // Bullet
        inline btRigidBody* getBulletRigidBody() { return this->bullet_RigidBody; }

	private:
		friend class bt_cPhysicsWorld;
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

        // Bullet
        btRigidBody* bullet_RigidBody;
	};
}