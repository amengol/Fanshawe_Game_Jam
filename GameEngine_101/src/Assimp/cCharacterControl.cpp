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

    std::map<std::string, cGameObject*>::iterator it = mMapNameToCharacters.find(friendlyName);

    if (it == mMapNameToCharacters.end())
    {
        error = "The name of the character was not found!";
        return false;
    }

    // Init all animations
    cGameObject* GO = it->second;

    // Make sure the GameObject has their animations ready
    if (!GO->InitCharacterAnimations(error))
    {
        return false;
    }

    mActiveCharacter = GO;
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

    mMapNameToCharacters[GO->friendlyName] = GO;
    return true;
}

void cCharacterControl::Forward()
{
    if (mActiveCharacter != NULL)
    {
        // Rotate the start position
        glm::vec3 rotatedStartPos =
            mActiveCharacter->orientation * glm::vec4(mActiveCharacter->pSimpleSkinnedMesh->mLastHipPosition, 0.0f);
        mActiveCharacter->position += rotatedStartPos * mActiveCharacter->scale;

        // Project the root to the ground level
        mActiveCharacter->position.y = 0.0f;

        // Reorient the character
        mActiveCharacter->orientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        mActiveCharacter->orientation[0].y = 0.0f;
        mActiveCharacter->orientation[1].x = 0.0f;
        mActiveCharacter->orientation[1].y = 1.0f;
        mActiveCharacter->orientation[1].z = 0.0f;
        mActiveCharacter->orientation[2].y = 0.0f;

        std::string animationName = mActiveCharacter->animations.walking;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::ForwardLeft()
{
    if (mActiveCharacter != NULL)
    {
        // Rotate the start position
        glm::vec3 rotatedStartPos =
            mActiveCharacter->orientation * glm::vec4(mActiveCharacter->pSimpleSkinnedMesh->mLastHipPosition, 0.0f);
        mActiveCharacter->position += rotatedStartPos * mActiveCharacter->scale;

        // Project the root to the ground level
        mActiveCharacter->position.y = 0.0f;

        // Reorient the character
        mActiveCharacter->orientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        mActiveCharacter->orientation[0].y = 0.0f;
        mActiveCharacter->orientation[1].x = 0.0f;
        mActiveCharacter->orientation[1].y = 1.0f;
        mActiveCharacter->orientation[1].z = 0.0f;
        mActiveCharacter->orientation[2].y = 0.0f;

        std::string animationName = mActiveCharacter->animations.walking_arc_left;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::ForwardRight()
{
    if (mActiveCharacter != NULL)
    {
        // Rotate the start position
        glm::vec3 rotatedStartPos =
            mActiveCharacter->orientation * glm::vec4(mActiveCharacter->pSimpleSkinnedMesh->mLastHipPosition, 0.0f);
        mActiveCharacter->position += rotatedStartPos * mActiveCharacter->scale;

        // Project the root to the ground level
        mActiveCharacter->position.y = 0.0f;

        // Reorient the character
        mActiveCharacter->orientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        mActiveCharacter->orientation[0].y = 0.0f;
        mActiveCharacter->orientation[1].x = 0.0f;
        mActiveCharacter->orientation[1].y = 1.0f;
        mActiveCharacter->orientation[1].z = 0.0f;
        mActiveCharacter->orientation[2].y = 0.0f;

        std::string animationName = mActiveCharacter->animations.walking_arc_right;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::Backwards()
{
    if (mActiveCharacter != NULL)
    {
        std::string animationName = mActiveCharacter->animations.walking_backwards;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::TurnLeft90()
{
    if (mActiveCharacter != NULL)
    {
        std::string animationName = mActiveCharacter->animations.left_turn_90;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::TurnRight90()
{
    if (mActiveCharacter != NULL)
    {
        std::string animationName = mActiveCharacter->animations.right_turn_90;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}

void cCharacterControl::Idle()
{
    if (mActiveCharacter != NULL)
    {
        // Rotate the start position
        glm::vec3 rotatedStartPos =
            mActiveCharacter->orientation * glm::vec4(mActiveCharacter->pSimpleSkinnedMesh->mLastHipPosition, 0.0f);
        mActiveCharacter->position += rotatedStartPos * mActiveCharacter->scale;

        // Project the root to the ground level
        mActiveCharacter->position.y = 0.0f;

        // Reorient the character
        mActiveCharacter->orientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        mActiveCharacter->orientation[0].y = 0.0f;
        mActiveCharacter->orientation[1].x = 0.0f;
        mActiveCharacter->orientation[1].y = 1.0f;
        mActiveCharacter->orientation[1].z = 0.0f;
        mActiveCharacter->orientation[2].y = 0.0f;

        std::string animationName = mActiveCharacter->animations.idle;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);
    }
}
