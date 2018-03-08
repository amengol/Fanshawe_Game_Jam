#pragma once
#include "iRigidBody.h"
#include "sRigidBodyDesc.h"
#include "iShape.h"
#include "iPhysicsWorld.h"
#include "iSoftBody.h"
#include "iForm.h"

namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}

		virtual iPhysicsWorld* CreateWorld() = 0;

		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape) = 0;
		
		virtual iShape* CreateSphere(float radius) = 0;
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst) = 0;
        virtual iShape* CreateCube(float size) = 0;

        virtual iSoftBody* CreateSoftBody(iForm* form) = 0;

        virtual iForm* CreateCloth(glm::vec3 upperLeftCornerPostion,
                                   float damping,
                                   float nodeMass,
                                   float width, 
                                   float height, 
                                   int numNodesWidth, 
                                   int numNodesHeight) = 0;

        virtual iForm* CreateRubber() = 0;
        virtual iForm* Water() = 0;
	};
}