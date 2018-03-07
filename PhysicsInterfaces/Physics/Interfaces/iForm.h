#pragma once
#include "eSoftBodyType.h"
#include <glm\vec3.hpp>

namespace nPhysics
{
	class iForm
	{
	public:
		virtual ~iForm() {}

        eSoftBodyType GetSoftBodyType() { return mSoftBodyType; }

		virtual bool SetWind(glm::vec3& wind) { return false; }

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