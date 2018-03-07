#pragma once
#include <glm\vec3.hpp>

namespace nPhysics
{
	class iNode
	{
	public:
		virtual ~iNode() {}

        // Adds a force to a node
        virtual void addForce(glm::vec3 force) = 0;

        // Gets the node position
        virtual glm::vec3& getPos() = 0;

        // Resets node acceleration
        virtual void resetAcceleration() = 0;

        // Offsets the node position
        virtual void offsetPos(const glm::vec3 position) = 0;

        // Makes a node movable
        virtual void makeUnmovable() = 0;

        // Adds to the accunulated normal (for shading)
        virtual void addToNormal(glm::vec3 normal) = 0;

        // Gets node's normal (vertex normal)
        virtual glm::vec3& getNormal() = 0;

        // Resets node's normal (vertex normal)
        virtual void resetNormal() = 0;

        // Advance the node in time
        virtual void timeStep(float deltaTime) = 0;
	};
}