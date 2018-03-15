#include "cCharacterControl.h"
#include "../cGameObject.h"
#include "cSkinnedMesh.h"
#include "cAnimationState.h"

cCharacterControl::cCharacterControl()
{
    mActiveCharacter = NULL;
}

bool cCharacterControl::SetControlledCharacter(std::string friendlyName, std::string& error)
{
    if (friendlyName == mActiveCharacterName)
        return true;

    std::map<std::string, cCharacterControl::sCharacterDetails>::iterator 
        it = mMapNameToCharacters.find(friendlyName);

    if (it == mMapNameToCharacters.end())
    {
        error = "The name of the character was not found!";
        return false;
    }

    // Init all animations
    cGameObject* GO = it->second.GO;

    // Make sure the GameObject has their animations ready
    if (!GO->InitCharacterAnimations(error))
    {
        return false;
    }

    mActiveCharacter = &it->second;
    mActiveCharacterName = friendlyName;
    return true;
}

bool cCharacterControl::AddCharacter(cGameObject* GO, std::string& error)
{
    if (GO->friendlyName == "")
    {
        error = "The Character Gameobject number is missing its friendly name!";
        return false;
    }

    cCharacterControl::sCharacterDetails ch;
    ch.GO = GO;

    mMapNameToCharacters[GO->friendlyName] = ch;
    return true;
}

std::vector<cGameObject*> cCharacterControl::GetNPCs()
{
    std::vector<cGameObject*> npcs;

    std::map<std::string, cCharacterControl::sCharacterDetails>::iterator it = mMapNameToCharacters.begin();

    for (; it != mMapNameToCharacters.end(); it++)
    {
        if (it->second.GO != mActiveCharacter->GO)
        {
            npcs.push_back(it->second.GO);
        }
    }

    return npcs;
}

void cCharacterControl::Forward()
{

    if (mActiveCharacter->GO != NULL)
    {
        if (mActiveCharacter->mActiveState == FORWARD)
            return;

        // Don't cut the jump
        if (mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump_forward)
        {
            // Reorient the character
            glm::mat4 matOrientation;
            mActiveCharacter->GO->rigidBody->GetMatOrientation(matOrientation);
            matOrientation *= mActiveCharacter->GO->pSimpleSkinnedMesh->mLastHipRotation;

            mActiveCharacter->GO->rigidBody->SetMatOrientation(matOrientation);

            std::string animationName = mActiveCharacter->GO->animations.walking;
            mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

            mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

            mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
                mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mActiveCharacter->GO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));

            mActiveCharacter->mActiveState = FORWARD;
        }
    }
}

void cCharacterControl::ForwardRun()
{
    if (mActiveCharacter->GO != NULL)
    {// Don't cut the jump
        if (mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump_forward)
        {
            std::string animationName = mActiveCharacter->GO->animations.running;
            mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

            mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

            mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
                mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mActiveCharacter->GO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 3.75f));
        }
    }
}

void cCharacterControl::Backwards()
{
    if (mActiveCharacter->GO != NULL)
    {
        std::string animationName = mActiveCharacter->GO->animations.walking_backwards;
        mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
            mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mActiveCharacter->GO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -1.125f));
    }
}

void cCharacterControl::TurnLeft90()
{
    if (mActiveCharacter->GO != NULL)
    {
        std::string animationName = mActiveCharacter->GO->animations.left_turn_90;
        mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
            mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::TurnRight90()
{
    if (mActiveCharacter->GO != NULL)
    {
        std::string animationName = mActiveCharacter->GO->animations.right_turn_90;
        mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
            mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::Jump()
{
    if (mActiveCharacter->GO != NULL)
    {// Don't cut the jump
        if (mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump &&
            mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump_forward)
        {
            std::string animationName = mActiveCharacter->GO->animations.jump;
            mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

            mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

            mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
                mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        }
    }
}

void cCharacterControl::ForwardJump()
{
    if (mActiveCharacter->GO != NULL)
    {// Don't cut the jump
        if (mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump_forward)
        {
            std::string animationName = mActiveCharacter->GO->animations.jump_forward;
            mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

            mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

            mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
                mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
        }
    }
}

void cCharacterControl::Idle()
{
    if (mActiveCharacter->GO != NULL)
    {
        // Don't cut the jump
        if (mActiveCharacter->GO->pAniState->activeAnimation.name != mActiveCharacter->GO->animations.jump_forward)
        {
            // Reorient the character
            glm::mat4 matOrientation;
            mActiveCharacter->GO->rigidBody->GetMatOrientation(matOrientation);
            matOrientation *= mActiveCharacter->GO->pSimpleSkinnedMesh->mLastHipRotation;

            mActiveCharacter->GO->rigidBody->SetMatOrientation(matOrientation);

            std::string animationName = mActiveCharacter->GO->animations.idle;
            mActiveCharacter->GO->pAniState->activeAnimation.name = animationName;

            mActiveCharacter->GO->pAniState->activeAnimation.currentTime = 0.0f;

            mActiveCharacter->GO->pAniState->activeAnimation.totalTime =
                mActiveCharacter->GO->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

            mActiveCharacter->GO->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }
}
