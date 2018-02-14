#ifndef _cSimpleAi_Manager_HG_
#define _cSimpleAi_Manager_HG_

#include <glm\vec3.hpp>
#include <string>
#include <vector>

struct sVertex;

class cSimpleAi_Manager
{
public:
    cSimpleAi_Manager() : 
        debugGridEnable(false) 
    { }
    ~cSimpleAi_Manager();

    // Makes the grid of the Path system
    void makeGrid(unsigned int edgeSize, unsigned int rows,
                  unsigned int columns, glm::vec3 leftBottom);

    // Loads all obstacles of the scene. Must be called before loadNodes()
    void loadWalls(std::string meshName, std::vector<unsigned int> nodeIDs); 

    // Update Ai related stuff
    void updateAi();

    void showDebugGrid(bool enable);

private:
    struct Node
    {
        Node(int id, unsigned int edge, glm::vec3 leftBottom)
        {
            this->ID = id;

            int iX = id / 100;
            int iY = id - iX * 100;

            // Right handed coord. system:
            this->position.x = iX * edge;
            this->position.y = 0.0f;
            this->position.z = iY * edge;

            this->position += leftBottom;
        }

        int ID;
        std::vector<Node*> connectedNodes;
        glm::vec3 position;
    };

    // Loads a mesh to represent a node and create edges to draw them
    void loadNodes(std::string meshName);

    std::vector<int> findNeighborsIDs(Node* n);

    std::vector<sVertex> mEdges;
    std::vector<Node*> mNodes;
    unsigned int edgeSize;
    bool debugGridEnable;
};

#endif // !_cSimpleAi_Manager_HG_

