#pragma once
#include <iForm.h>
#include "cSpring.h"

namespace nPhysics
{
    class cNode;
    class cClothMesh;

    class cCloth : public iForm
    {
    public:

        cCloth(glm::vec3 upLeftPostion,
               float damping,
               float nodeMass,
               float width,
               float height,
               int numNodesWidth, 
               int numNodesHeight);

        virtual ~cCloth() { }

        // Sets the wind
        virtual void SetWind(glm::vec3& wind);

        // Clears the wind
        virtual void ClearWind();

        // Sets Gravity
        virtual void SetGravity(glm::vec3& gravity);

        // Clears Gravity
        virtual void ClearGravity();

        // This is the main time step for all cloth simulation.  This includes
        //  calling restoreSpring() for every spring, and timeStep() for all node
        void TimeStep(float deltaTime, size_t iterations);

        // Used to detect and resolve the collision of the cloth with the sphere.
        // The position of each node is simply compared to the sphere and corrected.
        // This also means that the sphere can "slip through" if it is small enough 
        // compared to the distance in the grid bewteen nodes
        void SphereCollision(const glm::vec3 center, const float radius);

        // Gets the current cloth mesh
        // WARNING: sVertex*'s and cTriangle* MUST be deleted after using them
        void GetClothMesh(cClothMesh& mesh);

    private:
        int mNumNodesWidth;              // number of nodes in "width" direction
        int mNumNodesHeight;             // number of nodes in "height" direction
        std::vector<cNode> mNodes;       // all nodes that are part of this cloth
        std::vector<cSpring> mSprings;   // alle springs between nodes as part of this cloth
        glm::vec3 mWind;                 // The wind force
        bool mHasWind;                   // Control for the wind step
        glm::vec3 mGravity;              // The grabity acting on the cloth
        bool mHasGravity;                // Control the step gravity

        // Returns a node at XY position
        cNode* getNode(int x, int y) { return &mNodes[y * mNumNodesWidth + x]; }

        // Make a spring out of two nodes
        void makeSpring(cNode* n1, cNode* n2) { mSprings.push_back(cSpring(n1, n2)); }

        // Returns the normal of a triangles. Used for the shader and wind calcs
        glm::vec3 calcTriangleNormal(cNode* n1, cNode* n2, cNode* n3);

        // Calcualtes the wind force for a single triangle
        void addWindForcesForTriangle(cNode* n1, cNode* n2, cNode* n3, const glm::vec3 direction);

        // Update all cloth nodes with the set wind
        void stepWind(float deltaTime);

        // Update all cloth nodes with the set gravity
        void stepGravity(float deltaTime);
    };
}