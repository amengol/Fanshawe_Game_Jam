#ifndef _cTransparencyManager_HG_
#define _cTransparencyManager_HG_

#include <vector>

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
    
    std::vector<cGameObject*> transpObjects;
};
#endif // !_cTransparencyManager_HG_

