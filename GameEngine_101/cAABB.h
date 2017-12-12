#ifndef _cAABB_HG_
#define _cAABB_HG_

#include "globalGameStuff.h"
#include "sAABB_Triangle.h"

class cAABB
{
public:
    cAABB(long long int ID);
    ~cAABB();

    // The triangles stored for the AABB
    std::vector<sAABB_Triangle*> AABBsTriangles;

private:
    // MinXYZ and MaxXYZ
    glm::vec3 min;
    glm::vec3 max;

    // sAABB ID
    long long int ID;

};

cAABB::cAABB(long long int ID)
{
}

cAABB::~cAABB()
{
}

#endif // !_cAABB_HG_

