#include "stdafx.h"
#include "bt_cRigidBody.h"
#include "bt_shapes.h"

namespace nPhysics
{
	bt_cRigidBody::bt_cRigidBody(const sRigidBodyDesc& desc, iShape* shape)
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
        switch (shape->GetShapeType())
        {
        case nPhysics::SHAPE_TYPE_PLANE:
        {
            btDefaultMotionState* groundMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
                                         btVector3(desc.Position.x, 
                                         desc.Position.y, 
                                         desc.Position.z)));

            bt_cPlaneShape* planeShape = dynamic_cast<bt_cPlaneShape*>(shape);

            btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, 
                                                                       groundMotionState, 
                                                                       planeShape->getBulletShape(),
                                                                       btVector3(0, 0, 0));

            this->bullet_RigidBody = new btRigidBody(groundRigidBodyCI);

        }
            break;
        case nPhysics::SHAPE_TYPE_SPHERE:
        {
            btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                         btVector3(desc.Position.x,
                                         desc.Position.y,
                                         desc.Position.z)));

            bt_cSphereShape* sphereShape = dynamic_cast<bt_cSphereShape*>(shape);

            btVector3 fallInertia(0, 0, 0);
            sphereShape->getBulletShape()->calculateLocalInertia(desc.Mass, fallInertia);

            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(desc.Mass, 
                                                                     fallMotionState, 
                                                                     sphereShape->getBulletShape(), 
                                                                     fallInertia);
            
            this->bullet_RigidBody = new btRigidBody(fallRigidBodyCI);
        }
            break;
        case nPhysics::SHAPE_TYPE_CUBE:
            break;
        default:
            break;
        }
	}

	bt_cRigidBody::~bt_cRigidBody()
	{

	}

	iShape* bt_cRigidBody::GetShape()
	{
		return mShape;
	}

	void bt_cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4_cast(mRotation);
		transformOut[3][0] = mPosition.x;
		transformOut[3][1] = mPosition.y;
		transformOut[3][2] = mPosition.z;
		transformOut[3][3] = 1.f;
	}
	void bt_cRigidBody::GetPostion(glm::vec3& positionOut)
	{
        btVector3 btVec = this->bullet_RigidBody->getCenterOfMassPosition();
        positionOut.x = btVec.getX();
        positionOut.y = btVec.getY();
        positionOut.z = btVec.getZ();
	}
    void bt_cRigidBody::SetPosition(glm::vec3& positionIn)
    {    
        btVector3 vec;
        vec.setX(positionIn.x);
        vec.setY(positionIn.y);
        vec.setZ(positionIn.z);

        btTransform trans = this->bullet_RigidBody->getCenterOfMassTransform();
        trans.setOrigin(vec);
        this->bullet_RigidBody->setCenterOfMassTransform(trans);
    }
	void bt_cRigidBody::GetRotation(glm::vec3& rotationOut)
	{
		rotationOut = glm::eulerAngles(mRotation);
	}

    void bt_cRigidBody::GetMatOrientation(glm::mat4& orientationOut)
    {
        btQuaternion btQuat = this->bullet_RigidBody->getOrientation();
        glm::quat glmQuat(btQuat.getW(), btQuat.getX(), btQuat.getY(), btQuat.getZ());
        orientationOut = glm::toMat4(glmQuat);
    }

    void bt_cRigidBody::SetMatOrientation(const glm::mat4& orientationIn)
    {
        glm::quat qOr = glm::toQuat(orientationIn);
        btQuaternion btQuat;
        btQuat.setW(qOr.w);
        btQuat.setX(qOr.x);
        btQuat.setY(qOr.y);
        btQuat.setZ(qOr.z);

        btTransform transf = this->bullet_RigidBody->getCenterOfMassTransform();
        transf.setRotation(btQuat);

        this->bullet_RigidBody->setCenterOfMassTransform(transf);
    }

    void bt_cRigidBody::GetVelocity(glm::vec3& velocityOut)
    {
        btVector3 bt_Vel = this->bullet_RigidBody->getLinearVelocity();
        velocityOut.x = bt_Vel.getX();
        velocityOut.y = bt_Vel.getY();
        velocityOut.z = bt_Vel.getZ();
    }

    void bt_cRigidBody::SetVelocity(const glm::vec3 & velocityIn)
    {
        btVector3 bt_Vel;
        bt_Vel.setX(velocityIn.x);
        bt_Vel.setY(velocityIn.y);
        bt_Vel.setZ(velocityIn.z);
        this->bullet_RigidBody->activate();
        this->bullet_RigidBody->setLinearVelocity(bt_Vel);
    }

    //void bt_cRigidBody::SetMatOrientation(const glm::mat4& orientationIn)
    //{
    //    this->mRotation = glm::toQuat(orientationIn);
    //}

    void bt_cRigidBody::SetVelocityLocal(const glm::vec3& velocity)
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