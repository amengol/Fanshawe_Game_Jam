#pragma once
#include <iNode.h>

namespace nPhysics
{
	class cNode : public iNode
	{
	public:
        cNode(glm::vec3 pos, float damping, float mass) : 
            mPos(pos), 
            mOldPos(pos), 
            mAcceleration(glm::vec3(0.0f, 0.0f, 0.0f)), 
            mMass(mass),
            mDamping(damping),
            mMovable(true), 
            mAccumulatedNormal(glm::vec3(0.0f, 0.0f, 0.0f)) 
        { }

        cNode() { }

		virtual ~cNode() {}

        // Adds a force to a node
        virtual void addForce(glm::vec3 force) { mAcceleration += force / mMass; }

        // Gets the node position
        virtual glm::vec3& getPos() { return mPos; }

        // Resets node acceleration
        virtual void resetAcceleration() { mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f); }

        // Offsets the node position
        virtual void offsetPos(const glm::vec3 position) { if (mMovable) mPos += position; }

        // Makes a node movable
        virtual void makeUnmovable() { mMovable = false; }

        // Adds to the accunulated normal (for shading)
        virtual void addToNormal(glm::vec3 normal);

        // Gets node's normal (vertex normal)
        virtual glm::vec3& getNormal() { return mAccumulatedNormal; } // Notice: not normilized

        // Resets node's normal (vertex normal)
        virtual void resetNormal() { mAccumulatedNormal = glm::vec3(0.0f, 0.0f, 0.0f); }

        // Advance the node in time
        virtual void timeStep(float deltaTime);

    private:
        bool mMovable;                   // Pin the particle?
        float mMass;                    // the mass of the node
        float mDamping;                  // How much to dump in each fram
        glm::vec3 mPos;                 // the current position of the node
        glm::vec3 mOldPos;              // the position of the node in the previous time step
        glm::vec3 mAcceleration;        // a vector representing the current acceleration of the node
        glm::vec3 mAccumulatedNormal;   // an accumulated normal (i.e. non normalized), used for OpenGL soft shading
	};
}