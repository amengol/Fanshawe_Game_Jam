#pragma once
#include <map>
#include "sAnimations.h"

class cGameObject;

class cCharacterControl
{
public:
    cCharacterControl();

    // Sets the controlled Character
    bool SetControlledCharacter(std::string friendlyName, std::string& error);

    // Adds a character to the Character Control
    bool AddCharacter(cGameObject* GO, std::string& error);

    // Moves character forward
    void Forward();

    // Moves character backwords
    void Backwards();

    // Turns character left 90 degrees
    void TurnLeft90();

    // Turns character right 90 degrees
    void TurnRight90();

    // Reverts back to idle;
    void Idle();

private:
    cGameObject* mActiveCharacter;                              // The controlled character
    std::string mActiveCharacterName;                           // Stores the name of the active character
    std::map<std::string, cGameObject*> mMapNameToCharacters;   // Maps friendly names to characters
};

