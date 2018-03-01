#ifndef _iDebugRenderer_HG_
#define _iDebugRenderer_HG_

// This is the interface that EVERYTHING can use.
// Adds the ability to add debug items to draw

#include <string>
#include <map>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// Needed to use glm quaternions, now
#include <glm/gtx/quaternion.hpp>
#include <vector>

class iDebugRenderer
{
public:
	virtual ~iDebugRenderer() {};

	struct sDebugTri
	{
		sDebugTri();
		sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, float lifeTime=0.0f);
		sDebugTri(glm::vec3 v[3], glm::vec3 colour, float lifeTime=0.0f);
		glm::vec3 v[3];		glm::vec3 colour;
		float lifeTime;	
		bool bIgnorDepthBuffer;
	};

	struct sDebugLine
	{
		sDebugLine();
		sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, float lifeTime=0.0f);
		sDebugLine(glm::vec3 points[2], glm::vec3 colour, float lifeTime=0.0f);
		glm::vec3 points[2];		glm::vec3 colour;
		float lifeTime;
		bool bIgnorDepthBuffer;
	};

	struct sDebugPoint
	{
		sDebugPoint();
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime=0.0f, float pointSize=1.0f);
		glm::vec3 xyz;		glm::vec3 colour;	float pointSize;
		float lifeTime;
		bool bIgnorDepthBuffer;
	};

	struct sDebugMesh
	{
		sDebugMesh();
		std::string name;
		sDebugMesh(std::string name);	// Assumes a size of 1, colour white
		sDebugMesh(std::string name, glm::vec3 xyz, float lifeTime=0.0f);
		sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime=0.0f);
		sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 EulerOrientation, glm::vec3 colour, float scale, float lifeTime=0.0f);
		sDebugMesh(std::string name, glm::vec3 xyz, glm::quat qOrientation, glm::vec3 colour, float scale, float lifeTime=0.0f);
		glm::vec3 xyz; 		glm::quat qOrientation;		float scale; 
		glm::vec3 colour;	float lifeTime;
	};

	virtual void addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, float lifeTime=0.0f) = 0;
	virtual void addTriangle(sDebugTri &tri) = 0;
	virtual void addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, float lifeTime=0.0f) = 0;
	virtual void addLine(sDebugLine &line) = 0;
	virtual void addPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime=0.0f, float pointSize=1.0f) = 0;
	virtual void addPoint(sDebugPoint &point) = 0;

	//virtual void addDebugMesh(sDebugMesh &mesh) = 0;
	//virtual void addDebugMesh(std::string name, glm::vec3 xyz, float lifeTime=0.0f) = 0;
	//virtual void addDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime=0.0f) = 0;

	// Replaces the DrawDebugSphere
	virtual void addDebugSphere(glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime=0.0f) = 0;
	static const std::string DEFAULT_DEBUG_SPHERE_MESH_NAME;// = "DSPHERE";

	// Used to draw an arbitrary mesh. If 
//	virtual void addDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime=0.0f) = 0;
//	virtual void addDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, glm::vec3 EulerOrientation, float scale, float lifeTime=0.0f) = 0;
//	virtual void addDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, glm::quat qOrientation, float scale, float lifeTime=0.0f) = 0;

	// Various meshes that you could load and draw (are drawn with static meshes)
	// Note: the mesh is ONLY triangles, so not indexed
	virtual void loadDebugMesh(std::string friendlyName, std::vector<sDebugTri> &vecTris) = 0;

};

typedef iDebugRenderer::sDebugTri	drTri;
typedef iDebugRenderer::sDebugLine	drLine;
typedef iDebugRenderer::sDebugPoint drPoint;
typedef iDebugRenderer::sDebugMesh  drMesh;


#endif
