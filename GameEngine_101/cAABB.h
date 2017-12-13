#ifndef _cAABB_HG_
#define _cAABB_HG_

#include "globalGameStuff.h"
#include "sAABB_Triangle.h"

class cAABB
{
public:
    cAABB(long long int ID, float diameter);
    ~cAABB();

    // The triangles stored for the AABB
    std::vector<sAABB_Triangle*> AABBsTriangles;

private:
    // MinXYZ and Diameter
    glm::vec3 min;
    float diameter;

    // sAABB ID
    long long int ID;

};


#endif // !_cAABB_HG_

