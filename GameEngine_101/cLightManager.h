#ifndef _cLightManager_HG_
#define _cLightManager_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>


class cLight
{
public:
	cLight();

	glm::vec3 position;
	glm::vec3 diffuse;		// rgb
	glm::vec3 ambient;
	glm::vec4 specular;		// Colour (xyz), intensity (w)
	glm::vec3 attenuation;	// x = constant, y = linear, z = quadratic
	glm::vec3 direction;
	glm::vec4 typeParams;	// x = type, y = distance cut-off
							// z angle1, w = angle2

	// This represents a uniform variable inside a specific shader
	// (We are only using one shader)	int ShaderlocID_position;
	int shaderlocID_position;
	int shaderlocID_diffuse;
	int shaderlocID_ambient;
	int shaderlocID_specular;
	int shaderlocID_attenuation;
	int shaderlocID_direction;
	int shaderlocID_typeParams;

	static const unsigned int DEFAULTMAXITERATIONS = 50;
	static const float DEFAULTINFINITEDISTANCE;
	static const float DEFAULDIFFUSEACCURACYTHRESHOLD;
	float calcApproxDistFromAtten( float targetLightLevel );
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy );
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy, float infiniteDistance, unsigned int maxIterations = DEFAULTMAXITERATIONS);
	static const float DEFAULTZEROTHRESHOLD;
	// If value gets below the "zeroThreshold", will be treated as zero
	
	//float calcAttenFromDist(float distance, float zeroThreshold = DEFAULTZEROTHRESHOLD);
	float calcDiffuseFromAttenByDistance(float distance, float zeroThreshold = DEFAULTZEROTHRESHOLD);

};

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	void CreateLights( int numberOfLights, bool bKeepOldValues = true );

	void LoadShaderUniformLocations(int shaderID);
	void CopyLightInformationToCurrentShader(void);

	std::vector<cLight> vecLights;
};

#endif
