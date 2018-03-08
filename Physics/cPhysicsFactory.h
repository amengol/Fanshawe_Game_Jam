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

        virtual iSoftBody* CreateSoftBody(iForm* shape) { return NULL; }

        virtual iForm* CreateCloth(glm::vec3 upperLeftCornerPostion,
                                   float damping,
                                   float nodeMass,
                                   float width,
                                   float height,
                                   int numNodesWidth,
                                   int numNodesHeight);

        virtual iForm* CreateRubber() { return NULL; }
        virtual iForm* Water() { return NULL; }
	};
}