#ifndef _cAssimpAssetLoader_HG_
#define _cAssimpAssetLoader_HG_

#include <string>

// assimp include files.
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

class cVAOMeshManager;

class cAssimpAssetLoader
{
public:
    cAssimpAssetLoader();
    ~cAssimpAssetLoader();

    // Creates the logger
    void createAILogger();

    // Adds a message to the log file
    void logInfo(std::string logString);

    // Imports  the 3D scene from a file
    bool Import3DFromFile(const std::string& pPath, const std::string& pFile);

    // Imports an aiScene from a file
    const aiScene* GetAiSceneFromFile(const std::string& pPath, const std::string& pFile);

    // Loads all meshes into the VAO manager. If there is more than one mesh in
    // the Assimp Scene, all of them will have the friendlyName and persistence
    bool loadMeshesIntoVAO(cVAOMeshManager*, 
                           int shaderID,
                           std::string meshName, 
                           bool isPersistent);

   


    //unsigned int m_indexBuf_ID;			// Index buffer referring to VBO
    unsigned int m_numberOfIndices;
    unsigned int m_numberOfTriangles;

private:

    // Recurse mesh loader
    bool recursiveVAOMeshLoader(cVAOMeshManager*,
                                int shaderID,
                                std::string meshName,
                                const struct aiScene*,
                                const struct aiNode*,
                                bool isPersistent);

    // the global Assimp scene object
    const aiScene* scene;

    // Create an instance of the Importer class
    Assimp::Importer importer;
};
#endif // !_assimpAssetLoader_HG_

