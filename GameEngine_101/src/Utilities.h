#ifndef _Utilities_HG_
#define _Utilities_HG_

#include <vector>;
#include "cGameObject.h"

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation

template <class T>
T getRandInRange( T min, T max )
{
	double value = min + static_cast <double> ( rand() ) 
		           / ( static_cast <double> ( RAND_MAX/(static_cast<double>(max-min) ) ) );

	return static_cast<T>(value);

}

struct GameObjectsInfo
{
    std::string meshName;
    std::string texture;
    std::string alpha;
};

bool initialConfig(std::string fileName, unsigned int& width, unsigned int& height, std::string& title);

void createRamdomGameObjects(int numOfObjects,
                             std::vector<cGameObject*>&,
                             std::vector<GameObjectsInfo> info,
                             float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ);

void turnGameObjectToCamera(cGameObject*, glm::vec3 cameraPosition);

bool loadFileIntoString(std::string& theString, std::string fileName);

#endif
