#include "Physics.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cGameObject.h"
#include "cAABBsManager.h"
#include "cSimpleDebugRenderer.h"
#include "cTransparencyManager.h"
#include "simpleAI.h"

extern cTransparencyManager* g_pTranspManager;

extern cAABBsManager* g_pAABBsManager;      // (GE101_Main.cpp)

extern std::vector< cGameObject* >  g_vecGameObjects;

extern cSimpleDebugRenderer* g_simpleDebug; // (GE101_Main.cpp)

extern float g_AABBSize;                    // (GE101_Main.cpp)

void updateGameObjects(double deltaTime,
                       glm::vec3 gravity,
                       std::vector<cGameObject*> vecGO);

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{
    const glm::vec3 GRAVITY = glm::vec3(0.0f, 0.0f, 0.0f);

    // Update for regular Game Objects
    updateGameObjects(deltaTime, GRAVITY, g_vecGameObjects);

    // Update for transparency Game Objects
    updateGameObjects(deltaTime, GRAVITY, g_pTranspManager->transpObjects);

    return;
}

void updateGameObjects(double deltaTime, 
                       glm::vec3 gravity,
                       std::vector<cGameObject*> vecGO)
{
    for(int index = 0; index != vecGO.size(); index++)
    {
        cGameObject* pCurGO = vecGO[index];

        // Is this object to be updated?
        if(!pCurGO->bIsUpdatedInPhysics)
        {
            continue;
        }

        if(pCurGO->hasAI)
        {
            update_AI_Parameters(pCurGO, 10.0f);
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
                        //=====================================================
                        // WARNING it had a == and was working
                        // Investigate
                        //=====================================================
                        hasID = true;
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
                        glm::vec3 closestPointToSphere(0.0f);
                        if(TestSphereTriangle(pCurGO, theTri->verticeA, theTri->verticeB,
                           theTri->verticeC, closestPointToSphere))
                        {
                            // We have a collision                            
                            //glm::vec3 incidenceVector = pCurGO->position - pCurGO->previousPosition;
                            //glm::vec3 incidenceVector = pCurGO->vel;
                            glm::vec3 reflectedVector = glm::reflect(pCurGO->vel, theTri->faceNormal);
                            glm::quat qRotatedVel = glm::rotation(glm::normalize(pCurGO->vel), glm::normalize(reflectedVector));
                            pCurGO->vel = qRotatedVel * pCurGO->vel;
                            pCurGO->position = pCurGO->previousPosition;
                            pCurGO->vel.y *= 0.7f;
                        }

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



        }
        default:
            break;
        }

        pCurGO->Update(deltaTime, gravity);

    }//for ( int index... 
}

// Based on Ericson's code
// Returns true if sphere s intersects triangle ABC, false otherwise.
// The point p on abc closest to the sphere center is also returned
bool TestSphereTriangle(cGameObject* s, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 &p)
{
    // Find point P on triangle ABC closest to sphere center
    p = ClosestPtPointTriangle(s->position, a, b, c);

    // Sphere and triangle intersect if the (squared) distance from sphere
    // center to point p is less than the (squared) sphere radius
    glm::vec3 v = p - s->position;
    return glm::dot(v, v) <= s->radius * s->radius;
}

// Based on Ericson's code
glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    // Check if P in vertex region outside A
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 ap = p - a;
    float d1 = glm::dot(ab, ap);
    float d2 = glm::dot(ac, ap);
    if(d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)
                                           
    // Check if P in vertex region outside B
    glm::vec3 bp = p - b;
    float d3 = glm::dot(ab, bp);
    float d4 = glm::dot(ac, bp);
    if(d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)
                                         
    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1 * d4 - d3 * d2;
    if(vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0)
    }
    
    // Check if P in vertex region outside C
    glm::vec3 cp = p - c;
    float d5 = glm::dot(ab, cp);
    float d6 = glm::dot(ac, cp);
    if(d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5 * d2 - d1 * d6;
    if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return a + w * ac; // barycentric coordinates (1-w,0,w)
    }
    
    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3 * d6 - d5 * d4;
    if(va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }

    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}