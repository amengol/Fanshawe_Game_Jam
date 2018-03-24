#ifndef _cTransparencyManager_HG_
#define _cTransparencyManager_HG_

#include <vector>
#include "sThreadInfo.h"

class cGameObject;

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

    void sortObjects();
    void InitSortingThread();
    
    std::vector<cGameObject*> transpObjects;
    sThreadInfo threadInfo;
};
#endif // !_cTransparencyManager_HG_

