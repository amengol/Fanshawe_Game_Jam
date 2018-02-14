#pragma once
#include <iPhysicsFactory.h>

namespace nPhysics
{
	class cPhysicsFactory : public iPhysicsFactory
	{
	public:
		virtual ~cPhysicsFactory();

		virtual iPhysicsWorld* CreateWorld();

		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape);

		virtual iShape* CreateSphere(float radius);
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst);
        virtual iShape* CreateCube(float size);
	};
}