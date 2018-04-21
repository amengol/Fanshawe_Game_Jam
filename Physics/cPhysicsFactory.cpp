#include "stdafx.h"
#include "cPhysicsFactory.h"
#include "cRigidBody.h"
#include "shapes.h"
#include "cPhysicsWorld.h"
#include "cPoint2PointConstraint.h"
#include "cHingeConstraint.h"
#include "cFixedConstraint.h"
#include "cUniversalConstraint.h"

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

    iConstraint* cPhysicsFactory::CreatePoint2PointConstraint(iRigidBody* rbA, 
                                                                 iRigidBody* rbB, 
                                                                 const glm::vec3 & pivotInA, 
                                                                 const glm::vec3 & pivotInB)
    {
        return new cPoint2PointConstraint(rbA, rbB, pivotInA, pivotInB);
    }

    iConstraint * cPhysicsFactory::CreateHingeConstraint(iRigidBody* rbA, 
                                                            iRigidBody* rbB, 
                                                            const glm::vec3& pivotInA,
                                                            const glm::vec3& pivotInB,
                                                            glm::vec3 & axisInA, 
                                                            glm::vec3 & axisInB)
    {
        return new cHingeConstraint(rbA, rbB, pivotInA, pivotInB, axisInA, axisInB);
    }

    iConstraint * cPhysicsFactory::CreateFixedConstraint(iRigidBody* rbA,
                                                            iRigidBody* rbB, 
                                                            const glm::vec3& pivotInA, 
                                                            const glm::vec3& pivotInB)
    {
        return new cFixedConstraint(rbA, rbB, pivotInA, pivotInB);
    }

    iConstraint * cPhysicsFactory::CreateUniversalConstraint(iRigidBody* rbA, 
                                                                iRigidBody* rbB, 
                                                                const glm::vec3& anchor, 
                                                                const glm::vec3& axis1, 
                                                                const glm::vec3& axis2)
    {
        return new cUniversalConstraint(rbA, rbB, anchor, axis1, axis2);
    }

	iShape* cPhysicsFactory::CreateSphere(float radius)
	{
		return new cSphereShape(radius);
	}
    iShape* cPhysicsFactory::CreateCapsule(float radius, float height)
    {
        return new cCapsuleShape(radius, height);
    }
	iShape* cPhysicsFactory::CreatePlane(const glm::vec3& normal, float planeConst)
	{
		return new cPlaneShape(normal, planeConst);
	}
    iShape* cPhysicsFactory::CreateBox(const glm::vec3 & halfExtents)
    {
        return new cBoxShape(halfExtents);
    }
    iShape* cPhysicsFactory::CreateConvexHull(const GLInstanceVertex* instanceVertex, size_t numOfVertices)
    {
        return new cConvexHullShape(instanceVertex, numOfVertices);
    }
}