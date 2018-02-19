#include "stdafx.h"
#include "bt_cPhysicsFactory.h"
#include "bt_cRigidBody.h"
#include "bt_shapes.h"
#include "bt_cPhysicsWorld.h"

namespace nPhysics
{
	bt_cPhysicsFactory::~bt_cPhysicsFactory() {}

	iPhysicsWorld* bt_cPhysicsFactory::CreateWorld()
	{
        return new bt_cPhysicsWorld();
	}

	iRigidBody* bt_cPhysicsFactory::CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape)
	{
		return new bt_cRigidBody(desc, shape);
	}

	iShape* bt_cPhysicsFactory::CreateSphere(float radius)
	{
		return new bt_cSphereShape(radius);
	}
	iShape* bt_cPhysicsFactory::CreatePlane(const glm::vec3& normal, float planeConst)
	{
		return new bt_cPlaneShape(normal, planeConst);
	}

    iShape * bt_cPhysicsFactory::CreateCube(float size)
    {
        return new bt_cCubeShape(size);
    }
	
}