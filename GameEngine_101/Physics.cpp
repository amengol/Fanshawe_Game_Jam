#include "Physics.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cGameObject.h"
#include "cAABBsManager.h"
#include "cSimpleDebugRenderer.h"

extern cAABBsManager* g_pAABBsManager;      // (GE101_Main.cpp)

extern std::vector< cGameObject* >  g_vecGameObjects;

extern cSimpleDebugRenderer* g_simpleDebug; // (GE101_Main.cpp)

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
                std::vector<sVertex> theGeometry;

                for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
                {
                    sAABB_Triangle* theTri = theAABB.AABBsTriangles[i];
                    
                    // Make physics thing                   

                    // Do we need to test for the triangle?
                    // Check if the Dot product between
                    // the point and the angle is positive
                    if(glm::dot(pCurGO->position, theTri->faceNormal) < 0.0f)
                    {
                        continue;
                    }
                    
                    // Print the triangle

                    
                    sVertex tmpGeo;

                    tmpGeo.x = theTri->verticeA.x;
                    tmpGeo.y = theTri->verticeA.y;
                    tmpGeo.z = theTri->verticeA.z;
                    // Find vertices normals
                    glm::vec3 normal = glm::normalize(glm::cross(theTri->verticeA, theTri->verticeB));
                    tmpGeo.nx = normal.x;
                    tmpGeo.ny = normal.y;
                    tmpGeo.nz = normal.z;
                    theGeometry.push_back(tmpGeo);

                    tmpGeo.x = theTri->verticeB.x;
                    tmpGeo.y = theTri->verticeB.y;
                    tmpGeo.z = theTri->verticeB.z;
                    // Find vertices normals
                    normal = glm::normalize(glm::cross(theTri->verticeB, theTri->verticeC));
                    tmpGeo.nx = normal.x;
                    tmpGeo.ny = normal.y;
                    tmpGeo.nz = normal.z;
                    theGeometry.push_back(tmpGeo);

                    tmpGeo.x = theTri->verticeC.x;
                    tmpGeo.y = theTri->verticeC.y;
                    tmpGeo.z = theTri->verticeC.z;
                    // Find vertices normals
                    normal = glm::normalize(glm::cross(theTri->verticeC, theTri->verticeA));
                    tmpGeo.nx = normal.x;
                    tmpGeo.ny = normal.y;
                    tmpGeo.nz = normal.z;
                    theGeometry.push_back(tmpGeo);
                }

                // Print the mesh
                g_simpleDebug->drawCustomGeometry(theGeometry);

            }

            pCurGO->Update(deltaTime, GRAVITY);
        }
        default:
            break;
        }

        


    }//for ( int index... 

    return;
}