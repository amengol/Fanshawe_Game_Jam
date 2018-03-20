#pragma once
#include <map>
#include <vector>
#include "..\Assimp\animHelper.h"

class cGameObject;

class cCharacterControl
{
public:
    cCharacterControl();

    enum eCharacterState
    {
        FOLLOWER,
        CURIOUS_APPROACH,
        CURIOUS_EVADE,
        ANGRY_PURSUIT,
        ANGRY_EVADE,
        BAGGING,
        DYING
    };

    // Sets the Character's GameObject
    inline void SetCharacter(cGameObject* GO) { mCharacter = GO; }

    // Returns the Character's GameObject
    inline cGameObject* GetCharacter() { return mCharacter; }

    // Moves character forward
    void Forward();

    // Strafes character to the left
    void LeftStrafe();

    // Strafes character to the right
    void RightStrafe();

    // Runs forward
    void ForwardRun();

    // Strafes character to the left running
    void LeftStrafeRun();

    // Strafes character to the right running
    void RightStrafeRun();

    // Moves character backwords
    void Backwards();

    // Turns character left 90 degrees
    void TurnLeft90();

    // Turns character right 90 degrees
    void TurnRight90();

    // Turns character left 180 degrees
    void TurnLeft180();

    // Turns character right 180 degrees
    void TurnRight180();

    // Jumps in place
    void Jump();

    // Jumps forward
    void ForwardJumpWalking();

    // Jumps forward
    void ForwardJump();

    // Reverts back to idle;
    void Idle();

    // Starts the trick animation;
    void Trick();

    // Starts the violent trick animation
    void ViolentTrick();

    // Runaway run
    void Runaway();

    // Right Cross Punch
    void LeftCrossPunch();

    // Right Cross Punch
    void RightCrossPunch();

    // Right Kicking
    void RightKicking();

    // Praying
    void Praying();

    // Dying
    void Dying();

    // Stunned
    void Stunned();

    // Returns the Character State
    inline eCharacterState GetCharacterState() { return mCharState; }

    // Sets the Character State
    inline void SetCharacterState(eCharacterState cs) { mCharState = cs; }

    // Returns the Character's Animation State
    eCharacterAnim GetAnimationState();

    // Returns the health
    inline float GetHealth() { return mHealth; }

    // Sets the health to 100%
    inline void Health100() { mHealth = 1.0f; }

    // Decrease health
    void Hurt(float amount);

private:
    float mHealth;              // The health of the character
    float mSysTimeRot;          // To control "interrupted" rotations    
    float mSysTimeJump;         // To avoid jumping in the air
    cGameObject* mCharacter;    // The controlled character
    eCharacterState mCharState; // The state of the character
    
    // Update orientation on interrupted rotations
    void UpdateInterruptedRotations();

    
};

