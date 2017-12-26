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
            cAABB theAABB(0, 0.0f);
            if(g_pAABBsManager->getAABB(GO_ID, theAABB))
            {
                for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
                {
                    sAABB_Triangle* theTri = theAABB.AABBsTriangles[i];
                    // Make physics thing
                }
            }

            pCurGO->Update(deltaTime, GRAVITY);
        }
        default:
            break;
        }

        


    }//for ( int index... 

    return;
}