#pragma once
#include <glm\vec3.hpp>

namespace nPhysics
{
	struct sRigidBodyDesc
	{
		sRigidBodyDesc()
			: Position(0.f, 0.f, 0.f)
			, Velocity(0.f, 0.f, 0.f)
			, Rotation(0.f, 0.f, 0.f)
			, Mass(1.f)
            , Scale(1.f)
		{

		}
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Rotation;
		float Mass;
        float Scale;
	};
}