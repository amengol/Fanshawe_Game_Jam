#ifndef _Utilities_HG_
#define _Utilities_HG_

#include <vector>;
#include "cGameObject.h"
#include <glm/glm.hpp>

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
/**
 * \brief Returns a random number in the given range
 */
template <class T>
T GetRandInRange( T min, T max )
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

/**
 * \brief Loads the initial configuration
 * \param [in] fileName The filename of the configuration to load
 * \param [in] titleEnd The tag indicating the end of the title section
 * \param [out] width Receives the width of the screen
 * \param [out] height Receives the height of the screen
 * \param [out] title Receives the title of the screen
 * \return True if the function succeeds, otherwise false
 */
bool InitialConfig(const std::string& fileName,
                   const std::string& titleEnd,
                   int& width,
                   int& height,
                   std::string& title);

/**
 * \brief 
 * \param numOfObjects The number of GameObjects to be created
 * \param [in] theVecGo A vector to receive the GameObjects
 * \param info GameObject info
 * \param minX Minimum X value for the area to create the GameObjects
 * \param maxX Maximum X value for the area to create the GameObjects
 * \param minY Minimum Y value for the area to create the GameObjects
 * \param maxY Maximum Y value for the area to create the GameObjects
 * \param minZ Minimum Z value for the area to create the GameObjects
 * \param maxZ Maximum Z value for the area to create the GameObjects
 */
void CreateRamdomGameObjects(int numOfObjects,
                             std::vector<cGameObject*>& theVecGo,
                             std::vector<GameObjectsInfo> info,
                             float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ);

/**
 * \brief Turns the GameObject to the camera
 */
void TurnGameObjectToCamera(cGameObject* theGo, glm::vec3 cameraPosition, bool resetPitch = true);

/**
 * \brief Gets an orientation matrix from a vector
 */
glm::mat4 GetMatrixFromVector(glm::vec3 xzDirection);

/**
 * \brief Loads a text file into a straing
 * \return True if the function succeeds, otherwise false
 */
bool LoadFileIntoString(std::string& theString, std::string fileName);

#endif
