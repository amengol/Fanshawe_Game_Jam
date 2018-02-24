#ifndef _cSimpleAi_Manager_HG_
#define _cSimpleAi_Manager_HG_

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>


#include <glm\vec3.hpp>
#include <string>
#include <vector>
#include <map>

struct sVertex;
class cGameObject;

class cSimpleAi_Manager
{
public:
    cSimpleAi_Manager() :
        debugGridEnable(false),
        mainGO(NULL),
        targetGO(NULL),
        currentTargetID(1000000),
        curTargetIndex(0),
        curTargetVel(0.0f)
    { }
    ~cSimpleAi_Manager();

    // Makes the grid of the Path system
    void makeGrid(unsigned int edgeSize, unsigned int rows,
                  unsigned int columns, glm::vec3 leftBottom);

    // Loads all obstacles of the scene. Must be called before loadNodes()
    void loadWalls(std::string meshName, std::vector<int> nodeIDs); 

    // Update Ai related stuff
    void updateAi();

    // Enables/Disables the degub grid
    void showDebugGrid(bool enable);

    // Creates main and target objects
    bool createMainObjects(std::string mainMeshName,
                           std::string targetMeshName,
                           unsigned int originID,
                           unsigned int targetID,
                           std::string& error);

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
            this->F_Distance = 1000000000;
            this->cameFrom = NULL;
            this->distanceSoFar = 1000000000;
        }

        int ID;
        int F_Distance;
        int distanceSoFar;
        Node* cameFrom;
        std::vector<Node*> connectedNodes;
        glm::vec3 position;
    };

    // Helper function to be used in the A_StarSearch
    int calcF_Distance(int current, int destination, int distanceSoFar);

    // A*: Returns a vector of sorted IDs from start to the destination
    std::vector<int> A_StarSearch(std::vector<Node*>& nodes, int start, int destination);

    // Returns the rotation quaternion between start and dest vector
    glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

    // Loads a mesh to represent a node and create edges to draw them
    void loadNodes(std::string meshName);

    // Makes the main GameObject go after a target
    void goToTarget();
    
    // Used with goToTarget to avoid look for a new target everytime
    std::vector<int> pathIDs;       // The main container with the final path
    size_t curTargetIndex;          // The current target index
    float curTargetVel;             // The current target velocity
    unsigned int currentTargetID;   // The current target ID
    glm::vec3 curTargetPos;         // The current target position

    // Returns the IDs of all possible neighbors from a Node
    std::vector<int> findNeighborsIDs(Node* n);

    std::vector<sVertex> mEdges;    // Vertices to draw the debug edges
    std::vector<Node*> mNodes;      // The Nodes of the grid
    unsigned int edgeSize;          // The length of an edge
    bool debugGridEnable;           // Flag the debug grid

    cGameObject* mainGO;            // The main GameObject (The Bunny)
    cGameObject* targetGO;          // The target GameObject (The carrot)
};

#endif // !_cSimpleAi_Manager_HG_

