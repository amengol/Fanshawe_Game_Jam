#pragma once
#include <iPhysicsFactory.h>

namespace nPhysics
{
	class bt_cPhysicsFactory : public iPhysicsFactory
	{
	public:
		virtual ~bt_cPhysicsFactory();

		virtual iPhysicsWorld* CreateWorld();

		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape);

		virtual iShape* CreateSphere(float radius);
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst);
        virtual iShape* CreateCube(float size);
	};
}