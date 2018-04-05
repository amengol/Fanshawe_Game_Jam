#include "cTransparencyManager.h"
#include <algorithm>
#include "cCameraObject.h"
#include "cGameObject.h"
#include "Utilities.h"
#include "globalGameStuff.h"

bool compare(cGameObject* i, cGameObject* j);

// This is the actual threading function
DWORD WINAPI SortClouds(void* pInitialData)	// CreateThread() format
{
    std::vector<cGameObject*>* transpObjects = (std::vector<cGameObject*>*)pInitialData;
    
    while (true)
    {
        std::sort(transpObjects->begin(), transpObjects->end(), compare);

        for (size_t i = 0; i < transpObjects->size(); i++)
        {
            cGameObject* cloud = transpObjects->at(i);
            turnGameObjectToCamera(cloud, g_camera.m_position);
        }

        Sleep(2000);
    }

    return 0;
}

cTransparencyManager::cTransparencyManager()
{
    return;
}

cTransparencyManager::~cTransparencyManager()
{
    //for(int i = 0; i < this->transpObjects.size(); i++)
    //{
    //    cGameObject* theGO = this->transpObjects[i];
    //    delete theGO;
    //}
}



void cTransparencyManager::sortObjects()
{
    
    std::sort(this->transpObjects.begin(), this->transpObjects.end(), compare);
}

void cTransparencyManager::InitSortingThread()
{
    // Create the thread
    threadInfo.handle = CreateThread(NULL,
                                     0,
                                     SortClouds,
                                     (void*)&transpObjects,
                                     0,
                                     &(threadInfo.address));
}

bool compare(cGameObject* i, cGameObject* j)
{
    if(glm::length(i->position - g_camera.m_position)
    > glm::length(j->position - g_camera.m_position))
        return true;
    else
        return false;
}