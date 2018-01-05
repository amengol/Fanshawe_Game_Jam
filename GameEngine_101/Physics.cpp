#include "Physics.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cGameObject.h"
#include "cAABBsManager.h"
#include "cSimpleDebugRenderer.h"

extern cAABBsManager* g_pAABBsManager;      // (GE101_Main.cpp)

extern std::vector< cGameObject* >  g_vecGameObjects;

extern cSimpleDebugRenderer* g_simpleDebug; // (GE101_Main.cpp)

extern float g_AABBSize;                    // (GE101_Main.cpp)

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -5.0f, 0.0f);

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
            if(!g_pAABBsManager->calcID(pCurGO->position, GO_ID, g_AABBSize))
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
                    /*glm::vec3 originPos = pCurGO->position - theTri->Centroid;
                    if(glm::dot(originPos, theTri->faceNormal) < 0.0f)
                    {
                        continue;
                    }*/
                    
                    //---------------------------------------------------------
                    // Collision Detection
                    
                    // Project the point at the same plane as the triangle
                    // Inspired by the Closest Point on Plane to Point from
                    // Christer Ericson's book
                    glm::vec3 normalA = theTri->verticeA + theTri->faceNormal;
                    //float t = glm::dot(normalA, (pCurGO->position - theTri->verticeA));
                    float t = (glm::dot(normalA, pCurGO->position) - glm::length(theTri->verticeA)) / glm::dot(normalA, normalA);
                    glm::vec3 projectedPoint = pCurGO->position - t * normalA;

                    // Find the triangle ABC
                    glm::vec3 AB = theTri->verticeB - theTri->verticeA;
                    glm::vec3 AC = theTri->verticeC - theTri->verticeA;
                    glm::vec3 BC = theTri->verticeC - theTri->verticeB;

                    // The triangle ABC area
                    float areaABC = glm::length(glm::cross(AB, AC)) / 2;

                    // If we consider the point being inside the triangle, the
                    // sum of the areas of the triangles formed by two vertex 
                    // and the point must be equal to the are of the triangle.
                    // If they are grater it means that the point is outside
                    // of the triangle.
                   
                    // Triangle APC
                    glm::vec3 AP = projectedPoint - theTri->verticeA;
                    float areaAPC = glm::length(glm::cross(AC, AP)) / 2;

                    // Triangle APB
                    float areaAPB = glm::length(glm::cross(AP, AB)) / 2;

                    // Triangle BPC
                    glm::vec3 CP = projectedPoint - theTri->verticeC;
                    float areaBPC = glm::length(glm::cross(CP, BC)) / 2;

                    // Area of the 3 sub triangles
                    float subTrisArea = areaAPB + areaAPC + areaBPC;

                    // Let's give a threshold for floating point erros
                    if(subTrisArea - areaABC < 0.1f)// && fabs(t) < 0.1f)
                    {
                        // We have a collision
                        //pCurGO->bIsUpdatedInPhysics = false;
                        pCurGO->vel.y = -pCurGO->vel.y;
                    }

                    //---------------------------------------------------------

                    // Create the triangle                    
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
                g_simpleDebug->drawCustomGeometry(theGeometry, glm::vec3(0.0f, 1.0f, 0.0f));

            }

            pCurGO->Update(deltaTime, GRAVITY);
        }
        default:
            break;
        }

        


    }//for ( int index... 

    return;
}