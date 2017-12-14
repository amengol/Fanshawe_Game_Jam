#include "sAABB_Triangle.h"

sAABB_Triangle::sAABB_Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    this->verticeA = A;
    this->verticeB = B;
    this->verticeC = C;
}

void sAABB_Triangle::setTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    this->verticeA = A;
    this->verticeB = B;
    this->verticeC = C;
}
