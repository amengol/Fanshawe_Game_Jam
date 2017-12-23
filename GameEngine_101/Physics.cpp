#include "Physics.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cGameObject.h"
#include "cAABBsManager.h"

extern cAABBsManager* g_pAABBsManager;      // (GE101_Main.cpp)

extern std::vector< cGameObject* >  g_vecGameObjects;

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{
    const glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, 0.0f);

    // Identical to the 'render' (drawing) loop
    for (int index = 0; index != ::g_vecGameObjects.size(); index++)
    {
        cGameObject* pCurGO = ::g_vecGameObjects[index];

        // Is this object to be updated?
        if (!pCurGO->bIsUpdatedInPhysics)
        {
            continue;
        }

        switch(pCurGO->typeOfObject)
        {
        case SPHERE:
        {
            // Calculate an AABB ID to the GameObject
            long long GO_ID;
            if(!g_pAABBsManager->calcID(pCurGO->position, GO_ID, 5.0f))
            {
                return;
            }

            // Check if we have an AABB in that position
            cAABB* theAABB = nullptr;
            if(g_pAABBsManager->getAABB(GO_ID, theAABB))
            {
                g_pAABBsManager->genDebugLines(GO_ID, 5.0f); // Not sure yet if I will go this way!!

                // TODO: draw my own debug AABB
            }

            pCurGO->Update(deltaTime, GRAVITY);
        }
        default:
            break;
        }

        


    }//for ( int index... 

    return;
}