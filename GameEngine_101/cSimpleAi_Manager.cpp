#include "cSimpleAi_Manager.h"

#include "cSimpleDebugRenderer.h"
#include "cGameObject.h"
extern std::vector< cGameObject* >  g_vecGameObjects;
extern cSimpleDebugRenderer* g_simpleDebug;

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
    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        if (this->mNodes[i]->ID == originID)
        {
            this->mainGO = new cGameObject();
            this->mainGO->meshName = mainMeshName;
            this->mainGO->friendlyName = "AiMainGameObject";
            this->mainGO->position = this->mNodes[i]->position;
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

    for (size_t i = 0; i < this->mNodes.size(); i++)
    {
        if (this->mNodes[i]->ID == targetID)
        {
            this->targetGO = new cGameObject();
            this->targetGO->meshName = targetMeshName;
            this->targetGO->friendlyName = "AiTargetGameObject";
            this->targetGO->position = this->mNodes[i]->position;
            foundID = true;
            break;
        }
    }

    if (!foundID)
    {
        error.append("The node ID for the target was not found!\n");
        if (this->mainGO != NULL)
        {
            delete this->mainGO;
            this->mainGO = NULL;
        }
        return false;
    }

    g_vecGameObjects.push_back(this->mainGO);
    g_vecGameObjects.push_back(this->targetGO);
    return true;
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