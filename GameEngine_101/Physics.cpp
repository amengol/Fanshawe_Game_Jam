#include "Physics.h"
#include <glm/vec3.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "cGameObject.h"
#include "cAABBsManager.h"
#include "cSimpleDebugRenderer.h"
#include "cTransparencyManager.h"
#include "simpleAI.h"
#include "OptTriTri.h"

extern std::vector<LimitPlane> g_vecLimitPlanes;

extern cTransparencyManager* g_pTranspManager;

extern cAABBsManager* g_pAABBsManager;      // (GE101_Main.cpp)

extern std::vector< cGameObject* >  g_vecGameObjects;

extern cSimpleDebugRenderer* g_simpleDebug; // (GE101_Main.cpp)

extern float g_AABBSize;                    // (GE101_Main.cpp)

extern cVAOMeshManager* g_pVAOManager;

void updateGameObjects(double deltaTime,
                       glm::vec3 gravity,
                       std::vector<cGameObject*> vecGO);

bool areIntercepting(sCollisionTriangle*, sAABB_Triangle*);

void drawCollMesh(std::string, cGameObject*);

RK4_Derivative evaluate(const RK4_State& initial, 
                        float dt, 
                        const RK4_Derivative& d)
{
    RK4_State state;
    state.x = initial.x + d.dx*dt;
    state.v = initial.v + d.dv*dt;

    RK4_Derivative output;
    output.dx = state.v;
    //output.dv = acceleration(state, t + dt);
    return output;
}

void integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt)
{
    {
        // Put the acceleration into the velocity
        glm::vec3 newVel = vel + accel * dt;
        
        RK4_State state;
        state.x = pos;
        state.v = newVel;

        RK4_Derivative a, b, c, d;

        a = evaluate(state, 0.0f, RK4_Derivative());
        b = evaluate(state, dt*0.5f, a);
        c = evaluate(state, dt*0.5f, b);
        d = evaluate(state, dt, c);

        glm::vec3 dxdt = 1.0f / 6.0f *
            (a.dx + 2.0f * (b.dx + c.dx) + d.dx);

        glm::vec3 dvdt = 1.0f / 6.0f *
            (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

        pos = state.x + dxdt * dt;
        vel = state.v + dvdt * dt;
    }
}

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime)
{
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -10.0f, 0.0f);

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
            for (size_t i = 0; i < g_vecLimitPlanes.size(); i++)
            {
                LimitPlane limitPlane = g_vecLimitPlanes[i];

                switch (limitPlane.type)
                {
                case FLOOR:
                    if (pCurGO->position.y <= limitPlane.position.y + pCurGO->radius)
                    {
                        pCurGO->position.y = limitPlane.position.y + pCurGO->radius + 0.01;
                        pCurGO->vel.y = -pCurGO->vel.y;
                        //pCurGO->vel *= 0.85f;
                    }
                    break;
                case FRONT:
                    if (pCurGO->position.z <= limitPlane.position.z + pCurGO->radius)
                    {
                        pCurGO->position.z = limitPlane.position.z + pCurGO->radius + 0.01;
                        pCurGO->vel.z = -pCurGO->vel.z;
                        //pCurGO->vel *= 0.85f;
                    }
                    break;
                case BACK:
                    if (pCurGO->position.z >= limitPlane.position.z - pCurGO->radius)
                    {
                        pCurGO->position.z = limitPlane.position.z - pCurGO->radius - 0.01;
                        pCurGO->vel.z = -pCurGO->vel.z;
                        //pCurGO->vel *= 0.85f;
                    }
                    break;
                case LEFT:
                    if (pCurGO->position.x <= limitPlane.position.x + pCurGO->radius)
                    {
                        pCurGO->position.x = limitPlane.position.x + pCurGO->radius + 0.01;
                        pCurGO->vel.x = -pCurGO->vel.x;
                        //pCurGO->vel *= 0.85f;
                    }
                    break;
                case RIGHT:
                    if (pCurGO->position.x >= limitPlane.position.x - pCurGO->radius)
                    {
                        pCurGO->position.x = limitPlane.position.x - pCurGO->radius - 0.01;
                        pCurGO->vel.x = -pCurGO->vel.x;
                        //pCurGO->vel *= 0.85f;
                    }
                    break;
                default:
                    break;
                }
            }
            

            //---------------------------------------------------------
            // Collision Detection with other spheres
            for (int i = 0; i < g_vecGameObjects.size(); i++)
            {
                if (g_vecGameObjects[i]->typeOfObject == SPHERE && g_vecGameObjects[i] != pCurGO)
                {
                    cGameObject* sphere1 = pCurGO;
                    cGameObject* sphere2 = g_vecGameObjects[i];
                    

                    float distance = glm::length(sphere1->position - sphere2->position);
                    if (distance <= sphere1->radius + sphere2->radius)
                    {
                        sphere1->position = sphere1->previousPosition;
                        sphere2->position = sphere2->previousPosition;

                        // Collision
                        glm::vec3 fakeNormalS2 = glm::normalize(sphere1->position - sphere2->position);
                        glm::vec3 reflectedVectorS1 = glm::reflect(sphere1->vel, fakeNormalS2);
                        sphere1->vel = reflectedVectorS1;
                        glm::vec3 fakeNormalS1 = glm::normalize(sphere2->position - sphere1->position);
                        glm::vec3 reflectedVectorS2 = glm::reflect(sphere1->vel, fakeNormalS1);
                        sphere2->vel = reflectedVectorS2;
                    }
                }
            }

            //----------------------------------------------------------
        }
            break;
        //case SPHERE:
        //{
        //    // Calculate all AABBs for the sphere
        //    // Put the sphere inside an axis-aligned box

        //    // Vertices
        //    float diameter = pCurGO->radius * 2;
        //    glm::vec3 vertices[8];
        //    vertices[0] = glm::vec3(pCurGO->position - pCurGO->radius);
        //    vertices[1] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z);
        //    vertices[2] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z);
        //    vertices[3] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z);
        //    vertices[4] = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z + diameter);
        //    vertices[5] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z + diameter);
        //    vertices[6] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z + diameter);
        //    vertices[7] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z + diameter);

        //    std::vector<long long> vecIDs;
        //    for(int i = 0; i < 8; i++)
        //    {
        //        long long GO_ID;
        //        if(!g_pAABBsManager->calcID(vertices[i], GO_ID, g_AABBSize))
        //        {
        //            continue;
        //        }
        //        bool hasID = false;
        //        for(int i = 0; i < vecIDs.size(); i++)
        //        {
        //            if(GO_ID == vecIDs[i])
        //            {
        //                hasID = true;
        //                break;
        //            }
        //        }
        //        if(!hasID)
        //        {
        //            vecIDs.push_back(GO_ID);
        //        }
        //    }

        //    for(int i = 0; i < vecIDs.size(); i++)
        //    {
        //        // Check if we have an AABB in that position
        //        cAABB theAABB(0, 0.0f);
        //        if(g_pAABBsManager->getAABB(vecIDs[i], theAABB))
        //        {
        //            std::vector<sVertex> theGeometry;

        //            for(int i = 0; i < theAABB.AABBsTriangles.size(); i++)
        //            {
        //                sAABB_Triangle* theTri = theAABB.AABBsTriangles[i];

        //                // Make physics thing                   

        //                // Do we need to test for the triangle?
        //                // Check if the Dot product between
        //                // the point and the angle is positive
        //                glm::vec3 originPos = pCurGO->position - theTri->Centroid;
        //                if(glm::dot(originPos, theTri->faceNormal) < 0.0f)
        //                {
        //                    continue;
        //                }

        //                //---------------------------------------------------------
        //                // Collision Detection
        //                glm::vec3 closestPointToSphere(0.0f);
        //                if(TestSphereTriangle(pCurGO, theTri->verticeA, theTri->verticeB,
        //                   theTri->verticeC, closestPointToSphere))
        //                {
        //                    // We have a collision                            
        //                    //glm::vec3 incidenceVector = pCurGO->position - pCurGO->previousPosition;
        //                    //glm::vec3 incidenceVector = pCurGO->vel;
        //                    glm::vec3 reflectedVector = glm::reflect(pCurGO->vel, theTri->faceNormal);
        //                    //glm::quat qRotatedVel = glm::rotation(glm::normalize(pCurGO->vel), glm::normalize(reflectedVector));
        //                    pCurGO->vel = reflectedVector;
        //                    pCurGO->position = pCurGO->previousPosition;
        //                    pCurGO->vel.x *= 0.7f;
        //                    pCurGO->vel.z *= 0.7f;
        //                }

        //                //---------------------------------------------------------
        //                // Collision Detection with other spheres
        //                for (int i = 0; i < g_vecGameObjects.size(); i++)
        //                {
        //                    if (g_vecGameObjects[i]->typeOfObject == SPHERE && g_vecGameObjects[i] != pCurGO)
        //                    {
        //                        cGameObject* sphere1 = pCurGO;
        //                        cGameObject* sphere2 = g_vecGameObjects[i];
        //                        

        //                        float distance = glm::length(sphere1->position - sphere2->position);
        //                        if (distance <= sphere1->radius + sphere2->radius)
        //                        {
        //                            sphere1->position = sphere1->previousPosition;
        //                            sphere2->position = sphere2->previousPosition;

        //                            // Collision
        //                            glm::vec3 fakeNormalS2 = glm::normalize(sphere1->position - sphere2->position);
        //                            glm::vec3 reflectedVectorS1 = glm::reflect(sphere1->vel, fakeNormalS2);
        //                            sphere1->vel = reflectedVectorS1;
        //                            glm::vec3 fakeNormalS1 = glm::normalize(sphere2->position - sphere1->position);
        //                            glm::vec3 reflectedVectorS2 = glm::reflect(sphere1->vel, fakeNormalS1);
        //                            sphere2->vel = reflectedVectorS2;
        //                        }
        //                    }
        //                }

        //                //----------------------------------------------------------
        //                // Create the triangle                    
        //                sVertex tmpGeo;

        //                tmpGeo.x = theTri->verticeA.x;
        //                tmpGeo.y = theTri->verticeA.y;
        //                tmpGeo.z = theTri->verticeA.z;
        //                // Find vertices normals
        //                glm::vec3 normal = glm::normalize(glm::cross(theTri->verticeA, theTri->verticeB));
        //                tmpGeo.nx = normal.x;
        //                tmpGeo.ny = normal.y;
        //                tmpGeo.nz = normal.z;
        //                theGeometry.push_back(tmpGeo);

        //                tmpGeo.x = theTri->verticeB.x;
        //                tmpGeo.y = theTri->verticeB.y;
        //                tmpGeo.z = theTri->verticeB.z;
        //                // Find vertices normals
        //                normal = glm::normalize(glm::cross(theTri->verticeB, theTri->verticeC));
        //                tmpGeo.nx = normal.x;
        //                tmpGeo.ny = normal.y;
        //                tmpGeo.nz = normal.z;
        //                theGeometry.push_back(tmpGeo);

        //                tmpGeo.x = theTri->verticeC.x;
        //                tmpGeo.y = theTri->verticeC.y;
        //                tmpGeo.z = theTri->verticeC.z;

        //                // Find vertices normals
        //                normal = glm::normalize(glm::cross(theTri->verticeC, theTri->verticeA));
        //                tmpGeo.nx = normal.x;
        //                tmpGeo.ny = normal.y;
        //                tmpGeo.nz = normal.z;
        //                theGeometry.push_back(tmpGeo);
        //            }

        //            // Print the mesh
        //            if(pCurGO->isDebugAABBActive)
        //            {
        //                g_simpleDebug->drawCustomGeometry(theGeometry, glm::vec3(0.0f, 1.0f, 0.0f));
        //            }

        //        }
        //    }

        //}
        case MESH:
        {
            {
                // Calculate all AABBs for the mesh
                
                // Take the mesh extents
                cMesh theMesh;
                if (!g_pVAOManager->lookupMeshFromName(pCurGO->meshName, theMesh))
                {
                    // Can't find the mesh
                    break;
                }
                float extent = theMesh.maxExtent;

                // Vertices                
                glm::vec3 vertices[8];
                vertices[0] = pCurGO->position - (extent / 2.0f);
                vertices[1] = glm::vec3(vertices[0].x + extent, vertices[0].y, vertices[0].z);
                vertices[2] = glm::vec3(vertices[0].x, vertices[0].y + extent, vertices[0].z);
                vertices[3] = glm::vec3(vertices[0].x + extent, vertices[0].y + extent, vertices[0].z);
                vertices[4] = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z + extent);
                vertices[5] = glm::vec3(vertices[0].x + extent, vertices[0].y, vertices[0].z + extent);
                vertices[6] = glm::vec3(vertices[0].x, vertices[0].y + extent, vertices[0].z + extent);
                vertices[7] = glm::vec3(vertices[0].x + extent, vertices[0].y + extent, vertices[0].z + extent);

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
                            hasID = true;
                            break;
                        }
                    }
                    if(!hasID)
                    {
                        vecIDs.push_back(GO_ID);
                    }
                }

                // Avoid getting stuck
                bool thisPassCollided = false;

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
                            //glm::vec3 originPos = pCurGO->position - theTri->Centroid;
                            //if(glm::dot(originPos, theTri->faceNormal) < 0.0f)
                            //{
                            //    continue;
                            //}

                            //---------------------------------------------------------
                            // Collision Detection
                            
                            for (int collIndex = 0; collIndex < pCurGO->contacPoints.size(); collIndex++)
                            {
                                sCollisionGeometry Geometry = pCurGO->contacPoints[collIndex];
                                

                                for (int geoIndex = 0; geoIndex < Geometry.collisionTriangles.size(); geoIndex++)
                                {
                                    sCollisionTriangle collTri = *Geometry.collisionTriangles[geoIndex];
                                    
                                    // The collision triangles are all at the origin. We need to
                                    // reorient to the object position and rotation

                                    // Rotation
                                    collTri.verticeA = pCurGO->orientation * glm::vec4(collTri.verticeA, 0.0f);
                                    collTri.verticeB = pCurGO->orientation * glm::vec4(collTri.verticeB, 0.0f);
                                    collTri.verticeC = pCurGO->orientation * glm::vec4(collTri.verticeC, 0.0f);

                                    // Translation
                                    collTri.verticeA += pCurGO->position;
                                    collTri.verticeB += pCurGO->position;
                                    collTri.verticeC += pCurGO->position;
                                    
                                                                        
                                    //Test if the trianlge is intercepting with a mesh
                                    if (areIntercepting(&collTri, theTri))
                                    {
                                        drawCollMesh(Geometry.meshName, pCurGO);
                                        thisPassCollided = true;
                                        if (!pCurGO->hadAlreadyCollided)
                                        {
                                            pCurGO->vel = glm::vec3(0.0f);
                                            pCurGO->rateOfTurnX = 0.0f;
                                            pCurGO->rateOfTurnY = 0.0f;
                                            pCurGO->rateOfTurnZ = 0.0f;
                                            pCurGO->hadAlreadyCollided = true;
                                        }
                                        
                                    }
                                }
                            }

                            
                            // End of collision detection
                            //---------------------------------------------------------

                            // Create the triangle of the mesh for rendering                
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
                        if(pCurGO->isDebugAABBActive)
                        {
                            g_simpleDebug->drawCustomGeometry(theGeometry, glm::vec3(0.0f, 1.0f, 0.0f));                           
                        }

                    }
                }

                // Did this pass had a collision
                if (!thisPassCollided)
                {
                    pCurGO->hadAlreadyCollided = false;
                }

            }
        }
        break;
        default:
            break;
        }

        //pCurGO->Update(deltaTime, gravity);

        // Remember the last position
        pCurGO->previousPosition = pCurGO->position;

        //RK4
        integrate(pCurGO->position, pCurGO->vel, gravity, deltaTime);

        //// New velocity is based on acceleration over time
        //// Acceleration is based on local axis
        ////glm::vec3 newAccel = this->orientation * glm::vec4(this->accel, 0.0f);
        //glm::vec3 deltaVelocity = ((float)deltaTime * /*newAccel*/pCurGO->accel)
        //    + ((float)deltaTime * gravity);
        //pCurGO->vel += deltaVelocity;

        //// Explicit Euler  (RK4)
        //// New position is based on velocity over time
        //// Velocity is based on local axis
        //glm::vec3 newVel = pCurGO->orientation * glm::vec4(pCurGO->vel, 0.0f);
        //glm::vec3 deltaPosition = (float)deltaTime * newVel;
        //pCurGO->position += deltaPosition;

        //------------------------------------------------------------------------
        // Change orientation according to the Rate Of Turn per minute     
        if (pCurGO->rateOfTurnX != 0.0f)
        {
            float theTurnX = (pCurGO->rateOfTurnX * deltaTime) / 60.0f;
            pCurGO->orientation = glm::rotate(pCurGO->orientation, glm::radians(theTurnX), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (pCurGO->rateOfTurnY != 0.0f)
        {
            float theTurnY = (pCurGO->rateOfTurnY * deltaTime) / 60.0f;
            pCurGO->orientation = glm::rotate(pCurGO->orientation, glm::radians(theTurnY), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (pCurGO->rateOfTurnZ != 0.0f)
        {
            float theTurnZ = (pCurGO->rateOfTurnZ * deltaTime) / 60.0f;
            pCurGO->orientation = glm::rotate(pCurGO->orientation, glm::radians(theTurnZ), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        //------------------------------------------------------------------------ 
        // Update sound Objects
        if (pCurGO->hasSound())
            pCurGO->setGOSoundPos();
        //------------------------------------------------------------------------ 

    }//for ( int index... 
}

void drawCollMesh(std::string meshName, cGameObject* GO)
{
    for (int collIndex = 0; collIndex < GO->contacPoints.size(); collIndex++)
    {
        if (GO->contacPoints[collIndex].meshName == meshName)
        {
            sCollisionGeometry Geometry = GO->contacPoints[collIndex];
            std::vector<sVertex> CollisionGeometry;

            for (int geoIndex = 0; geoIndex < Geometry.collisionTriangles.size(); geoIndex++)
            {
                sCollisionTriangle* collTri = Geometry.collisionTriangles[geoIndex];

                // Create the triangle                    
                sVertex tmpGeo;

                tmpGeo.x = collTri->verticeA.x;
                tmpGeo.y = collTri->verticeA.y;
                tmpGeo.z = collTri->verticeA.z;
                // Find vertices normals
                glm::vec3 normal = glm::normalize(glm::cross(collTri->verticeA, collTri->verticeB));
                tmpGeo.nx = normal.x;
                tmpGeo.ny = normal.y;
                tmpGeo.nz = normal.z;
                CollisionGeometry.push_back(tmpGeo);

                tmpGeo.x = collTri->verticeB.x;
                tmpGeo.y = collTri->verticeB.y;
                tmpGeo.z = collTri->verticeB.z;
                // Find vertices normals
                normal = glm::normalize(glm::cross(collTri->verticeB, collTri->verticeC));
                tmpGeo.nx = normal.x;
                tmpGeo.ny = normal.y;
                tmpGeo.nz = normal.z;
                CollisionGeometry.push_back(tmpGeo);

                tmpGeo.x = collTri->verticeC.x;
                tmpGeo.y = collTri->verticeC.y;
                tmpGeo.z = collTri->verticeC.z;

                // Find vertices normals
                normal = glm::normalize(glm::cross(collTri->verticeC, collTri->verticeA));
                tmpGeo.nx = normal.x;
                tmpGeo.ny = normal.y;
                tmpGeo.nz = normal.z;
                CollisionGeometry.push_back(tmpGeo);
            }

            glm::mat4 repo(1.0f);
            repo = glm::translate(repo, GO->position);
            repo *= GO->orientation;
            g_simpleDebug->drawCustomGeometry(CollisionGeometry,
                                              glm::vec3(1.0f, 0.0f, 0.0f),
                                              true,
                                              repo);
        }

    }
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

bool areIntercepting(sCollisionTriangle* collTri, sAABB_Triangle* AABB_Tri)
{
    float collTriA[3];
    collTriA[0] = collTri->verticeA.x;
    collTriA[1] = collTri->verticeA.y;
    collTriA[2] = collTri->verticeA.z;

    float collTriB[3];
    collTriB[0] = collTri->verticeB.x;
    collTriB[1] = collTri->verticeB.y;
    collTriB[2] = collTri->verticeB.z;

    float collTriC[3];
    collTriC[0] = collTri->verticeC.x;
    collTriC[1] = collTri->verticeC.y;
    collTriC[2] = collTri->verticeC.z;

    float AABB_TriA[3];
    AABB_TriA[0] = AABB_Tri->verticeA.x;
    AABB_TriA[1] = AABB_Tri->verticeA.y;
    AABB_TriA[2] = AABB_Tri->verticeA.z;

    float AABB_TriB[3];
    AABB_TriB[0] = AABB_Tri->verticeB.x;
    AABB_TriB[1] = AABB_Tri->verticeB.y;
    AABB_TriB[2] = AABB_Tri->verticeB.z;

    float AABB_TriC[3];
    AABB_TriC[0] = AABB_Tri->verticeC.x;
    AABB_TriC[1] = AABB_Tri->verticeC.y;
    AABB_TriC[2] = AABB_Tri->verticeC.z;

    if (NoDivTriTriIsect(collTriA, collTriB, collTriC,
                         AABB_TriA, AABB_TriB, AABB_TriC))
    {
        return true;
    }
    else
    {
        return false;
    }
        
}