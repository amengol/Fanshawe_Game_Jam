#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include "sVertex.h"

cVAOMeshManager::cVAOMeshManager()
{
	return;
}

cVAOMeshManager::~cVAOMeshManager()
{
	return;
}

bool cVAOMeshManager::loadMeshIntoVAO( cMesh &theMesh, int shaderID, bool bKeepMesh /*=false*/)
{
// ******************************************************************************
//__      __       _              ____         __  __          
//\ \    / /      | |            |  _ \       / _|/ _|         
// \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
//  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
//   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
//    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
//                                                             
// ******************************************************************************

	sVAOInfo theVAOInfo;

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &(theVAOInfo.VAO_ID) );
	glBindVertexArray( theVAOInfo.VAO_ID );

    glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID) );
    glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

	// Allocate the global vertex array
    sVertex* pVertices = new sVertex[theMesh.numberOfVertices];

	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		pVertices[index].x = theMesh.pVertices[index].x;
		pVertices[index].y = theMesh.pVertices[index].y;
		pVertices[index].z = theMesh.pVertices[index].z;	

		pVertices[index].r = theMesh.pVertices[index].r;
		pVertices[index].g = theMesh.pVertices[index].g;
		pVertices[index].b = theMesh.pVertices[index].b;

		pVertices[index].nx = theMesh.pVertices[index].nx;
 		pVertices[index].ny = theMesh.pVertices[index].ny;
 		pVertices[index].nz = theMesh.pVertices[index].nz;

        // Other additions: texture coords, tangent, and bi-normal
        pVertices[index].u1 = theMesh.pVertices[index].u1;
        pVertices[index].v1 = theMesh.pVertices[index].v1;
        pVertices[index].u2 = theMesh.pVertices[index].u2;
        pVertices[index].v2 = theMesh.pVertices[index].v2;

        pVertices[index].bx = theMesh.pVertices[index].bx;
        pVertices[index].by = theMesh.pVertices[index].by;
        pVertices[index].bz = theMesh.pVertices[index].bz;

        pVertices[index].tx = theMesh.pVertices[index].tx;
        pVertices[index].ty = theMesh.pVertices[index].ty;
        pVertices[index].tz = theMesh.pVertices[index].tz;
	}

	// Copy the local vertex array into the GPUs memory
	int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * theMesh.numberOfVertices;
    glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

	delete [] pVertices;

// ******************************************************************************
//  _____           _           ____         __  __          
// |_   _|         | |         |  _ \       / _|/ _|         
//   | |  _ __   __| | _____  _| |_) |_   _| |_| |_ ___ _ __ 
//   | | | '_ \ / _` |/ _ \ \/ /  _ <| | | |  _|  _/ _ \ '__|
//  _| |_| | | | (_| |  __/>  <| |_) | |_| | | | ||  __/ |   
// |_____|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_| \___|_|   
//                                                           
// ******************************************************************************

	// Create a Index Array
	glGenBuffers( 1, &(theVAOInfo.index_buffer_ID) );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID ); // It's now an Index Buffer
		
	int numberOfIndices = theMesh.numberOfTriangles * 3;
		
	unsigned int* indexArray = new unsigned int[numberOfIndices];
	
	// Copy the triangle data into this 1D array 
	int triIndex = 0;		// Index into the triangle array (from mesh)
	int indexIndex = 0;		// Index into the "vertex index" array (1D)
	for (  ;  triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3 )
	{
		indexArray[ indexIndex + 0 ] = theMesh.pTriangles[triIndex].vertex_ID_0;
		indexArray[ indexIndex + 1 ] = theMesh.pTriangles[triIndex].vertex_ID_1;
		indexArray[ indexIndex + 2 ] = theMesh.pTriangles[triIndex].vertex_ID_2;
	}
	int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, indexArray, GL_DYNAMIC_DRAW);

	// Don't need local array anymore
	delete [] indexArray;

// ******************************************************************************
	
	// At this point, the vertex and index buffers are stored on GPU

	// Now set up the vertex layout (for this shader)

	GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
    GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");
	GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");
    GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");

    // Size of the vertex we are using in the array.
    // This is called the "stride" of the vertices in the vertex buffer
    const unsigned int VERTEX_STRIDE = sizeof(sVertex);

    glEnableVertexAttribArray(vpos_location);
    const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
    glVertexAttribPointer(vpos_location, 3,
						  GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE, 
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));

    glEnableVertexAttribArray(vcol_location);
    const unsigned int OFFSET_TO_R = offsetof(sVertex, r);
    glVertexAttribPointer(vcol_location, 4, 
						  GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R)));

	glEnableVertexAttribArray(vnorm_location);
    const unsigned int OFFSET_TO_NX = offsetof(sVertex, nx);
	glVertexAttribPointer(vnorm_location, 3,
		                  GL_FLOAT,	GL_FALSE,
		                  VERTEX_STRIDE, 
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX)));

    glEnableVertexAttribArray(vUVx2_location);
    const unsigned int OFFSET_TO_UVs = offsetof(sVertex, u1);
    glVertexAttribPointer(vUVx2_location, 4,
		                  GL_FLOAT,	GL_FALSE,
		                  VERTEX_STRIDE, 
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs)));

	// Copy the information into the VAOInfo structure
	theVAOInfo.numberOfIndices   = theMesh.numberOfTriangles * 3;
	theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
	theVAOInfo.numberOfVertices  = theMesh.numberOfVertices;
	theVAOInfo.friendlyName      = theMesh.name;
	theVAOInfo.shaderID          = shaderID;

	theMesh.CalculateExtents();
	theVAOInfo.scaleForUnitBox = theMesh.scaleForUnitBox;

	// Store the VAO info by mesh name
	this->m_mapNameToVAO[ theMesh.name ] = theVAOInfo;

	// CRITICAL 
	// Unbind the VAO first!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vnorm_location);
    glDisableVertexAttribArray(vUVx2_location);

    // Save this mesh? 
    if (bKeepMesh)
    {	// Make a COPY for later...
        //std::map< std::string, cMesh > m_mapNameToMesh;
        this->m_mapNameToMesh[theMesh.name] = theMesh;
    }//if (bKeepMesh)

	return true;
}

bool cVAOMeshManager::lookupVAOFromName( std::string name, sVAOInfo &theVAOInfo )
{
	// look up in map for the name of the mesh we want to draw
	std::map< std::string, sVAOInfo >::iterator itVAO 
					= this->m_mapNameToVAO.find( name );

	// Did I find something?
	if ( itVAO == this->m_mapNameToVAO.end() )
	{
		return false;
	}

	theVAOInfo = itVAO->second;

	return true;
}

bool cVAOMeshManager::lookupMeshFromName(std::string name, cMesh &theMesh)
{
    // Search for mesh by name using iterator based find
    std::map< std::string, cMesh >::iterator itMesh =
        this->m_mapNameToMesh.find(name);

    // Find it? 
    if (itMesh == this->m_mapNameToMesh.end())
    {	// Nope. 
        return false;
    }
    // Found the mesh (DIDN'T return .end())
    theMesh = itMesh->second;		// "return" the mesh by reference 
    return true;
}
