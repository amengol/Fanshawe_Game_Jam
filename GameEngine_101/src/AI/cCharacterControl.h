/**
    Controls characters
    
    @author Jorge Amengol
    @version 1.5
    @date April 25th, 2018
*/
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
        GUARDIAN,
        ANGRY_PURSUIT,
        DYING
    };

    // Sets the Character's GameObject
    void SetCharacter(cGameObject* GO);

    // Returns the Character's GameObject
    inline cGameObject* GetCharacter() { return mCharacter; }

    // Returns the last matrix orientation
    glm::mat4 getLastOrientation() { return mLastOrientation; }

    // Sets the last matrix orientation
    void setLastOrientation(const glm::mat4& orienation) { mLastOrientation = orienation; }

    // Moves character forward
    void Forward();

    // Moves character forward
    void Forward(glm::vec3 direction);

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

    // Moves character backwords
    void Backwards(glm::vec3 direction);

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

    // Attack 01
    void attack_01();

    // Attack 02
    void attack_02();

    // Attack 03
    void attack_03();

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
    glm::mat4 mLastOrientation; // Last character orientation;
    cGameObject* mCharacter;    // The controlled character
    eCharacterState mCharState; // The state of the character
    
    // Update orientation on interrupted rotations
    void UpdateInterruptedRotations();

    
};

