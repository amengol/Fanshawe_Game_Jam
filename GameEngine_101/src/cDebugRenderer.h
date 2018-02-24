#ifndef _cDebugRenderer_HG_
#define _cDebugRenderer_HG_

// This is a complete debug thing for rendering wireframe lines
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "sVAOInfo.h"		


class cDebugRenderer
{
public:
	cDebugRenderer();
	~cDebugRenderer();

	// Default maximum number of objects in buffer before a resize is forced
	static const unsigned int DEFAULTNUMBEROFTRIANGLES = 1000;
	static const unsigned int DEFAULTNUMBEROFLINES = 10000000;
	static const unsigned int DEFAULTNUMBEROFPOINTS = 1000;

	bool initialize(std::string &error);
	bool IsOK(void);

	bool resizeBufferForTriangles(unsigned int newNumberOfTriangles);
	bool resizeBufferForLines(unsigned int newNumberOfLines);
	bool resizeBufferForPoints(unsigned int newNumberOfPoints);

	// Renders scene
	void RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection);

	struct sDebugTri
	{
		sDebugTri();
		sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, bool bPersist=false);
		sDebugTri(glm::vec3 v[3], glm::vec3 colour, bool bPersist=false);
		glm::vec3 v[3];		glm::vec3 colour;
		bool bPersist;	
		bool bIgnorDepthBuffer;
	};

	struct sDebugLine
	{
		sDebugLine();
		sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, bool bPersist=false);
		sDebugLine(glm::vec3 points[2], glm::vec3 colour, bool bPersist=false);
		glm::vec3 points[2];		glm::vec3 colour;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	struct sDebugPoint
	{
		sDebugPoint();
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist=false);
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist=false);
		glm::vec3 xyz;		glm::vec3 colour;	float pointSize;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	static const float DEFAULT_POINT_SIZE;	// = 1.0f;

	void addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, bool bPersist = false);
	void addTriangle(sDebugTri &tri);
	void addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, bool bPersist = false);
	void addLine(sDebugLine &line);
	void addPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist = false);
	void addPoint(sDebugPoint &point);
	void addPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist = false);

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

private:

	std::string m_vertexShaderSource;
	std::string m_fragmentShaderSource;

	// As objects are added (to draw), they are added to these containers
	std::vector<sDebugTri> m_vecTriangles;	
	std::vector<sDebugLine> m_vecLines;		
	std::vector<sDebugPoint> m_vecPoints;	

	static const std::string DEFAULT_VERT_SHADER_SOURCE;
	static const std::string DEFAULT_FRAG_SHADER_SOURCE;

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
			pLocalVertexArray(0)
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
	};

	sVAOInfoDebug m_VAOBufferInfoTriangles;
	sVAOInfoDebug m_VAOBufferInfoLines;
	sVAOInfoDebug m_VAOBufferInfoPoints;

	// This is used to create all the buffers
	bool m_InitBuffer(sVAOInfoDebug &VAOInfo);

	// Copies the debug objects from the vectors to the vertex buffer to render
	// Will delete any "non persistent" object
	void m_copyTrianglesIntoRenderBuffer(void);
	void m_copyLinesIntoRenderBuffer(void);
	void m_copyPointsIntoRenderBuffer(void);


	// Used to hold the shader information
	// Note: it's set up this way so that we don't have to include the shader manager 
	//	in this file or set up a pimpl, etc. The cShaderProgramInfo is defined in
	//	the implementation file for the cDebugRenderer.cpp
	class cShaderProgramInfo;
	cShaderProgramInfo* m_pShaderProg;
};

#endif
