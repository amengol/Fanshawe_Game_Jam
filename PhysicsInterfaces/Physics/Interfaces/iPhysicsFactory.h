#pragma once
#include "iRigidBody.h"
#include "sRigidBodyDesc.h"
#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iConstraint.h"

namespace nPhysics
{
    struct GLInstanceVertex;

	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}

		virtual iPhysicsWorld* CreateWorld() = 0;

		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape) = 0;

        virtual iConstraint* CreatePoint2PointConstraint(iRigidBody* rbA,
                                                         iRigidBody* rbB,
                                                         const glm::vec3& pivotInA,
                                                         const glm::vec3& pivotInB) = 0;

        virtual iConstraint* CreateHingeConstraint(iRigidBody* rbA,
                                                   iRigidBody* rbB,
                                                   const glm::vec3& pivotInA,
                                                   const glm::vec3& pivotInB,
                                                   glm::vec3& axisInA,
                                                   glm::vec3& axisInB) = 0;

        virtual iConstraint* CreateFixedConstraint(iRigidBody* rbA,
                                                   iRigidBody* rbB,
                                                   const glm::vec3& pivotInA,
                                                   const glm::vec3& pivotInB) = 0;

        virtual iConstraint* CreateUniversalConstraint(iRigidBody* rbA,
                                                       iRigidBody* rbB,
                                                       const glm::vec3& anchor,
                                                       const glm::vec3& axis1,
                                                       const glm::vec3& axis2) = 0;

		virtual iShape* CreateSphere(float radius) = 0;
        virtual iShape* CreateCapsule(float radius, float height) = 0;
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst) = 0;
        virtual iShape* CreateBox(const glm::vec3& halfExtents) = 0;
        virtual iShape* CreateConvexHull(const GLInstanceVertex*, size_t numOfVertices) = 0;
	};
}