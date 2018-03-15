#pragma once
#include <vector>

class cCharacterControl;

class cNPCManager
{
public:
    cNPCManager();
    ~cNPCManager();

    // Sets the player character
    void SetPlayer(cCharacterControl* CH) { mPlayer = CH; }

    // Sets the NPCs
    void SetNPCs(std::vector<cCharacterControl*> vecCH) { mNPCs = vecCH; }

    // Evaluate the scene
    void Evaluate();

private:
    cCharacterControl * mPlayer;
    std::vector<cCharacterControl*> mNPCs;
};

