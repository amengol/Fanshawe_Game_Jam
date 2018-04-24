#include "cNPCManager.h"
#include "cCharacterControl.h"
#include <glm\glm.hpp>
#include "..\cGameObject.h"
#include <glm\gtx\transform.hpp>
#include "..\Assimp\cSkinnedMesh.h"
#include <GLFW/glfw3.h>
#include "..\Assimp\cAnimationState.h"

cNPCManager::cNPCManager()
{
    mInterestRadius = 10.0f;
    mStopDistance = 2.0f;                  
    mThreshold = 2.0f;
    mSysTimeDying = 0.0f;
    mIsDying = false;
}


cNPCManager::~cNPCManager()
{
}

float extension = 35.0f;

void cNPCManager::Evaluate(double deltaTime)
{

    for (size_t i = 0; i < mNPCs.size(); i++)
    {
       
        switch (mNPCs[i]->GetCharacterState())
        {
        case cCharacterControl::eCharacterState::GUARDIAN:
            SolveForGuardian(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::ANGRY_PURSUIT:
            SolveForAngryPursuit(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::DYING:
            SolveForDying(mNPCs[i], deltaTime);
            break;
        default:
            break;
        }
    }
}

glm::quat cNPCManager::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
    start = glm::normalize(start);
    dest = glm::normalize(dest);

    float cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f)
    {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

void cNPCManager::SolveForGuardian(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    float distance = glm::length(playerPosition - npcPosition);

    // Case when the player is out of the interest radius
    if (distance > mInterestRadius && npc->GetAnimationState() != eCharacterAnim::IDLE)
    {
        npc->Idle();
        npc->GetCharacter()->rigidBody->SetLinearVelocity(glm::vec3(0.0f));
        return; // Out of the interest radius
    }
    else if (distance > mInterestRadius && npc->GetAnimationState() == eCharacterAnim::IDLE)
    {
        return; // Out of the interest radius
    }

    // Case when the player is too close
    if (distance < mStopDistance)
    {
        if (npc->GetAnimationState() != eCharacterAnim::IDLE)
        {
            npc->Idle();
            npc->GetCharacter()->rigidBody->SetLinearVelocity(glm::vec3(0.0f));
        }

        return;
    }
    
    // Check for enough space before proceed
    if (distance < mThreshold + mStopDistance && npc->GetAnimationState() == eCharacterAnim::IDLE)
    {
        return; // Give it some space to avoid staggering
    }
    else
    {
        // All other cases

        npc->Forward();

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetLinearVelocity(npcVelocity);

        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 1.5f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 2.0f; // Speed up the steering a litle bit

        npcVelocity += steering;
        npcVelocity = glm::normalize(npcVelocity) * 1.5f;

        npc->GetCharacter()->rigidBody->SetLinearVelocity(npcVelocity);

        // Reorient the npc to the velocity vector
        glm::mat4 npcOrientation;
        npc->GetCharacter()->rigidBody->GetMatOrientation(npcOrientation);
        glm::vec3 npcDirection = npcOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        npcDirection.y = 0.0f; // Get rid of any pitch information

        glm::quat qRot = RotationBetweenVectors(npcDirection, npcVelocity);
        npcOrientation *= glm::toMat4(qRot);
        npc->GetCharacter()->rigidBody->SetMatOrientation(npcOrientation);
    }
}

void cNPCManager::SolveForAngryPursuit(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    float distance = glm::length(playerPosition - npcPosition);

    // Case when the player is too close
    if (distance < mStopDistance * 0.5f)
    {
        npc->RightKicking();
        npc->GetCharacter()->rigidBody->SetLinearVelocity(glm::vec3(0.0f));

        if (mPlayer->GetAnimationState() == eCharacterAnim::RIGHT_KICKING ||
            mPlayer->GetAnimationState() == eCharacterAnim::RIGHT_CROSS_PUNCH)
        {
            npc->Hurt(deltaTime * 0.1f);

            float control = npc->GetHealth();

            if (npc->GetHealth() > 0.32f)
                npc->GetCharacter()->diffuseColour.g += deltaTime * 0.1f;
        }

        return;
    }

    // Check for enough space before proceed
    if (distance < mThreshold + mStopDistance * 0.5f && npc->GetAnimationState() == eCharacterAnim::RIGHT_KICKING)
    {
        return; // Give it some space to avoid staggering
    }
    else
    {
        // All other cases

        npc->ForwardRun();

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetLinearVelocity(npcVelocity);

        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 3.75f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 3.0f; // Speed up the steering a litle bit

        npcVelocity += steering;
        npcVelocity = glm::normalize(npcVelocity) * 3.75f;

        npc->GetCharacter()->rigidBody->SetLinearVelocity(npcVelocity);

        // Reorient the npc to the velocity vector
        glm::mat4 npcOrientation;
        npc->GetCharacter()->rigidBody->GetMatOrientation(npcOrientation);
        glm::vec3 npcDirection = npcOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        npcDirection.y = 0.0f; // Get rid of any pitch information

        glm::quat qRot = RotationBetweenVectors(npcDirection, npcVelocity);
        npcOrientation *= glm::toMat4(qRot);
        npc->GetCharacter()->rigidBody->SetMatOrientation(npcOrientation);
    }
}

void cNPCManager::SolveForDying(cCharacterControl* npc, double deltaTime)
{
    if (mIsDying)
    {
        cGameObject* ch = npc->GetCharacter();
        std::string animationName = ch->animations.dying;
        float totalTimeDying = ch->pSimpleSkinnedMesh->GetAnimationDuration(animationName);

        if (glfwGetTime() - mSysTimeDying >= totalTimeDying)
        {
            npc->SetCharacterState(cCharacterControl::eCharacterState::GUARDIAN);
            npc->GetCharacter()->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            npc->Idle();
            npc->Health100();
            mIsDying = false;
        }
        else
        {
            return;
        }
    }
    else
    {
        npc->Dying();
        mIsDying = true;
        mSysTimeDying = glfwGetTime();
    }
    

    
}
