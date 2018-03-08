#pragma once
#include <iSpring.h>
#include "cNode.h"

namespace nPhysics
{
    class cSpring : public iSpring
	{
	public:
        cSpring(cNode* n1, cNode* n2) : node1(n1), node2(n2)
        {
            glm::vec3 vec = node1->getPos() - node2->getPos();
            mRestDistance = glm::length(vec);
        }

		virtual ~cSpring() {}

        // Offesets the two nodes of the spring according Hooke's Law
        virtual void restoreSpring();

        cNode *node1 ,*node2;   // The nodes of the spring
        
    private:
        
        float mRestDistance;    // The distance between Node1 and Node2 when resting
	};
}