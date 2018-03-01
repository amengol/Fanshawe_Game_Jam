#ifndef _cAssimpAssetLoader_HG_
#define _cAssimpAssetLoader_HG_

#include <string>
#include <array>
#include <vector>
#include <map>
#include <glm\mat4x4.hpp>

// assimp include files.
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

class cVAOMeshManager;

class cAssimpAssetLoader
{
private:

    static const int MAX_BONES_PER_VERTEX = 4;

    // Struct to hold bones info
    struct sVertexBoneData
    {
        std::array<float, MAX_BONES_PER_VERTEX> ids;
        std::array<float, MAX_BONES_PER_VERTEX> weights;

        void AddBoneData(unsigned int BoneID, float Weight);
    };

    struct sBoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;
        glm::mat4 ObjectBoneTransformation;
    };

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

public:
    cAssimpAssetLoader();
    ~cAssimpAssetLoader();

    // Creates the logger
    void createAILogger();

    // Adds a message to the log file
    void logInfo(std::string logString);

    // Imports  the 3D scene from a file
    bool Import3DFromFile(const std::string& pFile);

    // Loads all meshes into the VAO manager. If there is more than one mesh in
    // the Assimp Scene, all of them will have the friendlyName and persistence
    bool loadMeshesIntoVAO(cVAOMeshManager*, 
                           int shaderID,
                           std::string meshName, 
                           bool isPersistent);

    bool Initialize(void);
    void LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &vertexBoneData);

    std::string fileName;
    std::string friendlyName;
    std::vector<sVertexBoneData> vecVertexBoneData;	//Bones;
    glm::mat4 mGlobalInverseTransformation;

    unsigned int m_numberOfVertices;
    //unsigned int m_indexBuf_ID;			// Index buffer referring to VBO
    unsigned int m_numberOfIndices;
    unsigned int m_numberOfTriangles;
    std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> m_mapBoneNameToBoneIndex;	//mMapping;
    std::vector<sBoneInfo> mBoneInfo;
    unsigned int mNumBones;	//mNums;

};
#endif // !_assimpAssetLoader_HG_

