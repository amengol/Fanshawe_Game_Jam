#include "cDebugRenderer.h"
#include "globalOpenGL_GLFW.h"
#include "cShaderManager.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>		
#include <fstream>

// Used to hold the default sphere shape
extern float default_sphere_array [];
extern unsigned int default_sphere_array_size;


class cDebugRenderer::cShaderProgramInfo
{
public:
	cShaderProgramInfo() :
		shaderProgramID(0), 
		matProjectionUniformLoc(-1), 
		matViewUniformLoc(-1),
		matModelUniformLoc(-1)
	{};
	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;
	cShaderManager::cShader geomShader;
	unsigned int shaderProgramID;
	// Uniforms in the shader
	int matProjectionUniformLoc;
	int matViewUniformLoc;
	int matModelUniformLoc;
};


bool cDebugRenderer::initialize(std::string &error)
{
	cShaderManager shaderManager;

	shaderManager.setBasePath( "assets/shaders/" );
	this->m_pShaderProg->vertShader.fileName = "debugVert.glsl";
	this->m_pShaderProg->geomShader.fileName = "debugGeom.glsl";
	this->m_pShaderProg->fragShader.fileName = "debugFrag.glsl";

	if ( ! shaderManager.createProgramFromFile( "debugShader", 
												this->m_pShaderProg->vertShader,
												this->m_pShaderProg->geomShader,
												this->m_pShaderProg->fragShader) )
	{
		error = "Could not create the debug shader program.\n" + shaderManager.getLastError();
		this->m_lastError = error;
		return false;
	}
	// The shader was compiled, so get the shader program number
	this->m_pShaderProg->shaderProgramID = shaderManager.getIDFromFriendlyName("debugShader");

	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg->shaderProgramID);
	this->m_pShaderProg->matModelUniformLoc = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "mModel");
	this->m_pShaderProg->matViewUniformLoc = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "mView");
	this->m_pShaderProg->matProjectionUniformLoc = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "mProjection");
	glUseProgram(0);

	// Set up the VBOs...
	if ( ! this->resizeBufferForTriangles(cDebugRenderer::DEFAULTNUMBEROFTRIANGLES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForLines(cDebugRenderer::DEFAULTNUMBEROFLINES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForPoints(cDebugRenderer::DEFAULTNUMBEROFPOINTS) )
	{
		return false;
	}
	
	// Load the default sphere
	std::vector<sDebugTri> vecSphereTris;
	this->QnD_loadHeaderArrayInto_vecTri( default_sphere_array, default_sphere_array_size, vecSphereTris);
	this->loadDebugMesh("sphere", vecSphereTris);

	return true;
}

bool cDebugRenderer::resizeBufferForPoints(unsigned int newNumberOfPoints)
{
	//TODO
	return true;
}

bool cDebugRenderer::resizeBufferForLines(unsigned int newNumberOfLines)
{
	//TODO
	return true;
}

bool cDebugRenderer::resizeBufferForTriangles(unsigned int newNumberOfTriangles)
{
	// Erase any exisiting buffers 
	if ( this->m_VAOBufferInfoTriangles.bIsValid )
	{	// Assume it exists, so delete it
		delete [] this->m_VAOBufferInfoTriangles.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoTriangles.vertex_buffer_ID) );

		glDeleteVertexArrays( 1, &(this->m_VAOBufferInfoTriangles.VAO_ID) );
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfTriangles = (unsigned int)(newNumberOfTriangles * 1.1);	

	this->m_VAOBufferInfoTriangles.bufferSizeObjects = newNumberOfTriangles;
	this->m_VAOBufferInfoTriangles.bufferSizeVertices = newNumberOfTriangles * 3;
	this->m_VAOBufferInfoTriangles.bufferSizeBytes = 0;
	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoTriangles.shaderID = this->m_pShaderProg->shaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoTriangles);
}

bool cDebugRenderer::m_InitBuffer(sVAOInfoDebug &VAOInfo)
{
	glUseProgram(this->m_pShaderProg->shaderProgramID);

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &(VAOInfo.VAO_ID) );
	glBindVertexArray( VAOInfo.VAO_ID );


    glGenBuffers(1, &(VAOInfo.vertex_buffer_ID) );
    glBindBuffer(GL_ARRAY_BUFFER, VAOInfo.vertex_buffer_ID);

	VAOInfo.pLocalVertexArray = new sVertex_xyzw_rgba[VAOInfo.bufferSizeVertices];
	
	// Clear buffer
	VAOInfo.bufferSizeBytes = sizeof(sVertex_xyzw_rgba) * VAOInfo.bufferSizeVertices;
	memset(VAOInfo.pLocalVertexArray, 0, VAOInfo.bufferSizeBytes);

	// Copy the local vertex array into the GPUs memory
    glBufferData(GL_ARRAY_BUFFER, 
				 VAOInfo.bufferSizeBytes,		
				 VAOInfo.pLocalVertexArray,
				 GL_DYNAMIC_DRAW);

	// **DON'T** Get rid of local vertex array!
	// (We will need to copy the debug objects into this later!!)
	//delete [] pVertices;

	// Now set up the vertex layout (for this shader):
	//
    //	in vec4 vPosition;                           
    //	in vec4 vColour;                               
	//
	GLuint vpos_location = glGetAttribLocation(VAOInfo.shaderID, "vPosition");		// program, "vPos");	// 6
    GLuint vcol_location = glGetAttribLocation(VAOInfo.shaderID, "vColour");		// program, "vCol");	// 13

	// Size of the vertex we are using in the array.
	// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof(sVertex_xyzw_rgba);

    glEnableVertexAttribArray(vpos_location);
	const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, x );
    glVertexAttribPointer(vpos_location, 
						  4,					//	in vec4 vPosition; 	
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)) );	// 64-bit


    glEnableVertexAttribArray(vcol_location);
	const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, r );
    glVertexAttribPointer(vcol_location, 
						  4,					//	in vec4 vColour; 
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,		
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R_IN_CVERTEX)) );	// 64-bit
	// *******************************************************************

	VAOInfo.bIsValid = true;

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);

	glUseProgram(0);

	return true;
}

bool cDebugRenderer::IsOK(void)
{
	//TODO
	return true;
}


cDebugRenderer::cDebugRenderer()
{
	this->m_pShaderProg = new cShaderProgramInfo();
	return;
}

cDebugRenderer::~cDebugRenderer()
{
	if (this->m_pShaderProg)
	{
		delete this->m_pShaderProg;
	}
	return;
}


void cDebugRenderer::RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	this->m_copyTrianglesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugTriangles(matCameraView, matProjection, deltaTime);

	this->m_copyLinesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugLines(matCameraView, matProjection, deltaTime);

	this->m_copyPointsIntoRenderBuffer(deltaTime);
	this->m_RenderDebugPoints(matCameraView, matProjection, deltaTime);

	return;
}


void cDebugRenderer::m_RenderDebugTriangles(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// Start rendering 
	glUseProgram(this->m_pShaderProg->shaderProgramID);

	glUniformMatrix4fv( this->m_pShaderProg->matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_pShaderProg->matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_pShaderProg->matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(50.0f);

	// Draw triangles
	glBindVertexArray( this->m_VAOBufferInfoTriangles.VAO_ID );
	glDrawArrays( GL_TRIANGLES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw );
	glBindVertexArray( 0 );

	// Draw lines

	// Draw points

	glUseProgram(0);

	// Put everything back as it was 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);		

	return;
}

void cDebugRenderer::m_RenderDebugLines(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// TODO: 

	return;
}

void cDebugRenderer::m_RenderDebugPoints(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// TODO:
	
	return;
}


void cDebugRenderer::m_copyTrianglesIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drTri> vecTriTemp;

	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = (unsigned int)this->m_vecTriangles.size();

	// Is the draw buffer big enough? 
	if ( this->m_VAOBufferInfoTriangles.bufferSizeObjects < this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw )
	{
		// Resize the buffer
		this->resizeBufferForTriangles( this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw );
	}

	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw
		= this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw * 3;	// Triangles

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int triIndex = 0;		// index of the triangle buffer
	for (; triIndex != this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw; 
		   triIndex++, vertexIndex += 3)
	{
		drTri& curTri = this->m_vecTriangles[triIndex];
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].x = curTri.v[0].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].y = curTri.v[0].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].z = curTri.v[0].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].x = curTri.v[1].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].y = curTri.v[1].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].z = curTri.v[1].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].x = curTri.v[2].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].y = curTri.v[2].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].z = curTri.v[2].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].a = 1.0f;

		curTri.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curTri.lifeTime > 0.0f)
		{
			vecTriTemp.push_back(curTri);
		}
	}//for (; 


	// Clear the triangle list and push back the persistent ones
	this->m_vecTriangles.clear();
	for (std::vector<drTri>::iterator itTri = vecTriTemp.begin(); itTri != vecTriTemp.end(); itTri++)
	{
		this->m_vecTriangles.push_back(*itTri);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoTriangles.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
		numberOfBytesToCopy,
		this->m_VAOBufferInfoTriangles.pLocalVertexArray,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
		error = COpenGLError::TranslateErrorEnum(err);
	}



	return;
}


void cDebugRenderer::m_copyLinesIntoRenderBuffer(double deltaTime)
{
	//TODO:

	return;
}

void cDebugRenderer::m_copyPointsIntoRenderBuffer(double deltaTime)
{
	// TODO: 

	return;
}

void cDebugRenderer::addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drTri tempTri(v1XYZ, v2XYZ, v3XYZ, colour, lifeTime);
	this->addTriangle(tempTri);
	return;
}


void cDebugRenderer::addTriangle(drTri &tri)
{
	this->m_vecTriangles.push_back(tri);
	return;
}

void cDebugRenderer::addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drLine tempLine(startXYZ, endXYZ, colour, lifeTime);
	this->addLine(tempLine);
	return;
}

void cDebugRenderer::addLine(drLine &line)
{
	this->m_vecLines.push_back(line);
	return;
}

void cDebugRenderer::addPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/, float pointSize/*=1.0f*/)
{
	drPoint tempPoint(xyz, colour, lifeTime, pointSize);
	this->addPoint(tempPoint);
	return;
}

void cDebugRenderer::addPoint(drPoint &point)
{
	this->m_vecPoints.push_back(point);
	return;
}

// Replaces the DrawDebugSphere
void cDebugRenderer::addDebugSphere(glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	iDebugRenderer::sDebugMesh sphereMesh;
	sphereMesh.name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;
	sphereMesh.scale = scale;
	sphereMesh.xyz = xyz;
	sphereMesh.colour = colour;
	sphereMesh.lifeTime = lifeTime;
	this->m_vecMeshes.push_back(sphereMesh);
	return;
}

std::string cDebugRenderer::getLastError(bool bClearError /*=true*/)
{
	std::string theLastError = this->m_lastError;
	if ( bClearError )
	{
		this->m_lastError = "";
	}
	return theLastError;
}



// *********************************************************
//	 ___      _                ___ _                      
//	|   \ ___| |__ _  _ __ _  / __| |_  __ _ _ __  ___ ___
//	| |) / -_) '_ \ || / _` | \__ \ ' \/ _` | '_ \/ -_|_-<
//	|___/\___|_.__/\_,_\__, | |___/_||_\__,_| .__/\___/__/
//	                   |___/                |_|           
//

//static 
const std::string iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME = "DSPHERE";

iDebugRenderer::sDebugTri::sDebugTri()
{
	this->v[0] = glm::vec3(0.0f); this->v[1] = glm::vec3(0.0f); this->v[2] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v[3], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine()
{
	this->points[0] = glm::vec3(0.0f);
	this->points[0] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);		// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 points[2], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBuffer = false;
	return;
}

/*static*/ 
const float cDebugRendererDEFAULT_POINT_SIZE = 1.0f;


iDebugRenderer::sDebugPoint::sDebugPoint()
{
	this->xyz = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->pointSize = cDebugRendererDEFAULT_POINT_SIZE;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/, float pointSize/*=1.0f*/)
{
	this->xyz = xyz;
	this->colour = colour;
	this->pointSize = pointSize;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBuffer = false;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh()
{
	this->name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;	// = "DSPHERE";
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	return;
}

// Assumes a size of 1, colour white
iDebugRenderer::sDebugMesh::sDebugMesh(std::string name)
{
	this->name = name;
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 EulerOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(EulerOrientation);
	this->scale = scale;
	this->colour = colour;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::quat qOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = qOrientation;
	this->scale = scale;
	this->colour = colour;
	return;
}