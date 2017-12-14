#include "cAABB.h"

cAABB::cAABB(long long int ID, float diameter)
{
    // Set the diameter
    this->diameter = diameter;

    // Let's decompose the long long
    // The ID should be 18 digits long in the format
    // ?xxxxx?yyyyy?zzzzz (? can be 1 or 0 for negative and posite, respectively)

    const unsigned long long oneBi = 1000000;                  // one Billion
    const unsigned long long oneBiSq = oneBi * oneBi;          // one Billion Squared

    // For X we want the first 6 digits (Left to Ritgh)
    int x = floor(ID / oneBiSq);

    // Check if it is "negative"
    if (x > 99999)
    {
        int theX = x - 100000;
        this->min.x = -theX;
    }
    else
    {
        this->min.x = x;
    }


    // For Y we want the 6 in-between digits
    unsigned long long xyDigits = floor(ID / oneBi) * oneBi;
    int y = floor((xyDigits - (x * oneBiSq)) / oneBi);

    // Check if it is "negative"
    if (y > 99999)
    {
        y -= 100000;
        this->min.y = -y;
    }
    else
    {
        this->min.y = y;
    }

    // For Z we want the last 6 digits
    int z = ID - xyDigits;

    // Check if it is "negative"
    if (z > 99999)
    {
        z -= 100000;
        this->min.z = -z;
    }
    else
    {
        this->min.z = z;
    }


}

cAABB::~cAABB()
{
}

glm::vec3 cAABB::getMinVertice()
{
    return this->min;
}

glm::vec3 cAABB::getMaxVertice()
{
    return this->min + 1.0f;
}

float cAABB::getDiameter()
{
    return this->diameter;
}
