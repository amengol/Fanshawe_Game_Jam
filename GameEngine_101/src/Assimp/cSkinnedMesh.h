#pragma once
#include "assimp/scene.h"
#include <glm\mat4x4.hpp>
#include <array>
#include <vector>
#include <map>

class cSkinnedMesh
{
public:
    cSkinnedMesh(std::string file, 
                 std::string friendlyName, 
                 aiMesh* mesh,
                 glm::mat4 globalInverseTrans);
    ~cSkinnedMesh();

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

    void loadBones(const aiMesh* mesh, std::vector<sVertexBoneData> &vertexBoneData);

    std::string mFileName;
    std::string mFriendlyName;
    glm::mat4 mGlobalInverseTransformation;
    unsigned int mNumberOfVertices;
    std::vector<sVertexBoneData> mVecVertexBoneData;
    std::map<std::string, unsigned int> mMapBoneNameToBoneIndex;
    unsigned int mNumBones;
    std::vector<sBoneInfo> mBoneInfo;
};

