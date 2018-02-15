#include "stdafx.h"
#include "cRigidBody.h"

namespace nPhysics
{
	cRigidBody::cRigidBody(const sRigidBodyDesc& desc, iShape* shape)
		: mShape(shape)
		, mPosition(desc.Position)
		, mVelocity(desc.Velocity)
		, mMass(desc.Mass)
		, mRotation(desc.Rotation)
		
	{
	}

	cRigidBody::~cRigidBody()
	{

	}

	iShape* cRigidBody::GetShape()
	{
		return mShape;
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4_cast(mRotation);
		transformOut[3][0] = mPosition.x;
		transformOut[3][1] = mPosition.y;
		transformOut[3][2] = mPosition.z;
		transformOut[3][3] = 1.f;
	}
	void cRigidBody::GetPostion(glm::vec3& positionOut)
	{
		positionOut = mPosition;
	}
	void cRigidBody::GetRotation(glm::vec3& rotationOut)
	{
		rotationOut = glm::eulerAngles(mRotation);
	}

    void cRigidBody::GetMatOrientation(glm::mat4& orientationOut)
    {
        orientationOut = glm::toMat4(this->mRotation);
    }

    void cRigidBody::SetMatOrientation(const glm::mat4& orientationIn)
    {
        this->mRotation = glm::toQuat(orientationIn);
    }

    void cRigidBody::SetVelocityLocal(const glm::vec3& velocity)
    {
        glm::mat4 orientation = glm::toMat4(this->mRotation);

        // Local X axis
        glm::vec3 localX = orientation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        localX = localX * velocity.x;
        // Local Z axis
        glm::vec3 localY = orientation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        localY = localY * velocity.y;
        // Local Z axis
        glm::vec3 localZ = orientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        localZ = localZ * velocity.z;

        this->mVelocity = localX + localY + localZ;
    }
}