#include "stdafx.h"
#include "cSpring.h"
#include <glm\glm.hpp>

nPhysics::cSpring::cSpring(cNode* n1, cNode* n2)
{
    node1 = n1;
    node2 = n2;
    glm::vec3 vec = node1->getPos() - node2->getPos();
    mRestDistance = glm::length(vec);
}

void nPhysics::cSpring::restoreSpring()
{
    // Find the current distance between the nodes
    glm::vec3 vecN1N2 = node2->getPos() - node1->getPos();
    float currentDistance = glm::length(vecN1N2);

    // The vector that could moves Node1 into a rest distance related to Node1
    glm::vec3 correctionVector = vecN1N2 * (1.0f - mRestDistance / currentDistance);

    // Each node has to move halfway the correctionVector
    glm::vec3 correctionVectorHalf = correctionVector * 0.5f; 
    node1->offsetPos(correctionVectorHalf); 
    node2->offsetPos(-correctionVectorHalf);
}
