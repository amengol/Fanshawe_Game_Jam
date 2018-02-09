#pragma once
#include <Physics\iPhysicsWorld.h>
#include <vector>
#include "cRigidBody.h"

namespace nPhysics
{
	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
		virtual ~cPhysicsWorld();

		virtual void TimeStep(float deltaTime);

		virtual void AddRigidBody(iRigidBody* rigidBody);
		virtual void RemoveRigidBody(iRigidBody* rigidBody);

	private:
		
		std::vector<cRigidBody*> mRigidBody;
	};
}