#include "cCharacterControl.h"
#include "..\cGameObject.h"
#include "..\Assimp\cSkinnedMesh.h"
#include "..\Assimp\cAnimationState.h"

cCharacterControl::cCharacterControl()
{
    mCharacter = NULL;
    mAnimState = IDLE;
    mCharState = FOLLOWER;
    health = 1.0f;
}

void cCharacterControl::Forward()
{

    if (mCharacter != NULL)
    {
        if (mAnimState != WALKING)
        {
            // Don't cut the jump
            if (mCharacter->pAniState->activeAnimation.name != mCharacter->animations.jump_forward)
            {

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
}

void cCharacterControl::ForwardRun()
{
    if (mCharacter != NULL)
    {
        if (mAnimState != RUN_FORWARD && mAnimState != JUMP_FORWARD)
        {
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
        std::string animationName = mCharacter->animations.walking_backwards;
        mCharacter->pAniState->activeAnimation.name = animationName;

        mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mCharacter->pAniState->activeAnimation.totalTime =
            mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
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
    }
}

void cCharacterControl::Jump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mCharacter->pAniState->activeAnimation.name != mCharacter->animations.jump &&
            mCharacter->pAniState->activeAnimation.name != mCharacter->animations.jump_forward)
        {
            std::string animationName = mCharacter->animations.jump;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        }
    }
}

void cCharacterControl::ForwardJump()
{
    if (mCharacter != NULL)
    {// Don't cut the jump
        if (mCharacter->pAniState->activeAnimation.name != mCharacter->animations.jump_forward)
        {
            std::string animationName = mCharacter->animations.jump_forward;
            mCharacter->pAniState->activeAnimation.name = animationName;

            mCharacter->pAniState->activeAnimation.currentTime = 0.0f;

            mCharacter->pAniState->activeAnimation.totalTime =
                mCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        }
    }
}

void cCharacterControl::Idle()
{
    if (mCharacter != NULL)
    {
        // Don't cut the jump
        if (mCharacter->pAniState->activeAnimation.name != mCharacter->animations.jump_forward)
        {

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
