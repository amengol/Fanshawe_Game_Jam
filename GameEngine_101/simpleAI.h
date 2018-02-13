#ifndef _cSimpleAi_Manager_HG_
#define _cSimpleAi_Manager_HG_

#include "cGameObject.h"

struct Node
{
    Node(int id)
    {
        int iX = id / 100;
        int iY = id - iX;

        // Right handed coord. system:
        this->position.x = iX;
        this->position.y = 0.0f;
        this->position.z = iY;
    }

    int ID;
    std::vector<Node*> connectedNodes;
    glm::vec3 position;
};

std::vector<int> findNeighborsIDs(Node* n)
{
    // Decompose the ID
    int ID = n->ID;
    int x = ID / 100;
    int y = ID - x * 100;

    std::vector<int> ret;

    // Layout:
    //  n7  n6  n5
    //  n8  N   n4
    //  n1  n2  n3

    int n1 = -1;
    int n2 = -1;
    int n3 = -1;
    int n4 = -1;
    int n5 = -1;
    int n6 = -1;
    int n7 = -1;
    int n8 = -1;

    if (x == 0 && y == 0)
    {
        n4 = 100;
        n5 = 101;
        n6 = 1;

        ret.push_back(n4);
        ret.push_back(n5);
        ret.push_back(n6);

        return ret;
    }

    if (x == 0)
    {
        n2 = y - 1;
        n3 = 100 + y - 1;
        n4 = 100 + y;
        n5 = 100 + y + 1;
        n6 = y + 1;

        ret.push_back(n2);
        ret.push_back(n3);
        ret.push_back(n4);
        ret.push_back(n5);
        ret.push_back(n6);
        
        return ret;
    }

    if (y == 0)
    {
        n4 = (x + 1) * 100;
        n5 = (x + 1) * 100 + 1;
        n6 = x * 100 + 1;
        n7 = (x - 1) * 100 - 1;
        n8 = (x - 1) * 100;
       
        ret.push_back(n4);
        ret.push_back(n5);
        ret.push_back(n6);
        ret.push_back(n7);
        ret.push_back(n8);
        
        return ret;
    }

    n1 = (x - 1) * 100 + y - 1;
    n2 = x * 100 + y - 1;
    n3 = (x + 1) * 100 + y - 1;
    n4 = (x + 1) * 100 + y;
    n5 = (x + 1) * 100 + y + 1;
    n6 = x * 100 + y + 1;
    n7 = (x - 1) * 100 + y + 1;
    n8 = (x - 1) * 100 + y;

    ret.push_back(n1);
    ret.push_back(n2);
    ret.push_back(n3);
    ret.push_back(n4);
    ret.push_back(n5);
    ret.push_back(n6);
    ret.push_back(n7);
    ret.push_back(n8);

    return ret;
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
            Node* n = new Node(ID);
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

//void update_AI_Parameters(cGameObject* theGO, float threshold)
//{
//    float distanceToTarget = glm::length(theGO->target - theGO->position);
//
//    if(distanceToTarget > threshold)
//    {
//
//        glm::vec3 direction = glm::normalize(theGO->target - theGO->position);
//        glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), direction);
//
//        theGO->orientation = glm::toMat4(rot1);
//    }
//    else
//    {
//        unsigned int vecSize = theGO->wayPointList.size();
//        if(theGO->currentTargetIndex == vecSize - 1)
//        {
//            theGO->currentTargetIndex = 0;
//            theGO->target = theGO->wayPointList[0];
//        }
//        else
//        {
//            theGO->currentTargetIndex++;
//            theGO->target = theGO->wayPointList[theGO->currentTargetIndex];
//        }
//    }
//}

#endif // !_cSimpleAi_Manager_HG_

