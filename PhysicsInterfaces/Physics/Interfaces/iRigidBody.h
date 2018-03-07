#pragma once
#include "iShape.h"
#include "iCollisionBody.h"

namespace nPhysics
{
	class iRigidBody : public iCollisionBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual iShape* GetShape() = 0;
        
        virtual void SetRateOfTurnX(float rotX) = 0;
        virtual void SetRateOfTurnY(float rotY) = 0;
        virtual void SetRateOfTurnZ(float rotZ) = 0;
	};
}