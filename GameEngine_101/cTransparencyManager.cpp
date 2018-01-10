#include "cTransparencyManager.h"
#include <time.h>

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

void cTransparencyManager::createRamdomObjects(int numOfObjects,
                                               std::vector<transparencyInfo> info,
                                               float minX, float maxX,
                                               float minY, float maxY,
                                               float minZ, float maxZ)
{
    srand(time(NULL));

    for(int i = 0; i < numOfObjects; i++)
    {
        cGameObject* theGO = new cGameObject();

        // Pic one object
        
        int objIndex = std::rand() % info.size();

        theGO->meshName = info[objIndex].meshName;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = info[objIndex].texture;
        theGO->textureBlend[1] = 0.0f;
        theGO->textureNames[1] = info[objIndex].alpha;
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->hasAlpha = true;
        theGO->useDiscardAlpha = false;
        theGO->cullFace = false;
        theGO->typeOfObject = TERRAIN;

        // Calculate a random position
        int rangeX = maxX - minX;
        int x = (std::rand() % rangeX) + minX;
        int rangeY = maxY - minY;
        int y = (std::rand() % rangeY) + minY;
        int rangeZ = maxZ - minZ;
        int z = (std::rand() % rangeZ) + minZ;

        theGO->position = glm::vec3((float)x, (float)y, (float)z);      


        this->transpObjects.push_back(theGO);
    }

    return;
}
