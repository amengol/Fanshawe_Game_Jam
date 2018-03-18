#include "cCharacterManager.h"
#include "cCharacterControl.h"
#include "..\cGameObject.h"


cCharacterManager::cCharacterManager()
{
    mActiveCharacter = NULL;
}


cCharacterManager::~cCharacterManager()
{
}

bool cCharacterManager::AddCharacter(cGameObject* GO, std::string& error)
{
    if (GO->friendlyName == "")
    {
        error = "The Character Gameobject number is missing its friendly name!";
        return false;
    }

    cCharacterControl* ch = new cCharacterControl();
    ch->SetCharacter(GO);

    mMapNameToCharacters[GO->friendlyName] = ch;
    return true;
}

bool cCharacterManager::SetControlledCharacter(std::string friendlyName, std::string & error)
{
        if (friendlyName == mActiveCharacterName)
            return true;
    
        std::map<std::string, cCharacterControl*>::iterator 
            it = mMapNameToCharacters.find(friendlyName);
    
        if (it == mMapNameToCharacters.end())
        {
            error = "The name of the character was not found!";
            return false;
        }
    
        // Init all animations
        cGameObject* GO = it->second->GetCharacter();
    
        // Make sure the GameObject has their animations ready
        if (!GO->InitCharacterAnimations(error))
        {
            return false;
        }
    
        mActiveCharacter = it->second;
        mActiveCharacterName = friendlyName;
        return true;
}

std::vector<cCharacterControl*> cCharacterManager::GetNPCs()
{
        std::vector<cCharacterControl*> npcs;
    
        std::map<std::string, cCharacterControl*>::iterator it = mMapNameToCharacters.begin();
    
        for (; it != mMapNameToCharacters.end(); it++)
        {
            if (it->second != mActiveCharacter)
            {
                // HACK for now
                if (it->second->GetCharacter()->friendlyName == "Hero01"
                    || it->second->GetCharacter()->friendlyName == "Hero02")
                    continue;
                npcs.push_back(it->second);
            }
        }
    
        return npcs;
}
