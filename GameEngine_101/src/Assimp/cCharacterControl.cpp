#include "cCharacterControl.h"

bool cCharacterControl::SetControlledCharacter(std::string friendlyName, std::string& error)
{
    std::map<std::string, cGameObject*>::iterator it = mMapNameToCharacters.find(friendlyName);

    if (it == mMapNameToCharacters.end())
    {
        error = "The name of the character was not found!";
        return false;
    }

    // Init all animations
    cGameObject* GO = it->second;

    // Make sure the GameObject has their animations ready
    if (!GO->InitCharacterAnimations(error))
    {
        return false;
    }

    mControlledCharacter = GO;
    return true;
}

bool cCharacterControl::AddCharacter(cGameObject* GO, std::string& error)
{
    if (GO->friendlyName == "")
    {
        error = "The Character Gameobject number is missing its friendly name!";
        return false;
    }

    mMapNameToCharacters[GO->friendlyName] = GO;
    return true;
}
