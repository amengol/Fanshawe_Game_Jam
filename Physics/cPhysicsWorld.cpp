#include "stdafx.h"
#include "cPhysicsWorld.h"
#include <algorithm>
#include "shapes.h"

const glm::vec3 GRAVITY = glm::vec3(0.0f, -10.0f, 0.0f);

namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{

	}

    cPhysicsWorld::RK4_Derivative cPhysicsWorld::evaluate(const RK4_State& initial,
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

    void cPhysicsWorld::integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt)
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

	void cPhysicsWorld::TimeStep(float deltaTime)
	{
        // Maybe faster?
        size_t rbSize = mRigidBody.size();

        for (size_t i = 0; i < rbSize; i++)
        {
            cRigidBody* rb1 = mRigidBody[i];
            iShape* sh1 = rb1->GetShape();

            switch (sh1->GetShapeType())
            {
            case nPhysics::SHAPE_TYPE_PLANE:
                break;
            case nPhysics::SHAPE_TYPE_SPHERE:
            {
                for (size_t j = 0; j < rbSize; j++)
                {
                    cRigidBody* rb2 = mRigidBody[j];
                    cSphereShape* sphere1 = static_cast<cSphereShape*>(sh1);

                    if (rb1 != rb2)
                    {
                        iShape* sh2 = rb2->GetShape();

                        if (sh2->GetShapeType() == SHAPE_TYPE_PLANE)
                        {
                            // SPHERE-PLANE collision
                            cPlaneShape* plane = static_cast<cPlaneShape*>(sh2);

                            switch (plane->getPlaneType())
                            {
                            case nPhysics::cPlaneShape::PlaneType::FLOOR:
                                if (rb1->mPosition.y <= plane->getPlanePosition().y + sphere1->getRadius())
                                {
                                    rb1->mPosition.y = plane->getPlanePosition().y + sphere1->getRadius() + 0.01f;
                                    rb1->mVelocity.y = -rb1->mVelocity.y;
                                    //rb1->mVelocity.y *= 0.85f;
                                }
                                break;
                            case nPhysics::cPlaneShape::PlaneType::FRONT:
                                if (rb1->mPosition.z <= plane->getPlanePosition().z + sphere1->getRadius())
                                {
                                    rb1->mPosition.z = plane->getPlanePosition().z + sphere1->getRadius() + 0.01f;
                                    rb1->mVelocity.z = -rb1->mVelocity.z;
                                    //rb1->mVelocity.z *= 0.85f;
                                }
                                break;
                            case nPhysics::cPlaneShape::PlaneType::BACK:
                                if (rb1->mPosition.z <= plane->getPlanePosition().z - sphere1->getRadius())
                                {
                                    rb1->mPosition.z = plane->getPlanePosition().z - sphere1->getRadius() - 0.01f;
                                    rb1->mVelocity.z = -rb1->mVelocity.z;
                                    //rb1->mVelocity.z *= 0.85f;
                                }
                                break;
                            case nPhysics::cPlaneShape::PlaneType::LEFT:
                                if (rb1->mPosition.x <= plane->getPlanePosition().x + sphere1->getRadius())
                                {
                                    rb1->mPosition.x = plane->getPlanePosition().x + sphere1->getRadius() + 0.01f;
                                    rb1->mVelocity.x = -rb1->mVelocity.x;
                                    //rb1->mVelocity.x *= 0.85f;
                                }
                                break;
                            case nPhysics::cPlaneShape::PlaneType::RIGHT:
                                if (rb1->mPosition.x <= plane->getPlanePosition().x - sphere1->getRadius())
                                {
                                    rb1->mPosition.x = plane->getPlanePosition().x - sphere1->getRadius() - 0.01f;
                                    rb1->mVelocity.x = -rb1->mVelocity.x;
                                    //rb1->mVelocity.x *= 0.85f;
                                }
                                break;
                            default:
                                break;
                            }
                        }//!if (sh2->GetShapeType() == SHAPE_TYPE_PLANE)

                        if (sh2->GetShapeType() == SHAPE_TYPE_SPHERE)
                        {
                            // SPHERE-SPHERE collision
                            cSphereShape* sphere2 = static_cast<cSphereShape*>(sh2);

                            float distance = glm::length(rb1->mPosition - rb2->mPosition);
                            if (distance <= sphere1->getRadius() + sphere2->getRadius())
                            {
                                rb1->mPosition = rb1->mLastPos;
                                rb2->mPosition = rb2->mLastPos;

                                // Collision
                                glm::vec3 fakeNormalS2 = glm::normalize(rb1->mPosition - rb2->mPosition);
                                glm::vec3 reflectedVectorS1 = glm::reflect(rb1->mVelocity, fakeNormalS2);
                                rb1->mVelocity = reflectedVectorS1;
                                glm::vec3 fakeNormalS1 = glm::normalize(rb2->mPosition - rb1->mPosition);
                                glm::vec3 reflectedVectorS2 = glm::reflect(rb2->mVelocity, fakeNormalS1);
                                rb2->mVelocity = reflectedVectorS2;
                            }
                        }
                    }//!if (rb1 != rb2)
                }//!for (size_t j = 0; j < rbSize; j++)
            }//!case nPhysics::SHAPE_TYPE_SPHERE:
                break;
            default:
                break;
            }//!switch (sh1->GetShapeType()

             //RK4
            integrate(rb1->mPosition, rb1->mVelocity, GRAVITY, deltaTime);

        }//!for (size_t i = 0; i < rbSize; i++)
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

}