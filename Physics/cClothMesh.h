#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <vector>

namespace nPhysics
{
    struct sClothVertex
    {
        sClothVertex() :
            x(0.0f), y(0.0f), z(0.0f),
            r(0.0f), g(0.0f), b(0.0f), a(1.0f),
            nx(0.0f), ny(0.0f), nz(0.0f),
            u1(0.0f), v1(0.0f)
        {};

        float x, y, z;
        float r, g, b, a;
        float nx, ny, nz;
        float u1, v1;
    };

    class cClothTriangle
    {
    public:
        cClothTriangle() : vertex_ID_0(-1), vertex_ID_1(-1), vertex_ID_2(-1) { }
        int vertex_ID_0;
        int vertex_ID_1;
        int vertex_ID_2;
    };

    class cClothMesh
    {
    public:
        cClothMesh();
        ~cClothMesh();

        // the friendly name of the mesh
        std::string name;

        sClothVertex* pVertices;
        int numberOfVertices;

        cClothTriangle* pTriangles;
        int numberOfTriangles;

        // "Bounding box" edges
        glm::vec3 minXYZ;
        glm::vec3 maxXYZ;

        // This is Max - Min for each x,y,z
        glm::vec3 maxExtentXYZ;

        // Biggest extent (x, y or z)
        float maxExtent;

        float scaleForUnitBox;

        void CalculateExtents(void);

        void CalculateNormals(void);

    };
}