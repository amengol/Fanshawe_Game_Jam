/**
    Class cSceneLoader
    Purpose: to help loading all settings and config files for a scene

    @author Jorge Amengol
    @version 0.1.2 
    @date Jan 12th, 2018
*/

#ifndef _cSceneLoader_HG_
#define _cSceneLoader_HG_

#include <string>

class cVAOMeshManager;
class cModelAssetLoader;
class cLightManager;

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

private:

};
#endif // !_cSceneLoader_HG_
