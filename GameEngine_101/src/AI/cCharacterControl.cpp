#include "cCharacterControl.h"
#include "..\cGameObject.h"
#include "..\Assimp\cSkinnedMesh.h"
#include "..\Assimp\cAnimationState.h"
#include <GLFW/glfw3.h>
#include "..\Utilities.h"

cCharacterControl::cCharacterControl()
{
    mCharacter = NULL;
    mCharState = FOLLOWER;
    mHealth = 1.0f;
    mSysTimeRot = 0.0f;
    mSysTimeJump = 0.0f;
}

void cCharacterControl::Forward()
{

    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != WALKING 
            && mCharacter->characterAnim != JUMP_FORWARD_WALKING
            && mCharacter->characterAnim != JUMP_FORWARD)
        {
            // Update rotations
            //UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.walking;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->characterAnim = WALKING;
        }
    }
}

void cCharacterControl::Forward(glm::vec3 direction)
{

    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != WALKING && mCharacter->characterAnim != JUMP_FORWARD)
        {
            // Update rotations
            //UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.walking;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocity(direction * 1.5f);
            
            glm::mat4 orientation = getMatrixFromVector(direction);
            mCharacter->rigidBody->SetMatOrientation(orientation);

            mCharacter->rigidBody->SetRateOfTurnY(0.0f);

            mCharacter->characterAnim = WALKING;
        }
    }
}

void cCharacterControl::LeftStrafe()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != LEFT_STRAFE && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.left_strafe;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(1.5f, 0.0f, 0.0f));

            mCharacter->characterAnim = LEFT_STRAFE;
        }
    }
}

void cCharacterControl::RightStrafe()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != RIGHT_STRAFE && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.right_strafe;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(-1.5f, 0.0f, 0.0f));

            mCharacter->characterAnim = RIGHT_STRAFE;
        }
    }
}

void cCharacterControl::ForwardRun()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != RUN_FORWARD && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->characterAnim = RUN_FORWARD;
        }
    }
}

void cCharacterControl::LeftStrafeRun()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != LEFT_STRAFE_RUN && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.left_strafe_running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(3.75f, 0.0f, 0.0f));

            mCharacter->characterAnim = LEFT_STRAFE_RUN;
        }
    }
}

void cCharacterControl::RightStrafeRun()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != RIGHT_STRAFE_RUN && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.right_strafe_running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(-3.75f, 0.0f, 0.0f));

            mCharacter->characterAnim = RIGHT_STRAFE_RUN;
        }
    }
}

void cCharacterControl::Backwards()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != WALKING_BACKWARDS)
        {

            std::string animationName = mCharacter->animations.walking_backwards;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));

            mCharacter->characterAnim = WALKING_BACKWARDS;
        }
    }
}

void cCharacterControl::Backwards(glm::vec3 direction)
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != WALKING_BACKWARDS)
        {

            std::string animationName = mCharacter->animations.walking_backwards;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocity(direction * -1.125f);

            glm::mat4 orientation = getMatrixFromVector(direction);
            mCharacter->rigidBody->SetMatOrientation(orientation);

            mCharacter->rigidBody->SetRateOfTurnY(0.0f);

            mCharacter->characterAnim = WALKING_BACKWARDS;
        }
    }
}

void cCharacterControl::TurnLeft90()
{
    if (mCharacter != NULL)
    {
        std::string animationName = mCharacter->animations.left_turn_90;
        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
        mCharacter->rigidBody->SetRateOfTurnY(2880.0f);

        mCharacter->characterAnim = TURN_LEFT_90;

        mSysTimeRot = glfwGetTime();
    }
}

void cCharacterControl::TurnRight90()
{
    if (mCharacter != NULL)
    {
        std::string animationName = mCharacter->animations.right_turn_90;
        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
        mCharacter->rigidBody->SetRateOfTurnY(-2880.0f);

        mCharacter->characterAnim = TURN_RIGHT_90;

        mSysTimeRot = glfwGetTime();
    }
}

void cCharacterControl::TurnLeft180()
{
    if (mCharacter != NULL)
    {
        std::string animationName = mCharacter->animations.left_turn;
        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mCharacter->characterAnim = TURN_LEFT_180;

        mSysTimeRot = glfwGetTime();
    }
}

void cCharacterControl::TurnRight180()
{
    if (mCharacter != NULL)
    {
        std::string animationName = mCharacter->animations.right_turn;
        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mCharacter->characterAnim = TURN_RIGHT_180;

        mSysTimeRot = glfwGetTime();
    }
}

void cCharacterControl::Jump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mCharacter->characterAnim != JUMP_FORWARD && mCharacter->characterAnim != JUMP)
        {
            // Avoid jumping in the air
            std::string animationName = mCharacter->animations.jump;
            float duration =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
            float deltaTime = glfwGetTime() - mSysTimeJump;
            if (deltaTime < duration)
                return;

            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = JUMP;

            mSysTimeJump = glfwGetTime();
        }
    }
}

void cCharacterControl::ForwardJumpWalking()
{
    if (mCharacter != NULL)
    {
        // Avoid jumping in the air
        std::string animationName = mCharacter->animations.jump_forward_walking;
        float duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        float deltaTime = glfwGetTime() - mSysTimeJump;
        if (deltaTime < duration)
            return;

        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->characterAnim = JUMP_FORWARD_WALKING;

        mSysTimeJump = glfwGetTime();
    }
}

void cCharacterControl::ForwardJump()
{
    if (mCharacter != NULL)
    {
        // Avoid jumping in the air
        std::string animationName = mCharacter->animations.jump_forward_running;
        float duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        float deltaTime = glfwGetTime() - mSysTimeJump;
        if (deltaTime < duration)
            return;

        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->characterAnim = JUMP_FORWARD;

        mSysTimeJump = glfwGetTime();
    }
}

void cCharacterControl::Idle()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != IDLE 
            && mCharacter->characterAnim != JUMP
            && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.idle;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
            mCharacter->rigidBody->SetRateOfTurnY(0.0f);

            mCharacter->characterAnim = IDLE;
        }
    }
}

void cCharacterControl::Trick()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.trick;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = TRICK;
        }
    }
}

void cCharacterControl::ViolentTrick()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.violent_trick;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = VIOLENT_TRICK;
        }
    }
}

void cCharacterControl::Runaway()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->characterAnim != RUNAWAY && mCharacter->characterAnim != JUMP_FORWARD)
        {

            std::string animationName = mCharacter->animations.runaway;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));

            mCharacter->characterAnim = RUNAWAY;
        }
    }
}

void cCharacterControl::LeftCrossPunch()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != LEFT_CROSS_PUNCH 
            && mCharacter->characterAnim != RIGHT_CROSS_PUNCH
            && mCharacter->characterAnim != STUNNED)
        {

            std::string animationName = mCharacter->animations.left_cross_punch;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = LEFT_CROSS_PUNCH;
        }
    }
}

void cCharacterControl::RightCrossPunch()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != LEFT_CROSS_PUNCH
            && mCharacter->characterAnim != RIGHT_CROSS_PUNCH
            && mCharacter->characterAnim != STUNNED)
        {

            std::string animationName = mCharacter->animations.right_cross_punch;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = RIGHT_CROSS_PUNCH;
        }
    }
}

void cCharacterControl::RightKicking()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != RIGHT_KICKING)
        {

            std::string animationName = mCharacter->animations.right_kicking;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = RIGHT_KICKING;
        }
    }
}

void cCharacterControl::Praying()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != PRAYING)
        {

            std::string animationName = mCharacter->animations.praying;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = PRAYING;
        }
    }
}

void cCharacterControl::Dying()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != DYING_RISING)
        {

            std::string animationName = mCharacter->animations.dying;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = DYING_RISING;
        }
    }
}

void cCharacterControl::Stunned()
{
    if (mCharacter != NULL)
    {
        if (mCharacter->characterAnim != STUNNED)
        {

            std::string animationName = mCharacter->animations.stunned;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mCharacter->characterAnim = STUNNED;
        }
    }
}

eCharacterAnim cCharacterControl::GetAnimationState()
{
    if (mCharacter != NULL)
    {
        return mCharacter->characterAnim;
    }
}

void cCharacterControl::Hurt(float amount)
{
    if (amount < 0.0f)
        return;

    if (amount > mHealth)
    {
        mHealth = 0.0f;
        return;
    }

    mHealth -= amount;

}

void cCharacterControl::UpdateInterruptedRotations()
{
    std::string animationName;
    float duration, deltaTime, ratio;

    if (mCharacter->characterAnim == TURN_LEFT_180)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 180.0f, 180.0f));

        return;
    }

    if (mCharacter->characterAnim == TURN_RIGHT_180)
    {
        animationName = mCharacter->animations.right_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * -180.0f, 180.0f));

        return;
    }

    if (mCharacter->characterAnim == TURN_LEFT_90)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 90.0f, 90.0f));

        return;
    }

    if (mCharacter->characterAnim == TURN_RIGHT_90)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * -90.0f, 90.0f));

        return;
    }
}
