#include "cAABBsManager.h"
#include "sAABB_Triangle.h"

cAABBsManager::cAABBsManager()
{
}

cAABBsManager::~cAABBsManager()
{
}

void cAABBsManager::genAABBs(cMesh* mesh, float size)
{
    
    // For each triangle in the mesh, create the necessary AABBs    
    for (int i = 0; i < mesh->numberOfTriangles; i++)
    {
        // Reconstrunct the triangle
        glm::vec3 vertA;
        glm::vec3 vertB;
        glm::vec3 vertC;

        vertA.x = mesh->pVertices[mesh->pTriangles[i].vertex_ID_0].x;
        vertA.y = mesh->pVertices[mesh->pTriangles[i].vertex_ID_0].y;
        vertA.z = mesh->pVertices[mesh->pTriangles[i].vertex_ID_0].z;

        vertB.x = mesh->pVertices[mesh->pTriangles[i].vertex_ID_1].x;
        vertB.y = mesh->pVertices[mesh->pTriangles[i].vertex_ID_1].y;
        vertB.z = mesh->pVertices[mesh->pTriangles[i].vertex_ID_1].z;

        vertC.x = mesh->pVertices[mesh->pTriangles[i].vertex_ID_2].x;
        vertC.y = mesh->pVertices[mesh->pTriangles[i].vertex_ID_2].y;
        vertC.z = mesh->pVertices[mesh->pTriangles[i].vertex_ID_2].z;
        
        sAABB_Triangle* tri;
        tri->verticeA = vertA;
        tri->verticeB = vertB;
        tri->verticeC = vertC;
        
        // Find longest side
        float longestSide = this->calcLongestSide(tri);

        // Need tesselation?
        std::vector<sAABB_Triangle*> tesseleted;

        if (this->tesselate(tri, size, tesseleted))
        {
            // Create the AABBs according to the tesselated triangle
            for (int i = 0; i < tesseleted.size(); i++)
            {
                long long ID1 = this->calcID(tesseleted[i]->verticeA);
                
                // Check if the ID already exists
                std::map<long long, cAABB*>::iterator itAABB = this->m_mapIDtoAABB.find(ID1);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID1);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID1] = theAABB;
                }
                else
                {
                    // There is already an ID, so just add the triangle to it
                    cAABB* theAABB = itAABB->second;
                    theAABB->AABBsTriangles.push_back(tri);
                }


                long long ID2 = this->calcID(tesseleted[i]->verticeB);

                // Check if the ID already exists
                itAABB = this->m_mapIDtoAABB.find(ID2);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID2);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID2] = theAABB;
                }
                else
                {
                    // There is already an ID, so just add the triangle to it
                    cAABB* theAABB = itAABB->second;
                    theAABB->AABBsTriangles.push_back(tri);
                }

                long long ID3 = this->calcID(tesseleted[i]->verticeC);

                // Check if the ID already exists
                itAABB = this->m_mapIDtoAABB.find(ID3);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID3);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID2] = theAABB;
                }
                else
                {
                    // There is already an ID, so just add the triangle to it
                    cAABB* theAABB = itAABB->second;
                    theAABB->AABBsTriangles.push_back(tri);
                }
            }//for (int i = 0; i < tesseleted...
        }//if (this->tesselate(tri...
        else
        {
            // Create the AABBs acording to the triangle itself
            long long ID1 = this->calcID(tri->verticeA);

            // Check if the ID already exists
            std::map<long long, cAABB*>::iterator itAABB = this->m_mapIDtoAABB.find(ID1);
            if (itAABB == this->m_mapIDtoAABB.end())
            {
                // Didn't find an AABB, let's create it
                cAABB* theAABB = new cAABB(ID1);
                theAABB->AABBsTriangles.push_back(tri);
                this->m_mapIDtoAABB[ID1] = theAABB;
            }
            else
            {
                // There is already an ID, so just add the triangle to it
                cAABB* theAABB = itAABB->second;
                theAABB->AABBsTriangles.push_back(tri);
            }


            long long ID2 = this->calcID(tri->verticeB);

            // Check if the ID already exists
            itAABB = this->m_mapIDtoAABB.find(ID2);
            if (itAABB == this->m_mapIDtoAABB.end())
            {
                // Didn't find an AABB, let's create it
                cAABB* theAABB = new cAABB(ID2);
                theAABB->AABBsTriangles.push_back(tri);
                this->m_mapIDtoAABB[ID2] = theAABB;
            }
            else
            {
                // There is already an ID, so just add the triangle to it
                cAABB* theAABB = itAABB->second;
                theAABB->AABBsTriangles.push_back(tri);
            }

            long long ID3 = this->calcID(tri->verticeC);

            // Check if the ID already exists
            itAABB = this->m_mapIDtoAABB.find(ID3);
            if (itAABB == this->m_mapIDtoAABB.end())
            {
                // Didn't find an AABB, let's create it
                cAABB* theAABB = new cAABB(ID3);
                theAABB->AABBsTriangles.push_back(tri);
                this->m_mapIDtoAABB[ID2] = theAABB;
            }
            else
            {
                // There is already an ID, so just add the triangle to it
                cAABB* theAABB = itAABB->second;
                theAABB->AABBsTriangles.push_back(tri);
            }
        }//elseif (this->tesselate(tri...        
    }//for (int i = 0; i < mesh->numberOfTriangles...

}

float cAABBsManager::calcLongestSide(sAABB_Triangle* triangle)
{
    return 0.0f;
}

long long cAABBsManager::calcID(glm::vec3 vertice)
{
    return 0;
}

bool cAABBsManager::tesselate(sAABB_Triangle * triangle, 
                              float size, 
                              std::vector<sAABB_Triangle*>& tesseleted)
{
    return false;
}
