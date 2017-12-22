#include "Physics.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cGameObject.h"

extern std::vector< cGameObject* >  g_vecGameObjects;

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -10.0f, 0.0f);

    // Identical to the 'render' (drawing) loop
    for (int index = 0; index != ::g_vecGameObjects.size(); index++)
    {
        cGameObject* pCurGO = ::g_vecGameObjects[index];

        // Is this object to be updated?
        if (!pCurGO->bIsUpdatedInPhysics)
        {
            continue;
        }

        pCurGO->Update(deltaTime, GRAVITY);


    }//for ( int index... 

    return;
}