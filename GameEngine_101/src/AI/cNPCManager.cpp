#include "cNPCManager.h"
#include "cCharacterControl.h"
#include <glm\glm.hpp>
#include "..\cGameObject.h"
#include <glm\gtx\transform.hpp>

cNPCManager::cNPCManager()
{
}


cNPCManager::~cNPCManager()
{
}

void cNPCManager::Evaluate(double deltaTime)
{
    for (size_t i = 0; i < mNPCs.size(); i++)
    {
        cCharacterControl* npc = mNPCs[i];
        npc->Forward();

        glm::vec3 playerPosition;
        mPlayer->GetCharacter()->rigidBody->GetPostion(playerPosition);
        glm::vec3 npcPosition;
        npc->GetCharacter()->rigidBody->GetPostion(npcPosition);

        glm::vec3 npcVelocity;
        npc->GetCharacter()->rigidBody->GetVelocity(npcVelocity);
        
        glm::vec3 desiredVelocity = glm::normalize(playerPosition - npcPosition) * 1.5f;


        glm::vec3 steering = desiredVelocity - npcVelocity;
        steering *= deltaTime * 1.2f; // Speed up the steering a litle bit

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
