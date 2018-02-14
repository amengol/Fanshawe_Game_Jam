#include "cSimpleAi_Manager.h"

#include "cSimpleDebugRenderer.h"
#include "cGameObject.h"
#include <iPhysicsFactory.h>
extern std::vector< cGameObject* >  g_vecGameObjects;
extern cSimpleDebugRenderer* g_simpleDebug;
extern nPhysics::iPhysicsFactory* gPhysicsFactory;


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

void cSimpleAi_Manager::loadWalls(std::string meshName, std::vector<unsigned int> nodeIDs)
{
    for (size_t i = 0; i < nodeIDs.size(); i++)
    {
        for (size_t j = 0; j < this->mNodes.size(); j++)
        {
            if (nodeIDs[i] == this->mNodes[j]->ID)
            {
                cGameObject* GO = new cGameObject();
                GO->meshName = meshName;
                GO->bIsWireFrame = true;
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
        g_simpleDebug->drawCustomLines(this->mEdges, glm::vec3(1.0f, 0.0f, 0.0f));
    }    

    this->goToTarget(808, 5.0f);
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
    this->mainGO->friendlyName = "AiMainGameObject";
    nPhysics::sRigidBodyDesc desc;
    desc.Position = mainPos;
    nPhysics::iShape* shape = gPhysicsFactory->CreateCube(1.0f);
    nPhysics::iRigidBody* rb = gPhysicsFactory->CreateRigidBody(desc, shape);
    this->mainGO->rigidBody = rb;
    g_vecGameObjects.push_back(this->mainGO);

    // Target GameObject
    this->targetGO = new cGameObject();
    this->targetGO->meshName = targetMeshName;
    this->targetGO->friendlyName = "AiTargetGameObject";
    this->targetGO->position = targetPos;
    g_vecGameObjects.push_back(this->targetGO);

    return true;
}

void cSimpleAi_Manager::goToTarget(unsigned int targetID, float velocity)
{
    // Target position
    if (targetID != this->currentTargetID)
    {
        for (size_t i = 0; i < this->mNodes.size(); i++)
        {
            if (this->mNodes[i]->ID == targetID)
            {
                this->currentTargetID = targetID;
                this->curTargetPos = this->mNodes[i]->position;
            }
        }
    }

    // Orient the Main GO to the target
    glm::vec3 mainGOPos;
    this->mainGO->rigidBody->GetPostion(mainGOPos);
    glm::vec3 a = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 b = glm::normalize(this->curTargetPos - mainGOPos);
    float angle = acos(glm::dot(b, a) / (glm::length(b) * glm::length(a)));

    glm::mat4 orientation(1.0f);
    if (glm::dot(b, a) >= 0.0f)
    {
        orientation = glm::rotate(orientation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else
    {
        orientation = glm::rotate(orientation, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    
    this->mainGO->rigidBody->SetMatOrientation(orientation);
    this->mainGO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, velocity));
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