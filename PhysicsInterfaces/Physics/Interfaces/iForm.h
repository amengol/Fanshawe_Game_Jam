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

        eSoftBodyType GetSoftBodyType() { return mSoftBodyType; }
        std::vector<iNode*> GetSoftBodyNodes() { return mSoftBoryNodes; }

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
        std::vector<iNode*> mSoftBoryNodes;
	};
}