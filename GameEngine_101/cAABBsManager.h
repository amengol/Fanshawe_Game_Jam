#ifndef _cAABBsManager_HG_
#define _cAABBsManager_HG_

#include "cAABB.h"
#include "cMesh.h"
#include <map>

class sAABB_Triangle;

class cAABBsManager
{
public:
    cAABBsManager();
    ~cAABBsManager();

    // Generates AABBs from a mesh, respecting a size
    void genAABBs(cMesh* mesh, float size);

    // Calculates the longest side of a triangle
    float calcLongestSide(sAABB_Triangle* triangle);

    // Calculates an ID from a vertice
    long long calcID(glm::vec3 vertice);

    // Tesselates a triangle according to a minimum size
    // Set all tesselated triangles back in a vector
    // Returns false if no need to tesselate
    bool tesselate(sAABB_Triangle* triangle, float size, std::vector<sAABB_Triangle*>& tesseleted);

private:
   // std::vector<cAABB*> m_AABBs;                   // Stores all AABBs for the mesh
    std::map<long long, cAABB*> m_mapIDtoAABB;     // Map the IDs to the AABBs
};

#endif // !_cAABBsManager_HG_

