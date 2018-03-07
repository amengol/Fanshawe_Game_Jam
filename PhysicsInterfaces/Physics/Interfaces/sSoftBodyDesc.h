#pragma once
#include <glm\vec3.hpp>

namespace nPhysics
{
	struct sSoftBodyDesc
	{
        sSoftBodyDesc()
			: UpperLeftPosition(0.f, 0.f, 0.f)
			, nodeMass(1.f)
		{

		}
		glm::vec3 UpperLeftPosition;
		float nodeMass;
	};
}