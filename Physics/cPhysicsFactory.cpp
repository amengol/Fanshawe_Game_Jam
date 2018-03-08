#include "stdafx.h"
#include "cPhysicsFactory.h"
#include "cRigidBody.h"
#include "shapes.h"
#include "cPhysicsWorld.h"
#include "cCloth.h"
#include "cSoftBody.h"

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

    iShape* cPhysicsFactory::CreateCube(float size)
    {
        return new cCubeShape(size);
    }

    iSoftBody* cPhysicsFactory::CreateSoftBody(iForm* form)
    {
        return new cSoftBody(form);
    }

    iForm* cPhysicsFactory::CreateCloth(glm::vec3 upperLeftCornerPostion,
                                        float damping,
                                        float nodeMass,
                                        float width,
                                        float height,
                                        int numNodesWidth,
                                        int numNodesHeight)
    {
        return new cCloth(upperLeftCornerPostion,
                          damping,
                          nodeMass,
                          width,
                          height,
                          numNodesWidth,
                          numNodesHeight);
    }
	
}