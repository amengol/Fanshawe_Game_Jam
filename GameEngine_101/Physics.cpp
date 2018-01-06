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
            // Calculate all AABBs for the sphere
            // Put the sphere inside an axis-aligned box
            
            // Vertices
            float diameter = pCurGO->radius * 2;
            glm::vec3 vertices[8];
            vertices[0] = glm::vec3(pCurGO->position - pCurGO->radius);            
            vertices[1] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z);
            vertices[2] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z);
            vertices[3] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z);
            vertices[4] = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z + diameter);
            vertices[5] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z + diameter);
            vertices[6] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z + diameter);
            vertices[7] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z + diameter);

            std::vector<long long> vecIDs;
            for(int i = 0; i < 8; i++)
            {
                long long GO_ID;
                if(!g_pAABBsManager->calcID(vertices[i], GO_ID, g_AABBSize))
                {
                    continue;
                }
                bool hasID = false;
                for(int i = 0; i < vecIDs.size(); i++)
                {
                    if(GO_ID == vecIDs[i])
                    {
                        hasID == true;
                        break;
                    }
                }
                if(!hasID)
                {
                    vecIDs.push_back(GO_ID);
                }
            }
            
            for(int i = 0; i < vecIDs.size(); i++)
            {
                // Check if we have an AABB in that position
                cAABB theAABB(0, 0.0f);
                if(g_pAABBsManager->getAABB(vecIDs[i], theAABB))
                {
                    std::vector<sVertex> theGeometry;

                    for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
                    {
                        sAABB_Triangle* theTri = theAABB.AABBsTriangles[i];

                        // Make physics thing                   

                        // Do we need to test for the triangle?
                        // Check if the Dot product between
                        // the point and the angle is positive
                        glm::vec3 originPos = pCurGO->position - theTri->Centroid;
                        if(glm::dot(originPos, theTri->faceNormal) < 0.0f)
                        {
                            continue;
                        }

                        //---------------------------------------------------------
                        // Collision Detection


                        //---------------------------------------------------------

                        // OLD CODE
                        //// Project the point at the same plane as the triangle
                        //// Inspired by the Closest Point on Plane to Point from
                        //// Christer Ericson's book
                        //glm::vec3 projectedPoint = pCurGO->position - (glm::dot((pCurGO->position - theTri->verticeA), theTri->faceNormal) * theTri->faceNormal);
                        //float t = glm::dot((pCurGO->position - theTri->verticeA), theTri->faceNormal);

                        //    

                        //float testDot = glm::dot(projectedPoint - theTri->verticeA, theTri->faceNormal);

                        //// Find the triangle ABC
                        //glm::vec3 AB = theTri->verticeB - theTri->verticeA;
                        //glm::vec3 AC = theTri->verticeC - theTri->verticeA;
                        //glm::vec3 BC = theTri->verticeC - theTri->verticeB;

                        //// The triangle ABC area
                        //float areaABC = glm::length(glm::cross(AB, AC)) / 2;

                        //// If we consider the point being inside the triangle, the
                        //// sum of the areas of the triangles formed by two vertex 
                        //// and the point must be equal to the are of the triangle.
                        //// If they are grater it means that the point is outside
                        //// of the triangle.

                        //// Triangle APC
                        //glm::vec3 AP = projectedPoint - theTri->verticeA;
                        //float areaAPC = glm::length(glm::cross(AC, AP)) / 2;

                        //// Triangle APB
                        //float areaAPB = glm::length(glm::cross(AP, AB)) / 2;

                        //// Triangle BPC
                        //glm::vec3 CP = projectedPoint - theTri->verticeC;
                        //glm::vec3 CB = theTri->verticeB - theTri->verticeC;
                        //float areaBPC = glm::length(glm::cross(CP, CB)) / 2;

                        //// Area of the 3 sub triangles
                        //float subTrisArea = areaAPB + areaAPC + areaBPC;

                        //if(t < lowerT)
                        //{
                        //    lowerT = t;
                        //    printf("Lowest value of t is %f for a diff area of %f\n", lowerT, subTrisArea - areaABC);
                        //}

                        //if(subTrisArea - areaABC < lowerDiff)
                        //{
                        //    lowerDiff = subTrisArea - areaABC;
                        //    printf("Lowest diff is %f for a t of %f\n", lowerDiff, t);
                        //}

                        //// Let's give a threshold for floating point erros
                        //if(subTrisArea - areaABC < 0.1f && t < 0.0f)
                        //{
                        //    // We have a collision                     
                        //}

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
            }


            pCurGO->Update(deltaTime, GRAVITY);
        }
        default:
            break;
        }

        


    }//for ( int index... 

    return;
}