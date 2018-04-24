/**
    Controls NPCs
    
    @author Jorge Amengol
    @version 1.2
    @date April 24th, 2018
*/
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
    float mInterestRadius;                  // The interest radius
    float mStopDistance;                    // The distance where the player should stop
    float mThreshold;                       // Distance to avoid staggering
    float mSysTimeDying;                    // Control for dying
    bool mIsDying;                          // Control for dying
    cCharacterControl* mPlayer;             // The player
    std::vector<cCharacterControl*> mNPCs;  // The NPCs

    // Solve the NPC for Guardian
    void SolveForGuardian(cCharacterControl*, double deltaTime);

    // Solve the NPC for Angry Pursuit
    void SolveForAngryPursuit(cCharacterControl*, double deltaTime);

    // Solve the NPC for Dying
    void SolveForDying(cCharacterControl*, double deltaTime);
};

