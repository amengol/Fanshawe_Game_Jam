#ifndef _sVAOInfo_HG_
#define _sVAOInfo_HG_

#include <string>

struct sVAOInfo
{
	sVAOInfo() : VAO_ID(0), shaderID(-1), 
		         vertex_buffer_ID(0), index_buffer_ID(0),
		         numberOfVertices(0), numberOfTriangles(0),
				 numberOfIndices(0), 
				 scaleForUnitBox(1.0f)
	{ }
	std::string friendlyName;
	// OpenGL rendering specific stuff we need...
	unsigned int VAO_ID;	// **** The thing we REALLY want

	// We might need this information to draw (or "render")
	unsigned int numberOfVertices;
	unsigned int numberOfTriangles;
	unsigned int numberOfIndices;		/****/


	int shaderID;			// Perhaps?
	// Spoiler alert: we don't need these, actually...
	unsigned int vertex_buffer_ID;		// GLuint
	unsigned int index_buffer_ID;

	float scaleForUnitBox;
};


#endif

