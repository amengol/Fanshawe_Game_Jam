#include "stdafx.h"
#include "cPhysicsFactory.h"
#include "cRigidBody.h"
#include "shapes.h"
#include "cPhysicsWorld.h"

namespace nPhysics
{
	cPhysicsFactory::~cPhysicsFactory() {}

	iPhysicsWorld* cPhysicsFactory::CreateWorld()
	{
        return new cPhysicsWorld();
	}

	iRigidBody* cPhysicsFactory::CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape)
	{
		return new cRigidBody(desc, shape);
	}

	iShape* cPhysicsFactory::CreateSphere(float radius)
	{
		return new cSphereShape(radius);
	}
	iShape* cPhysicsFactory::CreatePlane(const glm::vec3& normal, float planeConst)
	{
		return new cPlaneShape(normal, planeConst);
	}

    iShape * cPhysicsFactory::CreateCube(float size)
    {
        return new cCubeShape(size);
    }
	
}