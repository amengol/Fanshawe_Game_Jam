#pragma once

#include "iDebugRenderer.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "sVAOInfo.h"		

// Note: 
// - Include this header in the thing(s) that MANAGE the debug render
//   (init, rendering, etc.)
// - Include the ++iDebugRenderer++ in the things that need to add things to draw
//

class cDebugRenderer : public iDebugRenderer
{
public:
	cDebugRenderer();
	~cDebugRenderer();

	// Default maximum number of objects in buffer before a resize is forced
	static const unsigned int DEFAULTNUMBEROFTRIANGLES = 1000;
	static const unsigned int DEFAULTNUMBEROFLINES = 1000;
	static const unsigned int DEFAULTNUMBEROFPOINTS = 1000;

	bool initialize(std::string &error);
	bool IsOK(void);

	bool resizeBufferForTriangles(unsigned int newNumberOfTriangles);
	bool resizeBufferForLines(unsigned int newNumberOfLines);
	bool resizeBufferForPoints(unsigned int newNumberOfPoints);

	std::string getLastError( bool bClearError = true );

	// Renders scene
	void RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime);

private:
	
    void m_RenderDebugTriangles(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime);
	void m_RenderDebugLines(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime);
	void m_RenderDebugPoints(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime);

public:
	
    static const float DEFAULT_POINT_SIZE;	// = 1.0f;

	virtual void addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, float lifeTime=0.0f);
	virtual void addTriangle(drTri &tri);
	virtual void addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, float lifeTime=0.0f);
	virtual void addLine(drLine &line);
	virtual void addPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime=0.0f, float pointSize=1.0f);
	virtual void addPoint(drPoint &point);

	// Replaces the DrawDebugSphere
	virtual void addDebugSphere(glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime=0.0f);

	// Various meshes that you could load and draw (are drawn with static meshes)
	// Note: the mesh is ONLY triangles, so NOT indexed (like ply, obj, etc.)
	virtual void loadDebugMesh(std::string friendlyName, std::vector<sDebugTri> &vecTris);

	unsigned int getTriangleBufferSizeInTriangles(void)	{ return this->m_VAOBufferInfoTriangles.bufferSizeObjects; }
	unsigned int getTriangleBufferSizeInBytes(void)		{ return this->m_VAOBufferInfoTriangles.bufferSizeBytes; }
	unsigned int getLineBufferSizeInLines(void)			{ return this->m_VAOBufferInfoLines.bufferSizeObjects; }
	unsigned int getLineBufferSizeInBytes(void)			{ return this->m_VAOBufferInfoLines.bufferSizeBytes; }
	unsigned int getPointBufferSizeInPoints(void)		{ return this->m_VAOBufferInfoPoints.bufferSizeObjects; }
	unsigned int getPointBufferSizeInBytes(void)		{ return this->m_VAOBufferInfoPoints.bufferSizeBytes; }

	bool getShadersUsed(std::string &vertexShaderSource, std::string &fragmentShaderSource);
	bool setVertexShader(std::string vertexShaderSource);
	bool setFragmentShader(std::string fragmentShaderSource);
	bool setShaders(std::string vertexShaderSource, std::string fragmentShaderSource);

	// Quick-n-Dirty utility to convert ply format to "flat" (only triangle) format
	bool QnD_convertIndexedXYZPlyToTriangleOnlyVertices( std::string &plyText, std::vector<sDebugTri> &vecTris );
	bool QnD_convertIndexedXYZPlyFileToTriangleOnlyVertices( std::string fileName, std::vector<sDebugTri> &vecTris );
	
    // This converts a vector of sDebugTris into a header file representation
	// "arrayName" is the name of the array and the size value:
	// - arrayName+"_array" for the array		(example: "float teapot_array[] = ...")
	// - arrayName+"_array_size" for the size	(example: "unsigned int teapot_array_size = ...")
	// NOTE: Size is the TOTAL number of floats, so 3x the number of triangles. 
	//       You pass this TOTAL number info the loadHeaderArrayInto_vecTri() method.
	// Leave outputFileName blank to NOT save to a file.
	bool QnD_convert_vecTri_to_array_header( std::vector<sDebugTri> &vecTris, std::string arrayName, std::string &arrayText, std::string outputFileName = "" );
	bool QnD_loadHeaderArrayInto_vecTri( float* shapeArray, int sizeOfArray, std::vector<sDebugTri> &vecTris );
	static const std::string DEFAULT_PLY_SPHERE_MODEL_TEXT;	

private:

	unsigned int m_RoundUpToNearest100( unsigned int value )
	{
		return (value + 100) / 100;
	}

	// As objects are added (to draw), they are added to these containers
	std::vector<drTri>   m_vecTriangles;	
	std::vector<drLine>  m_vecLines;		
	std::vector<drPoint> m_vecPoints;	
	std::vector<drMesh>  m_vecMeshes;	

	static const std::string DEFALUT_VERT_SHADER_SOURCE;
	static const std::string DEFAULT_FRAG_SHADER_SOURCE;
	static const std::string DEFAULT_GEOM_SHADER_SOURCE;

	// This is the point that's inside the vertex buffer
	struct sVertex_xyzw_rgba
	{
		sVertex_xyzw_rgba() :
			x(0.0f), y(0.0f), z(0.0f), w(1.0f),
			r(0.0f), g(0.0f), b(0.0f), a(1.0f)
		{};
		float x, y, z, w;
		float r, g, b, a;
	};
	struct sVAOInfoDebug
	{
		sVAOInfoDebug() :
			shaderID(0),
			VAO_ID(0),
			vertex_buffer_ID(0),
			numberOfVerticesToDraw(0),
			numberOfObjectsToDraw(0),
			bufferSizeBytes(0),
			bufferSizeObjects(0),
			bufferSizeVertices(0),
			pLocalVertexArray(0),
			bIsValid(false)
		{ };
		unsigned int shaderID;				// needed to get the uniforms for the VAO
		unsigned int VAO_ID;	
		unsigned int vertex_buffer_ID;		// GLuint
		// These change each frame
		unsigned int numberOfVerticesToDraw;
		unsigned int numberOfObjectsToDraw;	// points, lines, triangles
		// These are constant, the max size of buffer
		unsigned int bufferSizeBytes;
		unsigned int bufferSizeObjects;
		unsigned int bufferSizeVertices;
		sVertex_xyzw_rgba* pLocalVertexArray;
		bool bIsValid;	// used to see if this has been set up
	};

	sVAOInfoDebug m_VAOBufferInfoTriangles;
	sVAOInfoDebug m_VAOBufferInfoLines;
	sVAOInfoDebug m_VAOBufferInfoPoints;

	// This is used to create all the buffers
	bool m_InitBuffer(sVAOInfoDebug &VAOInfo);

	// Copies the debug objects from the vectors to the vertex buffer to render
	// Will delete any "non persistent" object
	void m_copyTrianglesIntoRenderBuffer(double deltaTime);
	void m_copyLinesIntoRenderBuffer(double deltaTime);
	void m_copyPointsIntoRenderBuffer(double deltaTime);

	std::map<std::string /*meshname*/, sVAOInfoDebug /*drawInfo*/> m_mapMeshNameToVAOInfo;
    
	// Used to hold the shader information
	// Note: it's set up this way so that we don't have to include the shader manager 
	//	in this file or set up a pimpl, etc. The cShaderProgramInfo is defined in
	//	the implementation file for the cDebugRenderer.cpp
	class cShaderProgramInfo;
	cShaderProgramInfo* m_pShaderProg;

	std::string m_lastError;
};
