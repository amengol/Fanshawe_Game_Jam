#include "stdafx.h"
#include "cRigidBody.h"
#include "shapes.h"

namespace nPhysics
{
	cRigidBody::cRigidBody(const sRigidBodyDesc& desc, iShape* shape)
		: mShape(shape)
		, mPosition(desc.Position)
		, mVelocity(desc.Velocity)
		, mMass(desc.Mass)
		, mRotation(desc.Rotation)
        , mOrientation(1.0f)
        , mHit(false)
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
            groundRigidBodyCI.m_restitution = 0.9;

            this->bullet_RigidBody = new btRigidBody(groundRigidBodyCI);

            this->bullet_RigidBody->setCollisionFlags(this->bullet_RigidBody->getCollisionFlags() |
                                                      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

            this->bullet_RigidBody->setUserPointer(this);
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

            // Sphere mass is a factor of its radius
            btScalar sMass;
            shape->GetSphereRadius(sMass);

            btVector3 fallInertia(0, 0, 0);
            sphereShape->getBulletShape()->calculateLocalInertia(sMass, fallInertia);

            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(sMass,
                                                                     fallMotionState, 
                                                                     sphereShape->getBulletShape(), 
                                                                     fallInertia);
            fallRigidBodyCI.m_restitution = 0.95;

            this->bullet_RigidBody = new btRigidBody(fallRigidBodyCI);

            this->bullet_RigidBody->setCollisionFlags(this->bullet_RigidBody->getCollisionFlags() |
                                                      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

            this->bullet_RigidBody->setUserPointer(this);
        }
            break;
        case nPhysics::SHAPE_TYPE_BOX:
        {

            bt_cBoxShape* boxShape = dynamic_cast<bt_cBoxShape*>(shape);

            // Create Dynamic Objects
            btTransform startTransform;
            startTransform.setIdentity();

            btScalar mass = desc.Mass;

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0, 0, 0);
            if (isDynamic)
                boxShape->getBulletShape()->calculateLocalInertia(mass, localInertia);

            btVector3 boxPosition;
            boxPosition.setX(desc.Position.x);
            boxPosition.setY(desc.Position.y);
            boxPosition.setZ(desc.Position.z);

            startTransform.setOrigin(boxPosition);
            
            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

            btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, boxShape->getBulletShape(), localInertia);

            cInfo.m_restitution = 0.4;

            this->bullet_RigidBody = new btRigidBody(cInfo);

            this->bullet_RigidBody->setCollisionFlags(this->bullet_RigidBody->getCollisionFlags() |
                                                      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

            this->bullet_RigidBody->setUserPointer(this);
        }
        break;
        case SHAPE_TYPE_CONVEX_HULL:
        {
            bt_cConvexHullShape* convexHull = dynamic_cast<bt_cConvexHullShape*>(shape);

            btConvexHullShape* btShape = static_cast<btConvexHullShape*>(convexHull->getBulletShape());

            btShape->optimizeConvexHull();

            btVector3 localScaling(desc.Scale, desc.Scale, desc.Scale);
            btShape->setLocalScaling(localScaling);

            // Create Dynamic Objects
            btTransform startTransform;
            startTransform.setIdentity();

            btScalar mass = desc.Mass;

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0, 0, 0);
            if (isDynamic)
                btShape->calculateLocalInertia(mass, localInertia);

            btVector3 position;
            position.setX(desc.Position.x);
            position.setY(desc.Position.y);
            position.setZ(desc.Position.z);

            startTransform.setOrigin(position);

            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

            btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, btShape, localInertia);


            this->bullet_RigidBody = new btRigidBody(cInfo);

            this->bullet_RigidBody->setCollisionFlags(this->bullet_RigidBody->getCollisionFlags() |
                                                      btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

            this->bullet_RigidBody->setUserPointer(this);
        }
            break;
        default:
            break;
        }
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
        btVector3 btVec = this->bullet_RigidBody->getCenterOfMassPosition();
        positionOut.x = btVec.getX();
        positionOut.y = btVec.getY();
        positionOut.z = btVec.getZ();
	}
    void cRigidBody::SetPosition(glm::vec3& positionIn)
    {    
        btVector3 vec;
        vec.setX(positionIn.x);
        vec.setY(positionIn.y);
        vec.setZ(positionIn.z);

        btTransform trans = this->bullet_RigidBody->getCenterOfMassTransform();
        trans.setOrigin(vec);
        this->bullet_RigidBody->setCenterOfMassTransform(trans);
    }
	void cRigidBody::GetRotation(glm::vec3& rotationOut)
	{
		rotationOut = glm::eulerAngles(mRotation);
	}

    void cRigidBody::GetMatOrientation(glm::mat4& orientationOut)
    {
        btQuaternion btQuat = this->bullet_RigidBody->getOrientation();
        glm::quat glmQuat(btQuat.getW(), btQuat.getX(), btQuat.getY(), btQuat.getZ());
        orientationOut = glm::toMat4(glmQuat);
    }

    void cRigidBody::SetMatOrientation(const glm::mat4& orientationIn)
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

    void cRigidBody::GetVelocity(glm::vec3& velocityOut)
    {
        btVector3 bt_Vel = this->bullet_RigidBody->getLinearVelocity();
        velocityOut.x = bt_Vel.getX();
        velocityOut.y = bt_Vel.getY();
        velocityOut.z = bt_Vel.getZ();
    }

    void cRigidBody::SetVelocity(const glm::vec3 & velocityIn)
    {
        btVector3 bt_Vel;
        bt_Vel.setX(velocityIn.x);
        bt_Vel.setY(velocityIn.y);
        bt_Vel.setZ(velocityIn.z);
        this->bullet_RigidBody->activate();
        this->bullet_RigidBody->setLinearVelocity(bt_Vel);
    }

    //void cRigidBody::SetMatOrientation(const glm::mat4& orientationIn)
    //{
    //    this->mRotation = glm::toQuat(orientationIn);
    //}

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