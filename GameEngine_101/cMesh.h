#ifndef _cMesh_HG_
#define _cMesh_HG_

#include "sVertex.h"
#include "cTriangle.h" 
#include <string>
#include <glm/vec3.hpp>
#include <vector>

// Stores information about the loaded mesh like vertices and triangles indices
class cMesh
{
public:
	cMesh();
	~cMesh();

	// the friendly name of the mesh
	std::string name;

	sVertex* pVertices;
	int numberOfVertices;

	cTriangle* pTriangles;
	int numberOfTriangles;

	// Used for the physics calculations
	std::vector< cPhysTriangle > vecPhysTris;
	void GeneratePhysicsTriangles(void);


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
#endif
