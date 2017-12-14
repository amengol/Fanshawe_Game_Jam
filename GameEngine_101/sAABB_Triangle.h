#ifndef _sAABB_Triangle_HG_
#define _sAABB_Triangle_HG_

#include "globalGameStuff.h"

struct sAABB_Triangle
{
    sAABB_Triangle() : verticeA(0.0f), verticeB(0.0f), verticeC(0.0f) {}
    sAABB_Triangle(glm::vec3, glm::vec3, glm::vec3);

    void setTriangle(glm::vec3, glm::vec3, glm::vec3);

    glm::vec3 verticeA;
    glm::vec3 verticeB;
    glm::vec3 verticeC;
};

#endif // !_sAABB_Triangle_HG_

