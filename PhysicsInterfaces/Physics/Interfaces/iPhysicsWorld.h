#pragma once
#include "iRigidBody.h"
#include "iConstraint.h"

namespace nPhysics
{
    class iPhysicsDebugDrawer;
    enum ContraintType
    {
        POINT2POINT,
        HINGE,
        FIXED,
        UNIVERSAL
    };

	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void TimeStep(float deltaTime) = 0;

		virtual void AddRigidBody(iRigidBody* rigidBody) = 0;
		virtual void RemoveRigidBody(iRigidBody* rigidBody) = 0;
        virtual void AddConstraint(ContraintType type, iConstraint* constraint) = 0;
        virtual void SetDebugDrawer(iPhysicsDebugDrawer* debugDrawer) = 0;
        virtual void DebugDrawWorld() = 0;
	};
}