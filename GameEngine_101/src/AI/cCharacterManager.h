#pragma once
#include <string>
#include <map>
#include <vector>

class cCharacterControl;
class cGameObject;

class cCharacterManager
{
public:
    cCharacterManager();
    ~cCharacterManager();

    // Adds a character to the Character Manager
    bool AddCharacter(cCharacterControl* cc, std::string& error);

    // Sets the controlled Character
    bool SetControlledCharacter(std::string friendlyName, std::string& error);

    // Returns the active character
    inline cCharacterControl* GetActiveCharacter() { return mActiveCharacter; }

    // Returns all NPCs (non active characters)
    std::vector<cCharacterControl*> GetNPCs();

private:
    cCharacterControl* mActiveCharacter;                            // The controlled character
    std::string mActiveCharacterName;                               // Stores the name of the active character
    std::map<std::string, cCharacterControl*> mMapNameToCharacters; // Maps friendly names to characters
};

