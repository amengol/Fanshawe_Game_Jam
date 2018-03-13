#pragma once
#include "../cGameObject.h"
#include <map>

class cCharacterControl
{
public:
    // Sets the controlled Character
    bool SetControlledCharacter(std::string friendlyName, std::string& error);

    // Adds a character to the Character Control
    bool AddCharacter(cGameObject* GO, std::string& error);

private:
    cGameObject* mControlledCharacter;
    std::map<std::string, cGameObject*> mMapNameToCharacters;
};

