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

class cVAOMeshManager;
class cModelAssetLoader;
class cLightManager;
class cCamera;

class cSceneLoader
{
public:
    cSceneLoader();
    ~cSceneLoader();

    // Loads all cGameObjects into scene. Internaly it loads also the mesh
    // models into cVAOMeshManager using the cModelAssetLoader. Reports back an
    // error via a string
    bool LoadModelsIntoScene(int shaderId,
                             const std::string& filePath,
                             cVAOMeshManager* vao, 
                             std::string& error) const;

    // Loads all lights parameters from a json file. Reports back an
    // error via a string
    bool loadLightParams(int shaderID, cLightManager*, std::string& error);

    // Loads all camera parameters from a joson files. Reports back an
    // error via a string
    bool loadCameraParams(cCamera&, std::string& error);

    //// Loads the AI configuration from the maze.config file
    //bool loadAiGrid(std::string& error);
    
    // Loads limit planes to be used in the Physics Engine
    bool loadLimitPlanes(std::string& error);
};
#endif // !_cSceneLoader_HG_
