#ifndef _cTransparencyManager_HG_
#define _cTransparencyManager_HG_

#include <vector>
#include "cGameObject.h"

struct transparencyInfo
{
    std::string meshName;
    std::string texture;
    std::string alpha;
};

class cTransparencyManager
{
public:
    cTransparencyManager();
    ~cTransparencyManager();

    void createRamdomObjects(int numOfObjects,
                             std::vector<transparencyInfo> info,
                             float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ);
    
    std::vector<cGameObject*> transpObjects;
};
#endif // !_cTransparencyManager_HG_

