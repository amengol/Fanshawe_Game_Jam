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
        virtual void SetDebugDrawer(iPhysicsDebugDrawer* debugDrawer);
        virtual void DebugDrawWorld(int shaderID, const glm::mat4& viewMatrix, float zoom, int width, int height);

	private:
		
		std::vector<cRigidBody*> m_vecRigidBody;
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