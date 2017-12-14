#include "cAABBsManager.h"
#include "sAABB_Triangle.h"
#include <iostream>
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
        
        sAABB_Triangle* tri = new sAABB_Triangle();
        tri->verticeA = vertA;
        tri->verticeB = vertB;
        tri->verticeC = vertC;
        
        // Find longest side
        float longestSide = this->calcLongestSide(*tri);

        // Need tesselation?
        std::vector<sAABB_Triangle> tesseleted;

        if (!this->tesselate(tri, size, tesseleted))
        {
            std::cout << "Something went wrong during the tesselation\n";
        }//if (this->tesselate(tri...
        else
        {
            // Create the AABBs according to the tesselated triangles
            for (int i = 0; i < tesseleted.size(); i++)
            {

                long long ID1;
                if (!this->calcID(tesseleted[i].verticeA, ID1))
                {
                    std::cout << "There was an error generating an AABB ID\n";
                }

                // Check if the ID already exists
                std::map<long long, cAABB*>::iterator itAABB = this->m_mapIDtoAABB.find(ID1);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID1, size);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID1] = theAABB;
                }
                else
                {
                    // There is already an ID, so just add the triangle to it
                    cAABB* theAABB = itAABB->second;
                    theAABB->AABBsTriangles.push_back(tri);
                }

                long long ID2;
                if (!this->calcID(tesseleted[i].verticeB, ID2))
                {
                    std::cout << "There was an error generating an AABB ID\n";
                }

                // Check if the ID already exists
                itAABB = this->m_mapIDtoAABB.find(ID2);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID2, size);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID2] = theAABB;
                }
                else
                {
                    // There is already an ID, but we have to make 
                    // sure that the triangle wasn't already there
                    if (ID2 != ID1)
                    {
                        cAABB* theAABB = itAABB->second;
                        theAABB->AABBsTriangles.push_back(tri);
                    }                    
                }

                long long ID3;
                if (!this->calcID(tesseleted[i].verticeC, ID3))
                {
                    std::cout << "There was an error generating an AABB ID\n";
                }

                // Check if the ID already exists
                itAABB = this->m_mapIDtoAABB.find(ID3);
                if (itAABB == this->m_mapIDtoAABB.end())
                {
                    // Didn't find an AABB, let's create it
                    cAABB* theAABB = new cAABB(ID3, size);
                    theAABB->AABBsTriangles.push_back(tri);
                    this->m_mapIDtoAABB[ID2] = theAABB;
                }
                else
                {
                    // There is already an ID, but we have to make 
                    // sure that the triangle wasn't already there
                    if ((ID3 != ID1) && (ID3 != ID2))
                    {
                        cAABB* theAABB = itAABB->second;
                        theAABB->AABBsTriangles.push_back(tri);
                    }
                }
            }//for (int i = 0; i < tesseleted...
        }//elseif (this->tesselate(tri...        
    }//for (int i = 0; i < mesh->numberOfTriangles...

}

void cAABBsManager::genDebugTris()
{
    // Loop through the map
    for (std::map<long long, cAABB*>::iterator itTri = m_mapIDtoAABB.begin();
        itTri != m_mapIDtoAABB.end(); itTri++)
    {
        cAABB* theAABB = itTri->second;
        float diameter = theAABB->getDiameter();

        // The box has eight vertices
        glm::vec3 vert0 = this->genVecFromID(itTri->first);
        glm::vec3 vert1 = vert0 + glm::vec3(diameter, 0.0f, 0.0f);
        glm::vec3 vert2 = vert0 + glm::vec3(0.0f, diameter, 0.0f);
        glm::vec3 vert3 = vert0 + glm::vec3(diameter, diameter, 0.0f);
        glm::vec3 vert4 = vert0 + glm::vec3(0.0f, 0.0f, diameter);
        glm::vec3 vert5 = vert0 + glm::vec3(diameter, 0.0f, diameter);
        glm::vec3 vert6 = vert0 + glm::vec3(0.0f, diameter, diameter);
        glm::vec3 vert7 = vert0 + glm::vec3(diameter, diameter, diameter);

        // Now create 16 triangles
        sAABB_Triangle tempTri;

        // The order of the vertices follows a counter clockwise
        // "render" direction (in case o need it)
        tempTri.setTriangle(vert2, vert1, vert0);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert1, vert2, vert3);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert4, vert2, vert0);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert2, vert4, vert6);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert1, vert4, vert0);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert4, vert1, vert5);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert6, vert5, vert7);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert5, vert6, vert4);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert3, vert6, vert7);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert6, vert3, vert2);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert5, vert3, vert7);
        m_vDebugTri.push_back(tempTri);
        tempTri.setTriangle(vert3, vert5, vert1);
        m_vDebugTri.push_back(tempTri);
        
    }
}

float cAABBsManager::calcLongestSide(sAABB_Triangle& triangle)
{
    // Based on Feeney's code
    float AB = glm::distance(triangle.verticeA, triangle.verticeB);
    float BC = glm::distance(triangle.verticeB, triangle.verticeC);
    float AC = glm::distance(triangle.verticeA, triangle.verticeC);

    return glm::max(AB, glm::max(BC, AC));
}

bool cAABBsManager::calcID(glm::vec3 vertice, long long& ID)
{
    // Following Feeney's example of unique IDs we would have a system like:
    // _XXXXX | _YYYYY | _ZZZZZ
    // The idea is to have a long long integer that has the min position of the box 
    // coded in the integer.
    // Examples:
    // 000027000010000021 --> x = 27, y = 10, z = 21
    // 026401002314002470 --> x = 26401, y = 2314, z = 2470
    // For negative number we put a 1 before each negative position:
    // 100012000016100003 --> x = -12, y = 16, z = -3

    // Put the vertices in long long integers
    long long x = fabs(floor(vertice.x));
    long long y = fabs(floor(vertice.y));
    long long z = fabs(floor(vertice.z));
    
    // Sanity check to see whether the coordinate is too long for this method
    if (x > 99999 || y > 99999 || z > 99999)
    {
        return false;
    }
    
    // Const to multiply each integer
    const long long MULT_Z = 1;
    const long long MULT_Y = 1000000;
    const long long MULT_X = MULT_Y * MULT_Y;

    x *= MULT_X;
    y *= MULT_Y;
    z *= MULT_Z;
    
    // Finally, the ID is calculated taking into account whether posite or negative
    if (vertice.x < 0.0f)
    {
        // Put 1 before it
        // 100,000,000,000,000,000 + x
        x += 100000000000000000;
    }
    if (vertice.y < 0.0f)
    {
        // Put 1 before it
        // 100,000,000,000 + y
        y += 100000000000;
    }
    if (vertice.z < 0.0f)
    {
        // Put 1 before it
        // 100,000 + z
        z += 100000;
    }

    // Assemble the ID
    ID = x + y + z;

    return true;
}

bool cAABBsManager::tesselate(sAABB_Triangle* triangle, 
                              float size, 
                              std::vector<sAABB_Triangle>& tesseleted)
{
    float minSize = size / 2;

    // Check whether we need to tesselate it or not
    float triSize = this->calcLongestSide(*triangle);
    if (triSize <= (minSize))
    {
        return false;
    }
    
    // Based on Feeney's code:

    // Create a temp source triangle list. 
    // This will contain any triangles that are still "too big"
    std::vector<sAABB_Triangle> vecSourceTris;
    // Add the original triangle to this list
    vecSourceTris.push_back(*triangle);

    // Loop through the source list
    for (std::vector<sAABB_Triangle>::iterator itTri = vecSourceTris.begin();
        itTri != vecSourceTris.end(); itTri++)
    {
        // Is the current triangle small enough? 
        if (this->calcLongestSide(*itTri) <= minSize)
        {	// Yes, so add it to the tessellated vector
            tesseleted.push_back(*itTri);
        }
        else
        {	// Triangle is too big, so split into three triangles
            //
            //                  TA
            //                  / \
            //                 / A \
			// (TA-TB split)  X-----X (TA-TC split)
            //               / \ D / \
			//              / B \ / C \
			//            TB-----X-----TC
            //                   |
            //               (TB-TC split)
            //  
            
            sAABB_Triangle A, B, C, D;

            A.verticeA = itTri->verticeA;
            A.verticeB = this->getCentreEdge(itTri->verticeA, itTri->verticeB);
            A.verticeC = this->getCentreEdge(itTri->verticeA, itTri->verticeC);

            B.verticeA = itTri->verticeB;
            B.verticeB = this->getCentreEdge(itTri->verticeA, itTri->verticeB);
            B.verticeC = this->getCentreEdge(itTri->verticeB, itTri->verticeC);

            C.verticeA = itTri->verticeC;
            C.verticeB = this->getCentreEdge(itTri->verticeA, itTri->verticeC);
            C.verticeC = this->getCentreEdge(itTri->verticeB, itTri->verticeC);

            D.verticeA = this->getCentreEdge(itTri->verticeA, itTri->verticeB);
            D.verticeB = this->getCentreEdge(itTri->verticeA, itTri->verticeC);
            D.verticeC = this->getCentreEdge(itTri->verticeB, itTri->verticeC);

            // Note we add this to the "too big" triangles, 
            //	but we DON'T bother taking the original one off.
            // Why? Because when we increment the iterator, we will move
            //	to the 'next' triangle, anyway. 
            // Consider: If there was only 1 triangle, and we split it, 
            //	the 1st triangle is the original one, then we add 4 more,
            //	which increases the size of the vector. When we move to 
            //	the 'next' triangle, we will start processing triangle 
            //  "A" of the tessellated triangles. 
            vecSourceTris.push_back(A);
            vecSourceTris.push_back(B);
            vecSourceTris.push_back(C);
            vecSourceTris.push_back(D);
        }// if ( itTri...
    }//for ( std::vector<sTriAABB>...

    return false;
}

// From Feeney's code
glm::vec3 cAABBsManager::getCentreEdge(glm::vec3 vertice1, glm::vec3 vertice2)
{
    return (vertice1 + vertice2) / 2.0f;
}

glm::vec3 cAABBsManager::genVecFromID(long long ID)
{
    // Extract XYZ coordinates
    
    int oneBi = 1000000;
    long long oneBiSq = oneBi * oneBi;

    // For X, shifit 12 places to the right
    float x = floor(ID / oneBiSq);

    // Is it "negative"?
    if (x > 99999) x = -x;

    // For Y, we need ID - the X with all the 12 places
    // then shift 6 places to the right
    float y = floor((ID - (x * oneBiSq)) / oneBi);

    // Is it "negative"?
    if (y > 99999) y = -y;

    // For Z, we need ID - XsYs
    float z = ID - ((x * oneBiSq) + (y * oneBi));

    // Is it "negative"?
    if (z > 99999) z = -z;

    return glm::vec3(x, y, z);
}

