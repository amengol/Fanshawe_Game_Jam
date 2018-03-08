#pragma once
#include "eSoftBodyType.h"
#include <glm\vec3.hpp>
#include <vector>
#include "iNode.h"

namespace nPhysics
{
	class iForm
	{
	public:
		virtual ~iForm() {}

        // Returns the soft body type
        eSoftBodyType GetSoftBodyType() { return mSoftBodyType; }

        // Sets the wind
		virtual void SetWind(glm::vec3& wind) { return; }

        // Clears the wind
        virtual void ClearWind() { return; }

        // Sets Gravity
        virtual void SetGravity(glm::vec3& gravity) { return; }

        // Clears Gravity
        virtual void ClearGravity() { return; }

	protected:
        iForm(eSoftBodyType shapeType)
			: mSoftBodyType(shapeType)
		{

		}
        iForm() {}
        iForm(const iForm& other) {}
        iForm& operator=(const iForm& other) { return *this; }
	private:
        eSoftBodyType mSoftBodyType;
	};
}