#pragma once
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>

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
    void Evaluate(double deltaTime);

    // Returns a rotation quaternion between two vectors
    glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

private:
    cCharacterControl* mPlayer;
    std::vector<cCharacterControl*> mNPCs;
};

