#include "cTransparencyManager.h"
#include <algorithm>
#include "cCameraObject.h"

extern cCameraObject* g_pCamera;

cTransparencyManager::cTransparencyManager()
{
    return;
}

cTransparencyManager::~cTransparencyManager()
{
    for(int i = 0; i < this->transpObjects.size(); i++)
    {
        cGameObject* theGO = this->transpObjects[i];
        delete theGO;
    }
}

bool compare(cGameObject* i, cGameObject* j);

void cTransparencyManager::sortObjects()
{
    
    std::sort(this->transpObjects.begin(), this->transpObjects.end(), compare);
}

bool compare(cGameObject* i, cGameObject* j)
{
    if(glm::length(i->position - g_pCamera->getCameraPosition())
    > glm::length(j->position - g_pCamera->getCameraPosition()))
        return true;
    else
        return false;
}