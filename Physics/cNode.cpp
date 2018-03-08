#include "stdafx.h"
#include "cNode.h"
#include <glm\glm.hpp>

void nPhysics::cNode::addToNormal(glm::vec3 normal)
{
    mAccumulatedNormal += glm::normalize(normal);
}

void nPhysics::cNode::timeStep(float deltaTime)
{
    if (mMovable)
    {
        glm::vec3 temp = mPos;
        // verlet numerical integration
        mPos = mPos + (mPos - mOldPos) * (1.0f - mDamping) + mAcceleration * deltaTime; 
        mOldPos = temp;
        mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}
