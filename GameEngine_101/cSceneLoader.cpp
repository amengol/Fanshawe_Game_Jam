#include "cSceneLoader.h"
#include "ModelUtilities.h"

cSceneLoader::cSceneLoader()
{
}

cSceneLoader::~cSceneLoader()
{
}

bool cSceneLoader::loadModelsIntoScene(int shaderID, 
                                       cVAOMeshManager* VAO,
                                       cModelAssetLoader* assetLoader, 
                                       std::string& error)
{
    if(!Load3DModelsIntoMeshManager(shaderID, VAO, assetLoader, error))
    {
        return false;
    }
    LoadModelsIntoScene();
    return true;
}
