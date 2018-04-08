#include "stdafx.h"
#include "cPhysicsWorld.h"
#include <algorithm>
#include "shapes.h"
#include <glm\gtx\transform.hpp>
#include "cPoint2PointConstraint.h"
#include "cHingeConstraint.h"
#include "cFixedConstraint.h"
#include "cUniversalConstraint.h"


extern ContactAddedCallback	gContactAddedCallback;

bool callbackFunc(btManifoldPoint& cp, 
                  const btCollisionObjectWrapper* colObj0Wrap, 
                  int partId0, 
                  int index0, 
                  const btCollisionObjectWrapper* colObj1Wrap, 
                  int partId1, int index1)
{
    ((nPhysics::cRigidBody*)colObj0Wrap->getCollisionObject()->getUserPointer())->SetHit(true);

    ((nPhysics::cRigidBody*)colObj1Wrap->getCollisionObject()->getUserPointer())->SetHit(true);
    return false;
}

namespace nPhysics
{

    cPhysicsWorld::cPhysicsWorld()
    {
        // Build the broadphase
        this->broadphase = new btDbvtBroadphase();

        // Set up the collision configuration and dispatcher
        this->collisionConfiguration = new btDefaultCollisionConfiguration();
        this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

        // The actual physics solver
        this->solver = new btSequentialImpulseConstraintSolver;

        // The world.
        this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        this->dynamicsWorld->setGravity(btVector3(0, -10, 0));

        // Callback
        gContactAddedCallback = callbackFunc;

        this->timeToFadeCollision = 1.0f;
        this->elapsedTime = 0.0f;
    }

    cPhysicsWorld::~cPhysicsWorld()
	{
        delete this->dynamicsWorld;
        delete this->solver;
        delete this->dispatcher;
        delete this->collisionConfiguration;
        delete this->broadphase;
	}

	void cPhysicsWorld::TimeStep(float deltaTime)
	{
        this->elapsedTime += deltaTime;

        if (this->elapsedTime > this->timeToFadeCollision)
        {
            this->elapsedTime = 0.0f;
            for (size_t i = 0; i < mRigidBody.size(); i++)
            {
                mRigidBody[i]->SetHit(false);
            }
        }

        this->dynamicsWorld->stepSimulation(deltaTime, 3);
	}

	void cPhysicsWorld::AddRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;
		itRigidBody = std::find(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody == mRigidBody.end())
		{
			mRigidBody.push_back(rb);
		}

        // Bullet
        cRigidBody* theRB = static_cast<cRigidBody*>(rigidBody);
        this->dynamicsWorld->addRigidBody(theRB->getBulletRigidBody());
       
	}

	void cPhysicsWorld::RemoveRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;
		
		itRigidBody = std::remove(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody != mRigidBody.end())
		{
			mRigidBody.resize(mRigidBody.size() - 1);
		}
	}

    void cPhysicsWorld::AddConstraint(ContraintType type, iConstraint* constraint)
    {
        switch (type)
        {
        case nPhysics::POINT2POINT:
        {
            btPoint2PointConstraint* btConstraint = 
                (dynamic_cast<cPoint2PointConstraint*>(constraint))->GetBulletConstraint();

            this->dynamicsWorld->addConstraint(btConstraint);
        }
            break;
        case nPhysics::HINGE:
        {
            btHingeConstraint* btConstraint =
                (dynamic_cast<cHingeConstraint*>(constraint))->GetBulletConstraint();

            this->dynamicsWorld->addConstraint(btConstraint);
        }
            break;
        case nPhysics::FIXED:
        {
            btFixedConstraint* btConstraint =
                (dynamic_cast<cFixedConstraint*>(constraint))->GetBulletConstraint();

            this->dynamicsWorld->addConstraint(btConstraint);
        }
            break;
        case nPhysics::UNIVERSAL:
        {
            btUniversalConstraint* btConstraint =
                (dynamic_cast<cUniversalConstraint*>(constraint))->GetBulletConstraint();

            this->dynamicsWorld->addConstraint(btConstraint);
        }
            break;
        default:
            break;
        }
    }

}