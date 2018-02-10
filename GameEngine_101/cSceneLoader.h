/**
    Class cSceneLoader
    Purpose: to help loading all settings and config files for a scene

    @author Jorge Amengol
    @version 0.1.4 
    @date Feb 9th, 2018
*/

#ifndef _cSceneLoader_HG_
#define _cSceneLoader_HG_

#include <string>
//#include "Physics.h"
#include <vector>
#include <glm\vec3.hpp>

class cVAOMeshManager;
class cModelAssetLoader;
class cLightManager;
class cCameraObject;

// Put this and LimitPlane here for now (was in the Physics.cpp)
enum LimitPlaneType
{
    FLOOR,
    FRONT,
    BACK,
    LEFT,
    RIGHT
};

struct LimitPlane
{
    glm::vec3 position;
    LimitPlaneType type;
};

class cSceneLoader
{
public:
    cSceneLoader();
    ~cSceneLoader();

    // Loads all cGameObjects into scene. Internaly it loads also the mesh
    // models into cVAOMeshManager using the cModelAssetLoader. Reports back an
    // error via a string
    bool loadModelsIntoScene(int shaderID, cVAOMeshManager*, 
                             cModelAssetLoader*, std::string& error);

    // Loads all lights parameters from a json file. Reports back an
    // error via a string
    bool loadLightParams(int shaderID, cLightManager*, std::string& error);

    // Loads all camera parameters from a joson files. Reports back an
    // error via a string
    bool loadCameraParams(cCameraObject*, std::string& error);
    
    // Loads limit planes to be used in the Physics Engine
    bool loadLimitPlanes(std::vector<LimitPlane>& vecLP, std::string& error);

private:

};
#endif // !_cSceneLoader_HG_
