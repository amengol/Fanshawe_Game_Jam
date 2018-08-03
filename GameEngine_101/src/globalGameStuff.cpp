#include "globalGameStuff.h"

cGameObject* FindObjectByFriendlyName(const std::string friendlyName, std::vector<cGameObject*>& gameObjects)
{
    // Linear search for now!
    const unsigned int numObjects = static_cast<unsigned int>(gameObjects.size());
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (gameObjects[index]->friendlyName == friendlyName)
        {
            return gameObjects[index];
        }
    }
    // Didn't find it
    return nullptr;
}

cGameObject* FindObjectByUniqueId(const unsigned int id, std::vector<cGameObject*> &gameObjects)
{
    // Linear search for now!
    const unsigned int numObjects = static_cast<unsigned int>(gameObjects.size());
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (id == gameObjects[index]->getUniqueID())
        {
            return gameObjects[index];
        }
    }
    // Didn't find it
    return nullptr;
}