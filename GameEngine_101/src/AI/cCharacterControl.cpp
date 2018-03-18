#include "cCharacterControl.h"
#include "..\cGameObject.h"
#include "..\Assimp\cSkinnedMesh.h"
#include "..\Assimp\cAnimationState.h"
#include <GLFW/glfw3.h>

cCharacterControl::cCharacterControl()
{
    mCharacter = NULL;
    mAnimState = IDLE;
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
        if (mAnimState != WALKING && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.walking;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));

            mAnimState = WALKING;
        }
    }
}

void cCharacterControl::LeftStrafe()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != LEFT_STRAFE && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.left_strafe;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(1.5f, 0.0f, 0.0f));

            mAnimState = LEFT_STRAFE;
        }
    }
}

void cCharacterControl::RightStrafe()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != RIGHT_STRAFE && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.right_strafe;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(-1.5f, 0.0f, 0.0f));

            mAnimState = RIGHT_STRAFE;
        }
    }
}

void cCharacterControl::ForwardRun()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != RUN_FORWARD && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));

            mAnimState = RUN_FORWARD;
        }
    }
}

void cCharacterControl::LeftStrafeRun()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != LEFT_STRAFE_RUN && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.left_strafe_running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(3.75f, 0.0f, 0.0f));

            mAnimState = LEFT_STRAFE_RUN;
        }
    }
}

void cCharacterControl::RightStrafeRun()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != RIGHT_STRAFE_RUN && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.right_strafe_running;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(-3.75f, 0.0f, 0.0f));

            mAnimState = RIGHT_STRAFE_RUN;
        }
    }
}

void cCharacterControl::Backwards()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != WALKING_BACKWARDS)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.walking_backwards;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));

            mAnimState = WALKING_BACKWARDS;
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

        mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mAnimState = TURN_LEFT_90;

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

        mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mAnimState = TURN_RIGHT_90;

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

        mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mAnimState = TURN_LEFT_180;

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

        mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

        mAnimState = TURN_RIGHT_180;

        mSysTimeRot = glfwGetTime();
    }
}

void cCharacterControl::Jump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mAnimState != JUMP_FORWARD)
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

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = JUMP;

            mSysTimeJump = glfwGetTime();
        }
    }
}

void cCharacterControl::ForwardJump()
{
    if (mCharacter != NULL)
    {
        // Avoid jumping in the air
        std::string animationName = mCharacter->animations.jump_forward;
        float duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        float deltaTime = glfwGetTime() - mSysTimeJump;
        if (deltaTime < duration)
            return;

        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mAnimState = JUMP_FORWARD;

        mSysTimeJump = glfwGetTime();
    }
}

void cCharacterControl::Idle()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != JUMP_FORWARD && mAnimState != IDLE)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.idle;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = IDLE;
        }
    }
}

void cCharacterControl::Trick()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.trick;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = TRICK;
        }
    }
}

void cCharacterControl::ViolentTrick()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.violent_trick;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = VIOLENT_TRICK;
        }
    }
}

void cCharacterControl::Runaway()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mAnimState != RUNAWAY && mAnimState != JUMP_FORWARD)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.runaway;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));

            mAnimState = RUNAWAY;
        }
    }
}

void cCharacterControl::RightCrossPunch()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != RIGHT_CROSS_PUNCH)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.right_cross_punch;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = RIGHT_CROSS_PUNCH;
        }
    }
}

void cCharacterControl::RightKicking()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != RIGHT_KICKING)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.right_kicking;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = RIGHT_KICKING;
        }
    }
}

void cCharacterControl::Praying()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != PRAYING)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.praying;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = PRAYING;
        }
    }
}

void cCharacterControl::Dying()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != DYING_RISING)
        {
            // Update rotations
            UpdateInterruptedRotations();

            std::string animationName = mCharacter->animations.dying;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = DYING_RISING;
        }
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

    if (mAnimState == TURN_LEFT_180)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 180.0f, 180.0f));

        return;
    }

    if (mAnimState == TURN_RIGHT_180)
    {
        animationName = mCharacter->animations.right_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * -180.0f, 180.0f));

        return;
    }

    if (mAnimState == TURN_LEFT_90)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSysTimeRot;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 90.0f, 90.0f));

        return;
    }

    if (mAnimState == TURN_RIGHT_90)
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
