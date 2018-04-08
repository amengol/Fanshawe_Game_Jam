#pragma once
#include <iPhysicsWorld.h>
#include <vector>
#include "cRigidBody.h"
#include <btBulletDynamicsCommon.h> // Bullet

namespace nPhysics
{

	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
        cPhysicsWorld();
		virtual ~cPhysicsWorld();

		virtual void TimeStep(float deltaTime);

		virtual void AddRigidBody(iRigidBody* rigidBody);
		virtual void RemoveRigidBody(iRigidBody* rigidBody);

        virtual void AddConstraint(ContraintType type, iConstraint* constraint);

	private:
		
		std::vector<cRigidBody*> mRigidBody;
        float timeToFadeCollision;  // Max time with collision ON
        float elapsedTime;          // Elapsed system time

        // Bullet--------------------------------------------------------------
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
	};
}