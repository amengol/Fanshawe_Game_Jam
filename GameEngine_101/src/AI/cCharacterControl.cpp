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
    mSystemTime = 0.0f;
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

void cCharacterControl::Backwards()
{
    if (mCharacter != NULL)
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

        mSystemTime = glfwGetTime();
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

        mSystemTime = glfwGetTime();
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

        mSystemTime = glfwGetTime();
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

        mSystemTime = glfwGetTime();
    }
}

void cCharacterControl::Jump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mAnimState != JUMP)
        {
            std::string animationName = mCharacter->animations.jump;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));

            mAnimState = JUMP;
        }
    }
}

void cCharacterControl::ForwardJump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mAnimState != JUMP_FORWARD)
        {
            std::string animationName = mCharacter->animations.jump_forward;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mAnimState = JUMP_FORWARD;
        }
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

void cCharacterControl::UpdateInterruptedRotations()
{
    std::string animationName;
    float duration, deltaTime, ratio;

    if (mAnimState == TURN_LEFT_180)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSystemTime;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 180.0f, 180.0f));

        return;
    }

    if (mAnimState == TURN_RIGHT_180)
    {
        animationName = mCharacter->animations.right_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSystemTime;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * -180.0f, 180.0f));

        return;
    }

    if (mAnimState == TURN_LEFT_90)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSystemTime;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * 90.0f, 90.0f));

        return;
    }

    if (mAnimState == TURN_RIGHT_90)
    {
        animationName = mCharacter->animations.left_turn;
        duration =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        deltaTime = glfwGetTime() - mSystemTime;

        ratio = deltaTime / duration;

        mCharacter->rigidBody->rotateY(fmod(ratio * -90.0f, 90.0f));

        return;
    }
}
