#include "cSoundManager.h"
#include "cSoundManager_Imp.h"


cSoundManager::cSoundManager()
{
    // Create the actual implementation
    this->pImp = new cSoundManager_Imp();
}

cSoundManager::~cSoundManager()
{
    this->pImp->~cSoundManager_Imp();
}

void cSoundManager::loadConfigFile(std::string file)
{
    this->pImp->loadConfigFile(file);
}

void cSoundManager::loadSoundObjectsIntoScene()
{
    this->pImp->loadSoundObjectsIntoScene();
}

void cSoundManager::initSoundScene()
{
    this->pImp->initSoundScene();
}

void cSoundManager::updateSoundScene(glm::vec3 listener)
{
    this->pImp->updateSoundScene(listener);
}

bool cSoundManager::loadSoundParameters(std::string configFile)
{
    return this->pImp->loadSoundParameters(configFile);
}