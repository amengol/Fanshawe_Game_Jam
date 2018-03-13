#pragma once
#include <map>

class cGameObject;

class cCharacterControl
{
public:
    // Sets the controlled Character
    bool SetControlledCharacter(std::string friendlyName, std::string& error);

    // Adds a character to the Character Control
    bool AddCharacter(cGameObject* GO, std::string& error);

private:
    std::string mActiveCharacter;                               // Stores the name of the active character
    cGameObject* mControlledCharacter;                          // The controlled character
    std::map<std::string, cGameObject*> mMapNameToCharacters;   // Maps friendly names to characters
};

