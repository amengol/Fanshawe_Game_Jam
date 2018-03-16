#pragma once
#include <map>
#include <vector>

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
        CURIOUS_OTHER,
        ANGRY_PURSUIT,
        ANGRY_EVADE,
        ANGRY_OTHER
    };

    enum eAnimationState
    {
        IDLE,
        WALKING,
        WALKING_BACKWARDS,
        RUN,
        RUN_FORWARD,
        TURN_LEFT_90,
        TURN_RIGHT_90,
        TURN_LEFT_180,
        TURN_RIGHT_180,
        JUMP,
        JUMP_FORWARD
    };

    // Sets the Character's GameObject
    inline void SetCharacter(cGameObject* GO) { mCharacter = GO; }

    // Returns the Character's GameObject
    inline cGameObject* GetCharacter() { return mCharacter; }

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

    // Turns character left 180 degrees
    void TurnLeft180();

    // Turns character right 180 degrees
    void TurnRight180();

    // Jumps in place
    void Jump();

    // Jumps forward
    void ForwardJump();

    // Reverts back to idle;
    void Idle();

    // Returns the Character State
    inline eCharacterState GetCharacterState() { return mCharState; }

    // Returns the Character's Animation State
    inline eAnimationState GetAnimationState() { return mAnimState; }

private:
    float mHealth;              // The health of the character
    float mSysTimeRot;          // To control "interrupted" rotations    
    float mSysTimeJump;         // To avoid jumping in the air
    cGameObject* mCharacter;    // The controlled character
    eCharacterState mCharState; // The state of the character
    eAnimationState mAnimState; // The animation state of the character
    
    // Update orientation on interrupted rotations
    void UpdateInterruptedRotations();
};

