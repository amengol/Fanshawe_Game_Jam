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

    // Returns the active character
    inline cGameObject* GetActiveCharacter() { return mActiveCharacter; }

    // Moves character forward
    void Forward();

    // Runs forward
    void ForwardRun();

    // Moves character backwords
    void Backwards();

    // Turns character left 90 degrees
    void TurnLeft90();

    // Turns character right 90 degrees
    void TurnRight90();

    // Jumps in place
    void Jump();

    // Jumps forward
    void ForwardJump();

    // Reverts back to idle;
    void Idle();

private:
    cGameObject* mActiveCharacter;                              // The controlled character
    std::string mActiveCharacterName;                           // Stores the name of the active character
    std::map<std::string, cGameObject*> mMapNameToCharacters;   // Maps friendly names to characters
};

