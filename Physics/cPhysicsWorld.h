#pragma once
#include <iPhysicsWorld.h>
#include <vector>
#include "cRigidBody.h"
#include <btBulletDynamicsCommon.h> // Bullet
#include "cCharacterBody.h"

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
        std::vector<cCharacterBody*> m_vecCharacterBody;
        float timeToFadeCollision;  // Max time with collision ON
        float elapsedTime;          // Elapsed system time

        // Bullet--------------------------------------------------------------
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;

        struct RK4_State
        {
            glm::vec3 x;      // position
            glm::vec3 v;      // velocity
        };

        struct RK4_Derivative
        {
            RK4_Derivative() :
                dx(0.0f), dv(0.0f)
            {
            }
            glm::vec3 dx;      // dx/dt = velocity
            glm::vec3 dv;      // dv/dt = acceleration
        };

        RK4_Derivative evaluate(const RK4_State& initial,
                                float dt,
                                const RK4_Derivative& d);

        void integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt);
	};
}