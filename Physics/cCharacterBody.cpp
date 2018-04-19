#include "stdafx.h"
#include "cCharacterBody.h"
#include <glm\gtx\transform.hpp>

namespace nPhysics
{
    cCharacterBody::cCharacterBody(const sRigidBodyDesc& desc, iShape* shape)
		: mShape(shape)
		, mPosition(desc.Position)
		, mVelocity(desc.Velocity)
		, mMass(desc.Mass)
		, mRotation(desc.Rotation)
        , rateOfTurnX(0.0f)
        , rateOfTurnY(0.0f)
        , rateOfTurnZ(0.0f)
        , mOrientation(1.0f)
	{
	}

    cCharacterBody::~cCharacterBody()
	{

	}

	iShape* cCharacterBody::GetShape()
	{
		return mShape;
	}

	void cCharacterBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4_cast(mRotation);
		transformOut[3][0] = mPosition.x;
		transformOut[3][1] = mPosition.y;
		transformOut[3][2] = mPosition.z;
		transformOut[3][3] = 1.f;
	}
	void cCharacterBody::GetPostion(glm::vec3& positionOut)
	{
		positionOut = mPosition;
	}
	void cCharacterBody::GetRotation(glm::vec3& rotationOut)
	{
		rotationOut = glm::eulerAngles(mRotation);
	}

    //void cCharacterBody::GetMatOrientation(glm::mat4& orientationOut)
    //{
    //    orientationOut = glm::toMat4(this->mRotation);
    //}

    //void cCharacterBody::SetMatOrientation(const glm::mat4& orientationIn)
    //{
    //    this->mRotation = glm::toQuat(orientationIn);
    //}

    void cCharacterBody::SetLinearVelocityLocal(const glm::vec3& velocity)
    {
        glm::mat4 orientation = this->mOrientation;

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

    void cCharacterBody::rotateX(float degrees)
    {
        this->mOrientation = glm::rotate(this->mOrientation, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void cCharacterBody::rotateY(float degrees)
    {
        this->mOrientation = glm::rotate(this->mOrientation, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void cCharacterBody::rotateZ(float degrees)
    {
        this->mOrientation = glm::rotate(this->mOrientation, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
    }
}