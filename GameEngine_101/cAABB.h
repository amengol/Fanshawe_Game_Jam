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
    
    glm::vec3 min;      // The minimum XYZ point of the bouding box
    glm::vec3 max;      // The maximum XYZ point of the bouding box    
    long long int ID;   // sAABB ID

};


#endif // !_cAABB_HG_

