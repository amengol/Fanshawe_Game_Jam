#include "cNPCManager.h"
#include "cCharacterControl.h"
#include <glm\glm.hpp>
#include "..\cGameObject.h"
#include <glm\gtx\transform.hpp>

cNPCManager::cNPCManager()
{
    mInterestRadius = 10.0f;
    mStopDistance = 2.0f;                  
    mThreshold = 2.0f;
}


cNPCManager::~cNPCManager()
{
}

void cNPCManager::Evaluate(double deltaTime)
{
    for (size_t i = 0; i < mNPCs.size(); i++)
    {
        // Shift too close NPCs
        for (size_t j = 0; j < mNPCs.size(); j++)
        {
            if (mNPCs[j] == mNPCs[i])
                continue;

            // Evaluate their distance
            glm::vec3 npcI_Position;
            mNPCs[i]->GetCharacter()->rigidBody->GetPostion(npcI_Position);
            glm::vec3 npcJ_Position;
            mNPCs[j]->GetCharacter()->rigidBody->GetPostion(npcJ_Position);

            float distance = glm::length(npcJ_Position - npcI_Position);

            if (distance > 0.5f)
                continue;

            glm::vec3 shiftDirection = npcJ_Position - npcI_Position;
            shiftDirection = glm::normalize(shiftDirection);

            if (mNPCs[i]->GetAnimationState() != cCharacterControl::eAnimationState::IDLE)
                npcI_Position += -(shiftDirection * 0.3f);
            if (mNPCs[j]->GetAnimationState() != cCharacterControl::eAnimationState::IDLE)
                npcJ_Position += shiftDirection * 0.3f;

            mNPCs[i]->GetCharacter()->rigidBody->SetPosition(npcI_Position);
            mNPCs[j]->GetCharacter()->rigidBody->SetPosition(npcJ_Position);
        }

        switch (mNPCs[i]->GetCharacterState())
        {
        case cCharacterControl::eCharacterState::FOLLOWER:
            SolveForFollower(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::CURIOUS_APPROACH:
            SolveForCuriousApproach(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::CURIOUS_EVADE:
            SolveForCuriousEvade(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::ANGRY_PURSUIT:
            SolveForAngryPursuit(mNPCs[i], deltaTime);
            break;
        case cCharacterControl::eCharacterState::ANGRY_EVADE:
            SolveForAngryEvade(mNPCs[i], deltaTime);
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

void cNPCManager::SolveForFollower(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    float distance = glm::length(playerPosition - npcPosition);

    // Case when the player is out of the interest radius
    if (distance > mInterestRadius && npc->GetAnimationState() != cCharacterControl::eAnimationState::IDLE)
    {
        npc->Idle();
        npc->GetCharacter()->rigidBody->SetVelocity(glm::vec3(0.0f));
        return; // Out of the interest radius
    }
    else if (distance > mInterestRadius && npc->GetAnimationState() == cCharacterControl::eAnimationState::IDLE)
    {
        return; // Out of the interest radius
    }

    // Case when the player is too close
    if (distance < mStopDistance)
    {
        if (npc->GetAnimationState() != cCharacterControl::eAnimationState::IDLE)
        {
            npc->Idle();
            npc->GetCharacter()->rigidBody->SetVelocity(glm::vec3(0.0f));
        }

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::TRICK)
        {
            npc->GetCharacter()->diffuseColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            npc->SetCharacterState(cCharacterControl::eCharacterState::CURIOUS_APPROACH);
        }

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::VIOLENT_TRICK)
        {
            npc->GetCharacter()->diffuseColour = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
            npc->SetCharacterState(cCharacterControl::eCharacterState::CURIOUS_EVADE);
        }

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_CROSS_PUNCH
            || mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_KICKING)
        {
            npc->GetCharacter()->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            npc->SetCharacterState(cCharacterControl::eCharacterState::ANGRY_PURSUIT);
        }

        return;
    }
    
    // Check for enough space before proceed
    if (distance < mThreshold + mStopDistance && npc->GetAnimationState() == cCharacterControl::eAnimationState::IDLE)
    {
        return; // Give it some space to avoid staggering
    }
    else
    {
        // All other cases

        npc->Forward();

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);

        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 1.5f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 2.0f; // Speed up the steering a litle bit

        npcVelocity += steering;
        npcVelocity = glm::normalize(npcVelocity) * 1.5f;

        npc->GetCharacter()->rigidBody->SetVelocity(npcVelocity);

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

void cNPCManager::SolveForCuriousApproach(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    float distance = glm::length(playerPosition - npcPosition);

    // Case when the player is too close
    if (distance < mStopDistance)
    {
        if (npc->GetAnimationState() != cCharacterControl::eAnimationState::IDLE)
        {
            npc->Idle();
            npc->GetCharacter()->rigidBody->SetVelocity(glm::vec3(0.0f));
        }

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::VIOLENT_TRICK)
        {
            npc->GetCharacter()->diffuseColour = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
            npc->SetCharacterState(cCharacterControl::eCharacterState::CURIOUS_EVADE);
        }

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_CROSS_PUNCH
            || mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_KICKING)
        {
            npc->GetCharacter()->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            npc->SetCharacterState(cCharacterControl::eCharacterState::ANGRY_PURSUIT);
        }

        return;
    }

    // Check for enough space before proceed
    if (distance < mThreshold + mStopDistance && npc->GetAnimationState() == cCharacterControl::eAnimationState::IDLE)
    {
        return; // Give it some space to avoid staggering
    }
    else
    {
        // All other cases

        npc->ForwardRun();

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);

        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 3.75f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 3.0f; // Speed up the steering a litle bit

        npcVelocity += steering;
        npcVelocity = glm::normalize(npcVelocity) * 3.75f;

        npc->GetCharacter()->rigidBody->SetVelocity(npcVelocity);

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

void cNPCManager::SolveForCuriousEvade(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    //float distance = glm::length(playerPosition - npcPosition);

    npc->Runaway();

    glm::vec3 npcVelocity;
    npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);

    glm::vec3 desiredVelocity = glm::normalize(npcPosition - playerPosition) * 3.75f;


    glm::vec3 steering = desiredVelocity - npcVelocity;
    steering *= deltaTime * 3.0f; // Speed up the steering a litle bit

    npcVelocity += steering;
    npcVelocity = glm::normalize(npcVelocity) * 3.75f;

    npc->GetCharacter()->rigidBody->SetVelocity(npcVelocity);

    // Reorient the npc to the velocity vector
    glm::mat4 npcOrientation;
    npc->GetCharacter()->rigidBody->GetMatOrientation(npcOrientation);
    glm::vec3 npcDirection = npcOrientation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    npcDirection.y = 0.0f; // Get rid of any pitch information

    glm::quat qRot = RotationBetweenVectors(npcDirection, npcVelocity);
    npcOrientation *= glm::toMat4(qRot);
    npc->GetCharacter()->rigidBody->SetMatOrientation(npcOrientation);
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
        npc->GetCharacter()->rigidBody->SetVelocity(glm::vec3(0.0f));

        if (mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_KICKING ||
            mPlayer->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_CROSS_PUNCH)
        {
            npc->Hurt(deltaTime * 0.001f);

            if (npc->GetCharacter()->diffuseColour.g < 0.68f)
                npc->GetCharacter()->diffuseColour.g += 1.0f - npc->GetHealth();
            else
                npc->SetCharacterState(cCharacterControl::eCharacterState::ANGRY_EVADE);
        }

        return;
    }

    // Check for enough space before proceed
    if (distance < mThreshold + mStopDistance * 0.5f && npc->GetAnimationState() == cCharacterControl::eAnimationState::RIGHT_KICKING)
    {
        return; // Give it some space to avoid staggering
    }
    else
    {
        // All other cases

        npc->ForwardRun();

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);

        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 3.75f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 3.0f; // Speed up the steering a litle bit

        npcVelocity += steering;
        npcVelocity = glm::normalize(npcVelocity) * 3.75f;

        npc->GetCharacter()->rigidBody->SetVelocity(npcVelocity);

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

void cNPCManager::SolveForAngryEvade(cCharacterControl* npc, double deltaTime)
{
    // Evaluate their distance
    glm::vec3 playerPosition;
    mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
    glm::vec3 npcPosition;
    npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

    npc->Backwards();

    glm::vec3 npcVelocity;
    npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);

    glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 1.125f;


    glm::vec3 steering = npcVelocity - desiredVelocity;
    steering *= deltaTime * 3.0f; // Speed up the steering a litle bit

    npcVelocity += steering;
    npcVelocity = glm::normalize(npcVelocity) * 1.125f;

    npc->GetCharacter()->rigidBody->SetVelocity(npcVelocity);

    // Reorient the npc to the velocity vector
    glm::mat4 npcOrientation;
    npc->GetCharacter()->rigidBody->GetMatOrientation(npcOrientation);
    glm::vec3 npcDirection = npcOrientation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    npcDirection.y = 0.0f; // Get rid of any pitch information

    glm::quat qRot = RotationBetweenVectors(npcDirection, npcVelocity);
    npcOrientation *= glm::toMat4(qRot);
    npc->GetCharacter()->rigidBody->SetMatOrientation(npcOrientation);
}
