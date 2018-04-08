#include "cSimpleAi_Manager.h"

#include <glm\gtx\vector_angle.hpp>

#include "..\globalGameStuff.h"


cSimpleAi_Manager::~cSimpleAi_Manager()
{
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        delete this->mNodes[i];
    }
}

void cSimpleAi_Manager::makeGrid(unsigned int edgeSize, unsigned int rows,
                                 unsigned int columns, glm::vec3 leftBottom)
{
    // Our grid has a limit
    if (rows > 99 || columns > 99)
        return;

    // Calc the IDs and create the nodes
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            // Shift X to the left
            int x = j * 100;
            int y = i;
            int ID = x + y;
            Node* n = new Node(ID, edgeSize, leftBottom);
            this->mNodes.push_back(n);
        }
    }

    this->edgeSize = edgeSize;
}

glm::quat cSimpleAi_Manager::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)

{
    start = glm::normalize(start);
    dest = glm::normalize(dest);

    float cosTheta = dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f)
    {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = cross(start, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );

}

void cSimpleAi_Manager::loadNodes(std::string meshName)
{
    // Connect all neighbors
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        Node* n1 = this->mNodes[i];

        std::vector<int> IDs = findNeighborsIDs(n1);

        for (size_t j = 0; j < this->mNodes.size(); j++)
        {
            Node* n2 = this->mNodes[j];

            if (n1 != n2)
            {
                for (size_t k = 0; k < IDs.size(); k++)
                {
                    if (n2->ID == IDs[k])
                    {
                        // Connect them
                        n1->connectedNodes.push_back(n2);
                    }
                }
            }
        }
    }

    // Puts a mesh in the node location to represent it
    // and create the edges (Just to draw them)
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        Node* n1 = this->mNodes[i];


        for (size_t j = 0; j < n1->connectedNodes.size(); j++)
        {
            sVertex vN1;
            vN1.x = n1->position.x;
            vN1.y = n1->position.y;
            vN1.z = n1->position.z;

            Node* n2 = n1->connectedNodes[j];
            sVertex vN2;
            vN2.x = n2->position.x;
            vN2.y = n2->position.y;
            vN2.z = n2->position.z;

            this->mEdges.push_back(vN1);
            this->mEdges.push_back(vN2);
        }

        cGameObject* GO = new cGameObject();
        GO->meshName = meshName;
        GO->friendlyName = "node";
        GO->renderable = false;
        GO->scale = this->edgeSize * 0.025f;
        GO->position = this->mNodes[i]->position;
        g_vecGameObjects.push_back(GO);
    }
}

void cSimpleAi_Manager::loadWalls(std::string meshName, std::vector<int> nodeIDs)
{
    for (size_t i = 0; i < nodeIDs.size(); i++)
    {
        for (size_t j = 0; j < this->mNodes.size(); j++)
        {
            if (nodeIDs[i] == this->mNodes[j]->ID)
            {
                cGameObject* GO = new cGameObject();
                GO->meshName = meshName;
                GO->textureBlend[0] = 1.0f;
                GO->textureNames[0] = meshName + ".bmp";
                GO->bIsWireFrame = false;
                GO->scale = this->edgeSize;
                GO->position = this->mNodes[j]->position;
                g_vecGameObjects.push_back(GO);

                this->mNodes.erase(this->mNodes.begin() + j);                
            }
        }
    }

    this->loadNodes(meshName);
}

void cSimpleAi_Manager::updateAi()
{
    if (this->debugGridEnable)
    {
        g_pSimpleDebug->drawCustomLines(this->mEdges, glm::vec3(1.0f, 0.0f, 0.0f));
    }    

    this->goToTarget();
}

void cSimpleAi_Manager::showDebugGrid(bool enable)
{
    for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    {
        if (g_vecGameObjects[i]->friendlyName == "node")
            g_vecGameObjects[i]->renderable = enable;
    }

    this->debugGridEnable = enable;
}

bool cSimpleAi_Manager::createMainObjects(std::string mainMeshName, 
                                          std::string targetMeshName, 
                                          unsigned int originID, 
                                          unsigned int targetID,
                                          std::string& error)
{
    bool foundID = false;
    glm::vec3 mainPos(0.0f);
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        if (this->mNodes[i]->ID == originID)
        {
            mainPos = this->mNodes[i]->position;
            foundID = true;
            break;
        }
    }

    if (!foundID)
    {
        error.append("The node ID for the origin was not found!\n");
        return false;
    }

    foundID = false;
    glm::vec3 targetPos(0.0f);
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        if (this->mNodes[i]->ID == targetID)
        {
            targetPos = this->mNodes[i]->position;
            foundID = true;
            break;
        }
    }

    if (!foundID)
    {
        error.append("The node ID for the target was not found!\n");
        return false;
    }

    // Main GameObject
    this->mainGO = new cGameObject();
    this->mainGO->meshName = mainMeshName;
    this->mainGO->textureBlend[0] = 1.0f;
    this->mainGO->textureNames[0] = mainMeshName + ".bmp";
    this->mainGO->friendlyName = "AiMainGameObject";
    nPhysics::sRigidBodyDesc desc;
    desc.Position = mainPos;
    nPhysics::iShape* shape = g_pPhysicsFactory->CreateBox(glm::vec3(0.5f));
    nPhysics::iRigidBody* rb = g_pPhysicsFactory->CreateRigidBody(desc, shape);
    this->mainGO->rigidBody = rb;
    g_vecGameObjects.push_back(this->mainGO);

    // Target GameObject
    this->targetGO = new cGameObject();
    this->targetGO->meshName = targetMeshName;
    this->targetGO->hasColour = true;
    this->targetGO->friendlyName = "AiTargetGameObject";
    desc;
    desc.Position = targetPos;
    shape = g_pPhysicsFactory->CreateBox(glm::vec3(0.5f));
    rb = g_pPhysicsFactory->CreateRigidBody(desc, shape);
    //rb->SetRateOfTurnY(5760.0f);
    this->targetGO->rigidBody = rb;
    g_vecGameObjects.push_back(this->targetGO);

    // Target position
    if (originID != this->currentTargetID)
    {
        for (size_t i = 0; i < this->mNodes.size(); i++)
        {
            if (this->mNodes[i]->ID == originID)
            {
                this->currentTargetID = originID;
                this->curTargetPos = this->mNodes[i]->position;
                this->curTargetVel = 10.0f;
            }
        }
    }

    this->pathIDs = A_StarSearch(this->mNodes, originID, targetID);

    return true;
}

int cSimpleAi_Manager::calcF_Distance(int current, int destination, int distanceSoFar)

{
    // Current:
    int xCur = current / 100;
    int yCur = current - xCur * 100;

    // Destination:
    int xDest = destination / 100;
    int yDest = destination - xDest * 100;

    // Manhatan distance
    int manhatan = abs(xDest - xCur) + abs(yDest - yCur);

    return manhatan + distanceSoFar;
}

std::vector<int> cSimpleAi_Manager::A_StarSearch(std::vector<Node*>& nodes, int start, int destination)

{
    std::map<int, Node*> openList;
    std::map<int, Node*> closedList;

    Node* current = NULL;

    // Find the start node
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->ID == start)
        {
            current = nodes[i];
            current->distanceSoFar = 0;
            current->F_Distance = calcF_Distance(start, destination, current->distanceSoFar);
            break;
        }
    }

    while (current->ID != destination)
    {
        for (size_t i = 0; i < current->connectedNodes.size(); i++)
        {
            Node* n = current->connectedNodes[i];

            if (openList.find(n->ID) == openList.end()
                && closedList.find(n->ID) == closedList.end())
            {
                openList[n->ID] = n;
                n->distanceSoFar = current->distanceSoFar + 1;
                int newF_Distance = calcF_Distance(n->ID, destination, n->distanceSoFar);

                if (newF_Distance < n->F_Distance)
                {
                    n->F_Distance = newF_Distance;
                    n->cameFrom = current;
                }// !_if (newF_Distance...
            }// !_if (openList.find(n->ID)
        }// !_for (size_t i = 0...

        closedList[current->ID] = current;

        std::map<int, Node*>::iterator it = openList.begin();
        std::map<int, Node*>::iterator itToErase;
        int leastF_Distance = 1000000000;
        for (; it != openList.end(); it++)
        {
            if (it->second->F_Distance < leastF_Distance)
            {
                leastF_Distance = it->second->F_Distance;
                current = it->second;
                itToErase = it;
            }
        }

        openList.erase(itToErase);

    }// !_while (current->ID != destination)

     // Now the path back
    std::vector<int> pathDesc;
    pathDesc.push_back(current->ID);
    while (current->cameFrom != NULL)
    {
        current = current->cameFrom;
        pathDesc.push_back(current->ID);
    }

    // Re-order
    std::vector<int> pathAsc;
    for (int i = pathDesc.size() - 1; i >= 0; i--)
    {
        pathAsc.push_back(pathDesc[i]);
    }

    return pathAsc;
}

void cSimpleAi_Manager::goToTarget()
{   
    // Do nothing if we are at the and or no path at all
    if (this->curTargetIndex == this->pathIDs.size())
        return;

    // Distance
    glm::vec3 mainGOPos;
    this->mainGO->rigidBody->GetPostion(mainGOPos);
    float distance = glm::length(this->curTargetPos - mainGOPos);

    if (distance <= 0.3)
    {
        // Set the next node
        this->curTargetIndex++;
        if (this->curTargetIndex == this->pathIDs.size())
        {
            this->mainGO->rigidBody->SetVelocityLocal(glm::vec3(0.0f));
            return;
        }

        this->currentTargetID = this->pathIDs[this->curTargetIndex];
        for (size_t i = 0; i < this->mNodes.size(); i++)
        {
            if (this->mNodes[i]->ID == this->currentTargetID)
            {
                this->curTargetPos = this->mNodes[i]->position;
            }
        }

        return;
    }

    // Orient the Main GO to the target
    glm::vec3 a = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 b = glm::normalize(this->curTargetPos - mainGOPos);
    glm::quat theRot = RotationBetweenVectors(a, b);
    this->mainGO->rigidBody->SetRotation(theRot);// ->SetMatOrientation(orientation);

    // Set the velocity
    this->mainGO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, this->curTargetVel));
}

std::vector<int> cSimpleAi_Manager::findNeighborsIDs(Node* n)
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

    if (x == 0 && y == 0)
    {
        n1 = 100;
        n2 = 1;

        ret.push_back(n1);
        ret.push_back(n2);

        return ret;
    }

    if (x == 0)
    {
        n1 = 100 + y;
        n2 = y + 1;
        n4 = y - 1;

        ret.push_back(n1);
        ret.push_back(n2);
        ret.push_back(n4);

        return ret;
    }

    if (y == 0)
    {
        n1 = (x + 1) * 100;
        n2 = x * 100 + 1;
        n3 = (x - 1) * 100;

        ret.push_back(n1);
        ret.push_back(n2);
        ret.push_back(n3);

        return ret;
    }

    n1 = (x + 1) * 100 + y;
    n2 = x * 100 + y + 1;
    n3 = (x - 1) * 100 + y;
    n4 = x * 100 + y - 1;

    ret.push_back(n1);
    ret.push_back(n2);
    ret.push_back(n3);
    ret.push_back(n4);

    return ret;
}