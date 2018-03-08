#include "stdafx.h"
#include "cCloth.h"
#include "cNode.h"
#include <glm\glm.hpp>

nPhysics::cCloth::cCloth(glm::vec3 upLeftPostion,
                         float damping,
                         float nodeMass,
                         float width, 
                         float height, 
                         int numNodesWidth, 
                         int numNodesHeight)
{
    mNumNodesWidth = numNodesWidth;
    mNumNodesHeight = numNodesHeight;
    mWind = glm::vec3(0.0f);
    mHasWind = false;
    mGravity = glm::vec3(0.0f);
    mHasGravity = false;

    mNodes.resize(mNumNodesWidth * mNumNodesHeight);

    // Creating nodes in a grid from (0,0,0) to (width,-height,0)
    for (int x = 0; x < numNodesWidth; x++)
    {
        for (int y = 0; y < numNodesHeight; y++)
        {
            glm::vec3 pos = glm::vec3(width * (x / (float)numNodesWidth),
                                      -height * (y / (float)numNodesHeight),
                                      0.0f) + upLeftPostion;
            // insert node in column x at y'th row
            mNodes[y * numNodesWidth + x] = cNode(pos, damping, nodeMass); 
        }
    }

    // Connecting immediate neighbor nodes with springs (distance 1 and sqrt(2) in the grid)
    for (int x = 0; x < numNodesWidth; x++)
    {
        for (int y = 0; y < numNodesHeight; y++)
        {
            // Horizontal
            if ( x < numNodesWidth - 1) 
                makeSpring(getNode(x, y), getNode(x + 1, y));
            // Vertical
            if ( y < numNodesHeight - 1) 
                makeSpring(getNode(x, y), getNode(x, y + 1));
            // Diagonal
            if (x < numNodesWidth - 1 && y < numNodesHeight - 1)
            {
                makeSpring(getNode(x, y), getNode(x + 1, y + 1));
                makeSpring(getNode(x + 1, y), getNode(x, y + 1));
            }
        }
    }
    
    // Connecting secondary neighbors with springs (distance 2 and sqrt(4) in the grid)
    for (int x = 0; x < numNodesWidth; x++)
    {
        for (int y = 0; y < numNodesHeight; y++)
        {
            if (x < numNodesWidth - 2) 
                makeSpring(getNode(x, y), getNode(x + 2, y));
            if (y < numNodesHeight - 2) 
                makeSpring(getNode(x, y), getNode(x, y + 2));
            if (x < numNodesWidth - 2 && y < numNodesHeight - 2)
            {
                makeSpring(getNode(x, y), getNode(x + 2, y + 2));
                makeSpring(getNode(x + 2, y), getNode(x, y + 2));
            }
        }
    }
    
    // making the upper left and right most two particles unmovable
    for (int i = 0; i < 2; i++)
    {
        getNode(0 + i, 0)->makeUnmovable();
        getNode(mNumNodesWidth - 1 - i, 0)->makeUnmovable();
    }
}

void nPhysics::cCloth::SetWind(glm::vec3& wind)
{
    mWind = wind;
    mHasWind = true;
}

void nPhysics::cCloth::ClearWind()
{
    mWind = glm::vec3(0.0f);
    mHasWind = false;
}

void nPhysics::cCloth::SetGravity(glm::vec3& gravity)
{
    mGravity = gravity;
    mHasGravity = true;
}

void nPhysics::cCloth::ClearGravity()
{
    mGravity = glm::vec3(0.0f);
    mHasGravity = false;
}

void nPhysics::cCloth::TimeStep(float deltaTime, size_t iterations)
{
    std::vector<cSpring>::iterator itSpring;
    for (int i = 0; i < iterations; i++) // iterate over all springs several times
    {
        for (itSpring = mSprings.begin(); itSpring != mSprings.end(); itSpring++)
        {
            (*itSpring).restoreSpring();
        }
    }

    std::vector<cNode>::iterator itNode;
    for (itNode = mNodes.begin(); itNode != mNodes.end(); itNode++)
    {
        (*itNode).timeStep(deltaTime);
    }

    stepWind(deltaTime);
    stepGravity(deltaTime);
}

void nPhysics::cCloth::SphereCollision(const glm::vec3 center, const float radius)
{
    std::vector<cNode>::iterator itNodes;
    for (itNodes = mNodes.begin(); itNodes != mNodes.end(); itNodes++)
    {
        glm::vec3 v = (*itNodes).getPos() - center;
        float vecLength = glm::length(v);
        if (vecLength < radius) // if the particle is inside the sphere
        {
            // project the particle to the surface of the sphere
            (*itNodes).offsetPos(glm::normalize(v)*(radius - vecLength));
        }
    }
}

void nPhysics::cCloth::GetClothMesh(cClothMesh& theMesh)
{
    theMesh.name = "Cloth";

    // reset normals (which where written to last frame)
    std::vector<cNode>::iterator itNodes;
    for (itNodes = mNodes.begin(); itNodes != mNodes.end(); itNodes++)
    {
        (*itNodes).resetNormal();
    }

    //create smooth per node normals by adding up all the 
    // (hard) triangle normals that each node it is part of
    for (int x = 0; x< mNumNodesWidth - 1; x++)
    {
        for (int y = 0; y< mNumNodesHeight - 1; y++)
        {
            glm::vec3 normal = calcTriangleNormal(getNode(x + 1, y), getNode(x, y), getNode(x, y + 1));
            getNode(x + 1, y)->addToNormal(normal);
            getNode(x, y)->addToNormal(normal);
            getNode(x, y + 1)->addToNormal(normal);

            normal = calcTriangleNormal(getNode(x + 1, y + 1), getNode(x + 1, y), getNode(x, y + 1));
            getNode(x + 1, y + 1)->addToNormal(normal);
            getNode(x + 1, y)->addToNormal(normal);
            getNode(x, y + 1)->addToNormal(normal);
        }
    }

    theMesh.numberOfVertices = (mNumNodesWidth - 1) * (mNumNodesHeight - 1) * 6;
    sClothVertex* pVertices = new sClothVertex[theMesh.numberOfVertices];
    theMesh.numberOfTriangles = (mNumNodesWidth - 1) * (mNumNodesHeight - 1) * 2;
    cClothTriangle* pTriangles = new cClothTriangle[theMesh.numberOfTriangles];

    int vertexIndex = 0;                // Vertex index
    int triangleIndex = 0;              // Triangle index
    int maxNum_U = mNumNodesWidth - 1;  // Maximum number of nodes in U direction
    int textCoordIndex_U = 0;           // The index of U (horizontal direction)
    int maxNum_V = mNumNodesHeight - 1; // Maximum number of nodes in V direction
    int textCoordIndex_V = maxNum_V;    // The index of V (vertical direction)

    for (int x = 0; x < mNumNodesWidth - 1; x++)
    {
        for (int y = 0; y < mNumNodesHeight - 1; y++)
        {
            // The first triangle
            //-------------------------------------------------------------
            // First vertice                
            glm::vec3 partPos = getNode(x + 1, y)->getPos();
            pVertices[vertexIndex + 0].x = partPos.x;
            pVertices[vertexIndex + 0].y = partPos.y;
            pVertices[vertexIndex + 0].z = partPos.z;

            glm::vec3 partNormal = glm::normalize(getNode(x + 1, y)->getNormal());
            pVertices[vertexIndex + 0].nx = partNormal.x;
            pVertices[vertexIndex + 0].ny = partNormal.y;
            pVertices[vertexIndex + 0].nz = partNormal.z;

            pVertices[vertexIndex + 0].r = 0.0f;
            pVertices[vertexIndex + 0].g = 0.0f;
            pVertices[vertexIndex + 0].b = 0.0f;
            pVertices[vertexIndex + 0].a = 1.0f;

            pVertices[vertexIndex + 0].u1 = (textCoordIndex_U + 1) / (float)maxNum_U;
            pVertices[vertexIndex + 0].v1 = (textCoordIndex_V + 1) / (float)maxNum_V;

            pTriangles[triangleIndex + 0].vertex_ID_0 = vertexIndex + 0;

            // Second vertice                
            partPos = getNode(x, y)->getPos();
            pVertices[vertexIndex + 1].x = partPos.x;
            pVertices[vertexIndex + 1].y = partPos.y;
            pVertices[vertexIndex + 1].z = partPos.z;

            partNormal = glm::normalize(getNode(x, y)->getNormal());
            pVertices[vertexIndex + 1].nx = partNormal.x;
            pVertices[vertexIndex + 1].ny = partNormal.y;
            pVertices[vertexIndex + 1].nz = partNormal.z;

            pVertices[vertexIndex + 1].r = 0.0f;
            pVertices[vertexIndex + 1].g = 0.0f;
            pVertices[vertexIndex + 1].b = 0.0f;
            pVertices[vertexIndex + 1].a = 1.0f;

            pVertices[vertexIndex + 1].u1 = (textCoordIndex_U + 0) / (float)maxNum_U;
            pVertices[vertexIndex + 1].v1 = (textCoordIndex_V + 1) / (float)maxNum_V;

            pTriangles[triangleIndex + 0].vertex_ID_1 = vertexIndex + 1;

            // Third vertice                
            partPos = getNode(x, y + 1)->getPos();
            pVertices[vertexIndex + 2].x = partPos.x;
            pVertices[vertexIndex + 2].y = partPos.y;
            pVertices[vertexIndex + 2].z = partPos.z;

            partNormal = glm::normalize(getNode(x, y + 1)->getNormal());
            pVertices[vertexIndex + 2].nx = partNormal.x;
            pVertices[vertexIndex + 2].ny = partNormal.y;
            pVertices[vertexIndex + 2].nz = partNormal.z;

            pVertices[vertexIndex + 2].r = 0.0f;
            pVertices[vertexIndex + 2].g = 0.0f;
            pVertices[vertexIndex + 2].b = 0.0f;
            pVertices[vertexIndex + 2].a = 1.0f;

            pVertices[vertexIndex + 2].u1 = (textCoordIndex_U + 0) / (float)maxNum_U;
            pVertices[vertexIndex + 2].v1 = (textCoordIndex_V + 0) / (float)maxNum_V;

            pTriangles[triangleIndex + 0].vertex_ID_2 = vertexIndex + 2;

            // The Second triangle
            //-------------------------------------------------------------
            // First vertice                
            partPos = getNode(x + 1, y + 1)->getPos();
            pVertices[vertexIndex + 3].x = partPos.x;
            pVertices[vertexIndex + 3].y = partPos.y;
            pVertices[vertexIndex + 3].z = partPos.z;

            partNormal = glm::normalize(getNode(x + 1, y + 1)->getNormal());
            pVertices[vertexIndex + 3].nx = partNormal.x;
            pVertices[vertexIndex + 3].ny = partNormal.y;
            pVertices[vertexIndex + 3].nz = partNormal.z;

            pVertices[vertexIndex + 3].r = 0.0f;
            pVertices[vertexIndex + 3].g = 0.0f;
            pVertices[vertexIndex + 3].b = 0.0f;
            pVertices[vertexIndex + 3].a = 1.0f;

            pVertices[vertexIndex + 3].u1 = (textCoordIndex_U + 1) / (float)maxNum_U;
            pVertices[vertexIndex + 3].v1 = (textCoordIndex_V + 0) / (float)maxNum_V;

            pTriangles[triangleIndex + 1].vertex_ID_0 = vertexIndex + 3;

            // Second vertice                
            partPos = getNode(x + 1, y)->getPos();
            pVertices[vertexIndex + 4].x = partPos.x;
            pVertices[vertexIndex + 4].y = partPos.y;
            pVertices[vertexIndex + 4].z = partPos.z;

            partNormal = glm::normalize(getNode(x + 1, y)->getNormal());
            pVertices[vertexIndex + 4].nx = partNormal.x;
            pVertices[vertexIndex + 4].ny = partNormal.y;
            pVertices[vertexIndex + 4].nz = partNormal.z;

            pVertices[vertexIndex + 4].r = 0.0f;
            pVertices[vertexIndex + 4].g = 0.0f;
            pVertices[vertexIndex + 4].b = 0.0f;
            pVertices[vertexIndex + 4].a = 1.0f;

            pVertices[vertexIndex + 4].u1 = (textCoordIndex_U + 1) / (float)maxNum_U;
            pVertices[vertexIndex + 4].v1 = (textCoordIndex_V + 1) / (float)maxNum_V;

            pTriangles[triangleIndex + 1].vertex_ID_1 = vertexIndex + 4;

            // Third vertice                
            partPos = getNode(x, y + 1)->getPos();
            pVertices[vertexIndex + 5].x = partPos.x;
            pVertices[vertexIndex + 5].y = partPos.y;
            pVertices[vertexIndex + 5].z = partPos.z;

            partNormal = glm::normalize(getNode(x, y + 1)->getNormal());
            pVertices[vertexIndex + 5].nx = partNormal.x;
            pVertices[vertexIndex + 5].ny = partNormal.y;
            pVertices[vertexIndex + 5].nz = partNormal.z;

            pVertices[vertexIndex + 5].r = 0.0f;
            pVertices[vertexIndex + 5].g = 0.0f;
            pVertices[vertexIndex + 5].b = 0.0f;
            pVertices[vertexIndex + 5].a = 1.0f;

            pVertices[vertexIndex + 5].u1 = (textCoordIndex_U + 0) / (float)maxNum_U;
            pVertices[vertexIndex + 5].v1 = (textCoordIndex_V + 0) / (float)maxNum_V;

            pTriangles[triangleIndex + 1].vertex_ID_2 = vertexIndex + 5;

            vertexIndex += 6;
            triangleIndex += 2;
            textCoordIndex_V--;
        }
        textCoordIndex_U++;
    }
    theMesh.pVertices = pVertices;
    theMesh.pTriangles = pTriangles;
}

glm::vec3 nPhysics::cCloth::calcTriangleNormal(cNode* n1, cNode* n2, cNode* n3)
{
    glm::vec3 pos1 = n1->getPos();
    glm::vec3 pos2 = n2->getPos();
    glm::vec3 pos3 = n3->getPos();

    glm::vec3 v1 = pos2 - pos1;
    glm::vec3 v2 = pos3 - pos1;

    return glm::cross(v1, v2);
}

void nPhysics::cCloth::addWindForcesForTriangle(cNode* n1, cNode* n2, cNode* n3, const glm::vec3 direction)
{
    glm::vec3 normal = calcTriangleNormal(n1, n2, n3);
    glm::vec3 d = glm::normalize(normal);
    glm::vec3 force = normal * (glm::dot(d, direction));
    n1->addForce(force);
    n2->addForce(force);
    n3->addForce(force);
}

void nPhysics::cCloth::stepWind(float deltaTime)
{
    if (mHasWind)
    {
        for (int x = 0; x < mNumNodesWidth - 1; x++)
        {
            for (int y = 0; y < mNumNodesHeight - 1; y++)
            {
                addWindForcesForTriangle(getNode(x + 1, y), getNode(x, y), getNode(x, y + 1), mWind * deltaTime);
                addWindForcesForTriangle(getNode(x + 1, y + 1), getNode(x + 1, y), getNode(x, y + 1), mWind * deltaTime);
            }
        }
    }
}

void nPhysics::cCloth::stepGravity(float deltaTime)
{
    if (mHasGravity)
    {
        std::vector<cNode>::iterator itNode;
        for (itNode = mNodes.begin(); itNode != mNodes.end(); itNode++)
        {
            (*itNode).addForce(mGravity * deltaTime);
        }
    }
}
