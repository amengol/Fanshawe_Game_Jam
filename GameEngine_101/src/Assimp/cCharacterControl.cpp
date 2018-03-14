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
        // Reorient the character
        glm::mat4 matOrientation;
        mActiveCharacter->rigidBody->GetMatOrientation(matOrientation);
        matOrientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        matOrientation[0].y = 0.0f;
        matOrientation[1].x = 0.0f;
        matOrientation[1].y = 1.0f;
        matOrientation[1].z = 0.0f;
        matOrientation[2].y = 0.0f;

        mActiveCharacter->rigidBody->SetMatOrientation(matOrientation);

        std::string animationName = mActiveCharacter->animations.walking;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mActiveCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 4.0f));
    }
}

void cCharacterControl::ForwardRun()
{
    if (mActiveCharacter != NULL)
    {
        std::string animationName = mActiveCharacter->animations.running;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mActiveCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 10.0f));
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

        mActiveCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, -3.0f));
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
        // Reorient the character
        glm::mat4 matOrientation;
        mActiveCharacter->rigidBody->GetMatOrientation(matOrientation);
        matOrientation *= mActiveCharacter->pSimpleSkinnedMesh->mLastHipRotation;

        // Keep only the rotation in the Y axis
        matOrientation[0].y = 0.0f;
        matOrientation[1].x = 0.0f;
        matOrientation[1].y = 1.0f;
        matOrientation[1].z = 0.0f;
        matOrientation[2].y = 0.0f;

        mActiveCharacter->rigidBody->SetMatOrientation(matOrientation);

        std::string animationName = mActiveCharacter->animations.idle;
        mActiveCharacter->pAniState->activeAnimation.name = animationName;

        mActiveCharacter->pAniState->activeAnimation.currentTime = 0.0f;

        mActiveCharacter->pAniState->activeAnimation.totalTime =
            mActiveCharacter->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        mActiveCharacter->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
    }
}
