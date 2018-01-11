/**
    Class cSceneLoader
    Purpose: to help loading all settings and config files for a scene

    @author Jorge Amengol
    @version 0.1.1 
    @date Jan 11th, 2018
*/

#ifndef _cSceneLoader_HG_
#define _cSceneLoader_HG_

#include <string>

class cVAOMeshManager;
class cModelAssetLoader;

class cSceneLoader
{
public:
    cSceneLoader();
    ~cSceneLoader();

    bool loadModelsIntoScene(int shaderID, cVAOMeshManager*, 
                             cModelAssetLoader*, std::string& error);

private:

};
#endif // !_cSceneLoader_HG_
