#include "cAABB.h"

cAABB::cAABB(long long int ID, float diameter)
{
    // Let's decompose the long long
    // The ID should be 18 digits long in the format
    // ?xxxxx?yyyyy?zzzzz (? can be 1 or 0 for negative and posite, respectively)

    const int oneBi = 1000000;                  // one Billion
    const long long oneBiSq = oneBi * oneBi;    // one Billion Squared

    // For X we want the first 6 digits (Left to Ritgh)
    int x = floor(ID / oneBiSq);

    // Check if it is "negative"
    if (x > oneBi)
    {
        this->min.x = -x;
    }
    else
    {
        this->min.x = x;
    }


    // For Y we want the 6 in-between digits
    long long xyDigits = floor(ID / oneBi) * oneBi;
    int y = (xyDigits - (x * oneBiSq)) / oneBi;

    // Check if it is "negative"
    if (y > oneBi)
    {
        this->min.y = -y;
    }
    else
    {
        this->min.y = y;
    }

    // For Z we want the last 6 digits
    int z = ID - xyDigits;

    // Check if it is "negative"
    if (z > oneBi)
    {
        this->min.z = -z;
    }
    else
    {
        this->min.z = z;
    }

    // Now the maximum point
    this->max = this->min + diameter;


}

cAABB::~cAABB()
{
}