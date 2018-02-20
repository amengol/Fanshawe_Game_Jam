#include "stdafx.h"
#include "bt_cPhysicsWorld.h"
#include <algorithm>
#include "bt_shapes.h"
#include <glm\gtx\transform.hpp>

namespace nPhysics
{
    bt_cPhysicsWorld::bt_cPhysicsWorld()
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
    }

    bt_cPhysicsWorld::~bt_cPhysicsWorld()
	{
        delete this->dynamicsWorld;
        delete this->solver;
        delete this->dispatcher;
        delete this->collisionConfiguration;
        delete this->broadphase;
	}

    bt_cPhysicsWorld::RK4_Derivative bt_cPhysicsWorld::evaluate(const RK4_State& initial,
                                                          float dt,
                                                          const RK4_Derivative& d)
    {
        RK4_State state;
        state.x = initial.x + d.dx*dt;
        state.v = initial.v + d.dv*dt;

        RK4_Derivative output;
        output.dx = state.v;
        //output.dv = acceleration(state, t + dt);
        return output;
    }

    void bt_cPhysicsWorld::integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt)
    {
        {
            // Put the acceleration into the velocity
            glm::vec3 newVel = vel + accel * dt;

            RK4_State state;
            state.x = pos;
            state.v = newVel;

            RK4_Derivative a, b, c, d;

            a = evaluate(state, 0.0f, RK4_Derivative());
            b = evaluate(state, dt*0.5f, a);
            c = evaluate(state, dt*0.5f, b);
            d = evaluate(state, dt, c);

            glm::vec3 dxdt = 1.0f / 6.0f *
                (a.dx + 2.0f * (b.dx + c.dx) + d.dx);

            glm::vec3 dvdt = 1.0f / 6.0f *
                (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

            pos = state.x + dxdt * dt;
            vel = state.v + dvdt * dt;
        }
    }

	void bt_cPhysicsWorld::TimeStep(float deltaTime)
	{
        this->dynamicsWorld->stepSimulation(deltaTime);
	}

	void bt_cPhysicsWorld::AddRigidBody(iRigidBody* rigidBody)
	{
		bt_cRigidBody* rb = dynamic_cast<bt_cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<bt_cRigidBody*>::iterator itRigidBody;
		itRigidBody = std::find(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody == mRigidBody.end())
		{
			mRigidBody.push_back(rb);
		}

        // Bullet
        bt_cRigidBody* theRB = static_cast<bt_cRigidBody*>(rigidBody);
        this->dynamicsWorld->addRigidBody(theRB->getBulletRigidBody());
       
	}

	void bt_cPhysicsWorld::RemoveRigidBody(iRigidBody* rigidBody)
	{
		bt_cRigidBody* rb = dynamic_cast<bt_cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<bt_cRigidBody*>::iterator itRigidBody;
		
		itRigidBody = std::remove(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody != mRigidBody.end())
		{
			mRigidBody.resize(mRigidBody.size() - 1);
		}
	}

}