#pragma once
#include <glm\vec3.hpp>

namespace nPhysics
{
	struct sSoftBodyDesc
	{
        sSoftBodyDesc()
			: position(0.f, 0.f, 0.f)
		{

		}
		glm::vec3 position;
	};
}