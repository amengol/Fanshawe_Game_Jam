#pragma once
#include <vector>

class cGameObject;
class cCharacterControl;

class cNPCManager
{
public:
    cNPCManager();
    ~cNPCManager();

    // Sets the player character
    void SetPlayer(cGameObject* GO) { mPlayer = GO; }

    // Sets the NPCs
    void SetNPCs(std::vector<cGameObject*> vecGO) { mNPCs = vecGO; }

    // Evaluate the scene
    void Evaluate();

private:
    cGameObject* mPlayer;
    std::vector<cGameObject*> mNPCs;
};

