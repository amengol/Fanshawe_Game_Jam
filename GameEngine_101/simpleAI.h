#ifndef _cSimpleAi_Manager_HG_
#define _cSimpleAi_Manager_HG_

#include "cGameObject.h"

struct Node
{
    int ID;
    std::vector<Node*> connectedNodes;
};

std::vector<int> findNeighborsIDs(Node* n)
{
    // Decompose the ID
    int ID = n->ID;
    int x = ID / 100;
    int y = ID - x * 100;

    std::vector<int> ret;

    int n1, n2, n3, n4, n5, n6, n7, n8;

    if (x == 0 && y == 0)
    {
        // first

    }
}

std::vector<Node*> makeGrid(int edgeSize, unsigned int rows, unsigned int columns)
{
    // Our grid has a limit
    if (rows > 99 || columns > 99)
        return;

    std::vector<Node*> nodes;

    // Calc the IDs and create the nodes
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            // Shift X to the left
            int x = i * rows * 100;
            int y = j * columns;
            int ID = x + y;
            Node* n = new Node();
            nodes.push_back(n);
        }
    }

    // Connect all neighbors
    for (size_t i = 0; i < nodes.size(); i++)
    {
        Node* n1 = nodes[i];

        std::vector<int> IDs = findNeighborsIDs(n1);

        for (size_t j = 0; j < nodes.size(); j++)
        {
            Node* n2 = nodes[j];

            if (n1 != n2)
            {
                for (size_t k = 0; k < IDs.size(); k++)
                {
                    if (n2->ID == IDs[i])
                    {
                        // Connect both
                        n1->connectedNodes.push_back(n2);
                        n2->connectedNodes.push_back(n1);
                    }
                }
            }
        }
    }
}

void update_AI_Parameters(cGameObject* theGO, float threshold)
{
    float distanceToTarget = glm::length(theGO->target - theGO->position);

    if(distanceToTarget > threshold)
    {

        glm::vec3 direction = glm::normalize(theGO->target - theGO->position);
        glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), direction);

        theGO->orientation = glm::toMat4(rot1);
    }
    else
    {
        unsigned int vecSize = theGO->wayPointList.size();
        if(theGO->currentTargetIndex == vecSize - 1)
        {
            theGO->currentTargetIndex = 0;
            theGO->target = theGO->wayPointList[0];
        }
        else
        {
            theGO->currentTargetIndex++;
            theGO->target = theGO->wayPointList[theGO->currentTargetIndex];
        }
    }
}

#endif // !_cSimpleAi_Manager_HG_

