#include "cLightManager.h"
#include <vector>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

cLight::cLight()
{
	this->position = glm::vec3(0.0f);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    this->typeParams2 = glm::vec4(1.0f);
	
	// Colour (xyz), intensity (w)
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// attenuation initialization
	this->attenuation = glm::vec3(0.0f);
	// x = constant, y = linear, z = quadratic
	this->attenuation.x = 0.0f;	// Constant atten
	this->attenuation.y = 1.0f;	// Linear
	this->attenuation.z = 0.0f;	// Quadratic

	this->direction = glm::vec3(0.0f);

	// x = type, y = distance cut-off, z angle1, w = angle2
	this->typeParams = glm::vec4(0.0f);
	// Set distance to infinity
	// (i.e. at this point, the light won't be calculated)
	this->typeParams.y = 1000000.0f;		// Huge number

	this->shaderlocID_position = -1;
	this->shaderlocID_diffuse = -1;
	this->shaderlocID_ambient = -1;
	this->shaderlocID_specular = -1;
	this->shaderlocID_attenuation = -1;
	this->shaderlocID_direction = -1;
	this->shaderlocID_typeParams = -1;
    this->shaderlocID_typeParams2 = -1;

	return;
}

cLightManager::cLightManager()
{

	return;
}

cLightManager::~cLightManager()
{

	return;
}

void cLightManager::CreateLights( int numberOfLights, bool bKeepOldValues )
{
	int howManyLightsToAdd = numberOfLights - (int)this->vecLights.size(); 
	howManyLightsToAdd = abs(howManyLightsToAdd);
	// Resize the vector
	this->vecLights.resize( howManyLightsToAdd, NULL );

	// If we DON'T want the original values, then clear the old ones
    if (!bKeepOldValues)
    {
        for (int index = 0; index != this->vecLights.size(); index++)
        {
            delete this->vecLights[index];
            this->vecLights[index] = new cLight();
        }
    }
    else
    {
        for (int index = 0; index != this->vecLights.size(); index++)
        {
            this->vecLights[index] = new cLight();
        }
    }
	return;
}

void cLightManager::CreateLights(std::vector<cLight*> lights)
{
    for (size_t i = 0; i < lights.size(); i++)
    {
        this->vecLights.push_back(lights[i]);
    }
}

std::string genUniName( int lightIndex, std::string paramName )
{
	// uniform vec4 myLight[0].position;
	std::stringstream ssUniName;
	ssUniName << "myLight[" << lightIndex << "]." << paramName;
	return ssUniName.str();
}


void cLightManager::LoadShaderUniformLocations(int shaderID)
{
	for ( int index = 0; index != this->vecLights.size(); index++ )
	{
		//"myLight[0].position"
		this->vecLights[index]->shaderlocID_position 
			= glGetUniformLocation( shaderID, genUniName(index, "position").c_str() );

		this->vecLights[index]->shaderlocID_diffuse = glGetUniformLocation( shaderID, genUniName(index, "diffuse").c_str() );
		this->vecLights[index]->shaderlocID_ambient = glGetUniformLocation( shaderID, genUniName(index, "ambient").c_str() );
		this->vecLights[index]->shaderlocID_specular = glGetUniformLocation( shaderID, genUniName(index, "specular").c_str() );
		this->vecLights[index]->shaderlocID_attenuation = glGetUniformLocation( shaderID, genUniName(index, "attenuation").c_str() );
		this->vecLights[index]->shaderlocID_direction = glGetUniformLocation( shaderID, genUniName(index, "direction").c_str() );
		this->vecLights[index]->shaderlocID_typeParams = glGetUniformLocation( shaderID, genUniName(index, "typeParams").c_str() );
        this->vecLights[index]->shaderlocID_typeParams2 = glGetUniformLocation(shaderID, genUniName(index, "typeParams2").c_str());
	}
	return;
}

void cLightManager::CopyLightInformationToCurrentShader(void)
{
	for ( int index = 0; index != this->vecLights.size(); index++ )
	{
		cLight* pCurLight = this->vecLights[index];

		glUniform4f( pCurLight->shaderlocID_position, 
					 pCurLight->position.x, 
					 pCurLight->position.y, 
					 pCurLight->position.z,
					 1.0f);

		glUniform4f( pCurLight->shaderlocID_diffuse, 
					 pCurLight->diffuse.r, 
					 pCurLight->diffuse.g, 
					 pCurLight->diffuse.b,
					 1.0f);
		
		glUniform4f( pCurLight->shaderlocID_ambient, 
					 pCurLight->ambient.r, 
					 pCurLight->ambient.g,
					 pCurLight->ambient.b, 
					 1.0f);

		glUniform4f( pCurLight->shaderlocID_specular,
					 pCurLight->specular.r,
					 pCurLight->specular.g,
					 pCurLight->specular.b,
					 pCurLight->specular.w );

		glUniform4f( pCurLight->shaderlocID_attenuation, 
					 pCurLight->attenuation.x,
					 pCurLight->attenuation.y,
					 pCurLight->attenuation.z, 
					 1.0f);

		glUniform4f( pCurLight->shaderlocID_direction,
					 pCurLight->direction.x, 
					 pCurLight->direction.y, 
					 pCurLight->direction.z,
					 1.0f);

		// x = type, y = distance cut-off
		// z angle1, w = angle2		
		glUniform4f( pCurLight->shaderlocID_typeParams, 
					 pCurLight->typeParams.x, 
					 pCurLight->typeParams.y, 
					 pCurLight->typeParams.z, 
					 pCurLight->typeParams.w );

        glUniform4f(pCurLight->shaderlocID_typeParams2,
                    pCurLight->typeParams2.x,
                    pCurLight->typeParams2.y,
                    pCurLight->typeParams2.z,
                    pCurLight->typeParams2.w);
	}// for ( int index = 0;...

	return;
}

//static 
const float cLight::DEFAULTINFINITEDISTANCE = 10000.0f;	

//static 
const float cLight::DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

float cLight::calcApproxDistFromAtten( float targetLightLevel )
{
	return this->calcApproxDistFromAtten( targetLightLevel, DEFAULDIFFUSEACCURACYTHRESHOLD );
}

float cLight::calcApproxDistFromAtten(float targetLightLevel, float accuracy )
{
	return this->calcApproxDistFromAtten( targetLightLevel, accuracy, DEFAULTINFINITEDISTANCE, DEFAULTMAXITERATIONS );
}
                                                      
float cLight::calcApproxDistFromAtten(float targetLightLevel, float accuracy, 
                                      float infiniteDistance, unsigned int maxIterations )
{
	// See if the accuracy being set it too big for the targetLightLevel, unless targetLightLevel 
	// is actually zero (0.0f). If it's actually zero, then adjusting the accuracy to a tenth of 
	// zero would give zero, and we would max out the iterations
	if ( targetLightLevel != 0.0f )	
	{	
		if ( ( accuracy * 10.0f) >= targetLightLevel * 10.0f )
		{	// Adjust the accuracy by a hundreth
			accuracy = targetLightLevel / 10.0f;
		}
	}	

	float targetLightLevelLow = targetLightLevel - accuracy;
	float targetLightLevelHigh = targetLightLevel + accuracy;

	// See if we're getting a value at infinite. i.e. at 'infinite distance', is the light level too high already
	if ( this->calcDiffuseFromAttenByDistance( DEFAULTINFINITEDISTANCE, accuracy ) > targetLightLevelHigh )
	{	// Yes, so we can never get down to this light level
		return DEFAULTINFINITEDISTANCE;
	}

	// There is a light level somewhere between a distance of 0.0 to DEFAULTINFINITEDISTANCE
	// Set initial guesses
	float distanceGuessLow = 0.0f;
	float distanceGuessHigh = DEFAULTINFINITEDISTANCE;

	unsigned int iterationCount = 0;
	while ( iterationCount < maxIterations )
	{
		// Pick a distance between the high and low
		float curDistanceGuess = ( (distanceGuessHigh - distanceGuessLow) / 2.0f ) + distanceGuessLow;
		
		float curDiffuseAtGuessDistance = this->calcDiffuseFromAttenByDistance( curDistanceGuess, DEFAULTZEROTHRESHOLD );
		// Could be three possibilities: too low, too high, or in between
		if ( curDiffuseAtGuessDistance < targetLightLevelLow )
		{	// Light is too dark, so distance is to HIGH. Reduce and guess again.
			distanceGuessHigh = curDistanceGuess;		// Lower the high limit for the guesses
		}
		else if ( curDiffuseAtGuessDistance > targetLightLevelHigh )
		{	// Light is too bright, so distance is to LOW. Increase and guess again
			distanceGuessLow = curDistanceGuess;
		}
		else 
		{	// Light level is within range, so return this distance
			return curDistanceGuess;
		}

		iterationCount++;

	}// while ( iterationCount < maxIterations )

	// If we are here, then we ran out of iterations.
	// Pick a distance between the low and high
	float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

	return distance;
}

//static 
const float cLight::DEFAULTZEROTHRESHOLD = 0.0001f;

float cLight::calcDiffuseFromAttenByDistance(float distance, float zeroThreshold )
{
	float diffuse = 1.0f;		// Assume full brightness

	float denominator = this->attenuation.x +						// Constant
	                    this->attenuation.y * distance +			// Linear
						this->attenuation.z * distance * distance;	// Quadratic
	
																	// Is this zero 
	if ( denominator <= zeroThreshold )
	{	
		diffuse = 1.0f;
	}
	else
	{
		float atten = 1.0f / denominator;
		diffuse *= atten;
		if ( diffuse > 1.0f )
		{
			diffuse = 1.0f;
		}
	}//if ( denominator <= zeroThreshold )
	
	return diffuse;
}
