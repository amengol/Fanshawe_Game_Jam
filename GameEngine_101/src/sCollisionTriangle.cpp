#include "sCollisionTriangle.h"

sCollisionTriangle::sCollisionTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    this->verticeA = A;
    this->verticeB = B;
    this->verticeC = C;

    // We suppose that in the mesh, each 
    // triangle will have a unique Centroid
    float centroidX = (A.x + B.x + C.x) / 3;
    float centroidY = (A.y + B.y + C.y) / 3;
    float centroidZ = (A.z + B.z + C.z) / 3;

    this->Centroid = glm::vec3(centroidX, centroidY, centroidZ);
}

void sCollisionTriangle::setTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    this->verticeA = A;
    this->verticeB = B;
    this->verticeC = C;

    // We suppose that in the mesh, each 
    // triangle will have a unique Centroid
    float centroidX = (A.x + B.x + C.x) / 3;
    float centroidY = (A.y + B.y + C.y) / 3;
    float centroidZ = (A.z + B.z + C.z) / 3;

    this->Centroid = glm::vec3(centroidX, centroidY, centroidZ);

}
