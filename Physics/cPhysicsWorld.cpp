#include "stdafx.h"
#include "cPhysicsWorld.h"
#include <algorithm>
#include "shapes.h"
#include <glm\gtx\transform.hpp>
#include "cPoint2PointConstraint.h"
#include "cHingeConstraint.h"
#include "cFixedConstraint.h"
#include "cUniversalConstraint.h"
#include "cPhysicsDebugDrawer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


extern ContactAddedCallback	gContactAddedCallback;
const glm::vec3 GRAVITY = glm::vec3(0.0f, -10.0f, 0.0f);

bool callbackFunc(btManifoldPoint& cp, 
                  const btCollisionObjectWrapper* colObj0Wrap, 
                  int partId0, 
                  int index0, 
                  const btCollisionObjectWrapper* colObj1Wrap, 
                  int partId1, int index1)
{
    if (colObj0Wrap->getCollisionObject()->getCollisionShape()->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
    {
        if (colObj1Wrap->getCollisionObject()->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
        {
            btRigidBody* rb = (btRigidBody*)colObj0Wrap->getCollisionObject();
            btVector3 velocity = rb->getLinearVelocity();
            velocity.setY(0.0f);
            rb->setLinearVelocity(velocity);
        }
    }

    if (colObj1Wrap->getCollisionObject()->getCollisionShape()->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
    {
        if (colObj0Wrap->getCollisionObject()->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
        {
            btRigidBody* rb = (btRigidBody*)colObj1Wrap->getCollisionObject();
            btVector3 velocity = rb->getLinearVelocity();
            velocity.setY(0.0f);
            rb->setLinearVelocity(velocity);
        }
    }

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
        btVector3 btGrabity = btVector3(GRAVITY.x, GRAVITY.y, GRAVITY.z);
        this->dynamicsWorld->setGravity(btGrabity);

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
            for (size_t i = 0; i < m_vecRigidBody.size(); i++)
            {
                m_vecRigidBody[i]->SetHit(false);
            }
        }

        this->dynamicsWorld->stepSimulation(deltaTime, 3);
	}

	void cPhysicsWorld::AddRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (rb)
		{
            std::vector<cRigidBody*>::iterator itRigidBody;
            itRigidBody = std::find(m_vecRigidBody.begin(), m_vecRigidBody.end(), rb);
            if (itRigidBody == m_vecRigidBody.end())
            {
                m_vecRigidBody.push_back(rb);
            }


            // Bullet
            cRigidBody* theRB = static_cast<cRigidBody*>(rigidBody);
            this->dynamicsWorld->addRigidBody(theRB->getBulletRigidBody());
            
            rb->mIsInWorld = true;
        }
	}

	void cPhysicsWorld::RemoveRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}

        // Bullet
        this->dynamicsWorld->removeRigidBody(rb->getBulletRigidBody());

		std::vector<cRigidBody*>::iterator itRigidBody;
		
		itRigidBody = std::remove(m_vecRigidBody.begin(), m_vecRigidBody.end(), rb);
		if (itRigidBody != m_vecRigidBody.end())
		{
            m_vecRigidBody.resize(m_vecRigidBody.size() - 1);
		}

        rb->mIsInWorld = false;
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

    void cPhysicsWorld::SetDebugDrawer(iPhysicsDebugDrawer* debugDrawer)
    {
        cPhysicsDebugDrawer* dd = dynamic_cast<cPhysicsDebugDrawer*>(debugDrawer);
        if (!dd)
        {
            return;
        }

        dynamicsWorld->setDebugDrawer(dd->getBulletDebugDrawer());
    }

    void cPhysicsWorld::DebugDrawWorld(int shaderID, const glm::mat4& viewMatrix, float zoom, int width, int height)
    {
        // Projection and view don't change per scene (maybe)
        glm::mat4x4 matProjection = glm::perspective(glm::radians(zoom),
            (float)width / (float)height, 0.1f, 500.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderID, "mView"), 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "mProjection"), 1, GL_FALSE, &matProjection[0][0]);

        dynamicsWorld->debugDrawWorld();
    }
}