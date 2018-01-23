#ifndef _cAssimAssetLoader_HG_
#define _cAssimAssetLoader_HG_
#include <string>

// assimp include files.
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

class cAssimAssetLoader
{
public:
    cAssimAssetLoader();
    ~cAssimAssetLoader();

    // Adds a message to the log file
    void logInfo(std::string logString);

    bool Import3DFromFile(const std::string& pFile);

    const aiScene* getScene();

private:
    void createAILogger();
    
    // the global Assimp scene object
    const aiScene* scene;
    // Create an instance of the Importer class
    Assimp::Importer importer;
};
#endif // !_assimAssetLoader_HG_

