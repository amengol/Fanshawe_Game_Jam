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

        virtual iRigidBody* CreateCharacterBody(const sRigidBodyDesc& desc, iShape* shape);
        
        virtual iConstraint* CreatePoint2PointConstraint(iRigidBody* rbA,
                                                         iRigidBody* rbB,
                                                         const glm::vec3& pivotInA,
                                                         const glm::vec3& pivotInB);

        virtual iConstraint* CreateHingeConstraint(iRigidBody* rbA,
                                                   iRigidBody* rbB,
                                                   const glm::vec3& pivotInA,
                                                   const glm::vec3& pivotInB,
                                                   glm::vec3& axisInA,
                                                   glm::vec3& axisInB);

        virtual iConstraint* CreateFixedConstraint(iRigidBody* rbA,
                                                   iRigidBody* rbB,
                                                   const glm::vec3& pivotInA,
                                                   const glm::vec3& pivotInB);

        virtual iConstraint* CreateUniversalConstraint(iRigidBody* rbA,
                                                       iRigidBody* rbB,
                                                       const glm::vec3& anchor,
                                                       const glm::vec3& axis1,
                                                       const glm::vec3& axis2);

		virtual iShape* CreateSphere(float radius);
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst);
        virtual iShape* CreateBox(const glm::vec3& halfExtents);
        virtual iShape* CreateConvexHull(const GLInstanceVertex*, size_t numOfVertices);
	};
}