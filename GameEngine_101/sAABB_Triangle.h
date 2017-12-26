#ifndef _sAABB_Triangle_HG_
#define _sAABB_Triangle_HG_

#include "globalGameStuff.h"

struct sAABB_Triangle
{
    sAABB_Triangle() : verticeA(0.0f), verticeB(0.0f), verticeC(0.0f), faceNormal(0.0f), Centroid(0.0f) {}
    sAABB_Triangle(glm::vec3, glm::vec3, glm::vec3);
    ~sAABB_Triangle() { return; };

    void setTriangle(glm::vec3, glm::vec3, glm::vec3);

    glm::vec3 verticeA;
    glm::vec3 verticeB;
    glm::vec3 verticeC;
    glm::vec3 faceNormal;

    // Triandle must have and ID too
    // the Centroind will do the job
    // For a mesh, we expect very different Centroids,
    // so to know if the Triangle is the same, we subtract
    // two centroid and expect an absolute difference 
    // no longer than 0.01f to cope with floating point errors
    glm::vec3 Centroid;
};

#endif // !_sAABB_Triangle_HG_

