#pragma once
#include "eShapeType.h"
#include <glm\vec3.hpp>

namespace nPhysics
{
	class iShape
	{
	public:
		virtual ~iShape() {}
		
        enum PlaneType
        {
            FLOOR,
            FRONT,
            BACK,
            LEFT,
            RIGHT
        };

		eShapeType GetShapeType() { return mShapeType; }

        virtual bool getPlaneType(PlaneType& type) { return false; }
        virtual bool setGameType(const PlaneType& type) { return false; }

		virtual bool GetSphereRadius(float& radiusOut) { return false; }
		virtual bool GetPlaneNormal(glm::vec3& normalOut) { return false; }
		virtual bool GetPlaneConst(float& planeConstOut) { return false; }

	protected:
		iShape(eShapeType shapeType)
			: mShapeType(shapeType)
		{

		}
		iShape() {}
		iShape(const iShape& other) {}
		iShape& operator=(const iShape& other) { return *this; }
	private:
		eShapeType mShapeType;
	};
}