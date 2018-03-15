#include "cNPCManager.h"
#include "../globalGameStuff.h"


cNPCManager::cNPCManager()
{
}


cNPCManager::~cNPCManager()
{
}

void cNPCManager::Evaluate()
{
    std::string error;
    for (size_t i = 0; i < mNPCs.size(); i++)
    {
        g_characterControl.SetControlledCharacter(mNPCs[i]->friendlyName, error);
        g_characterControl.Forward();
    }
}
