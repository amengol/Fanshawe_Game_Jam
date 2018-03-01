#include "cSkinnedMesh.h"
#include "animHelper.h"

cSkinnedMesh::cSkinnedMesh(std::string file, 
                           std::string friendlyName, 
                           aiMesh* mesh,
                           glm::mat4 globalInverseTrans)
{
    if (mesh->mNumBones == 0)
    {
        this->~cSkinnedMesh();
    }
    else
    {
        this->mFileName = file;
        this->mFriendlyName = friendlyName;
        this->mGlobalInverseTransformation = globalInverseTrans;
        this->mNumberOfVertices = mesh->mNumVertices;
        // This is the vertex information for JUST the bone stuff
        this->mVecVertexBoneData.resize(this->mNumberOfVertices);
        this->loadBones(mesh, this->mVecVertexBoneData);
    }
}


cSkinnedMesh::~cSkinnedMesh()
{
}

void cSkinnedMesh::loadBones(const aiMesh* mesh, std::vector<sVertexBoneData>& vertexBoneData)
{
    for (unsigned int boneIndex = 0; boneIndex != mesh->mNumBones; boneIndex++)
    {
        unsigned int BoneIndex = 0;
        std::string BoneName(mesh->mBones[boneIndex]->mName.data);

        std::map<std::string, unsigned int>::iterator it = this->mMapBoneNameToBoneIndex.find(BoneName);
        if (it == this->mMapBoneNameToBoneIndex.end())
        {
            BoneIndex = this->mNumBones;
            this->mNumBones++;
            sBoneInfo bi;
            this->mBoneInfo.push_back(bi);

            this->mBoneInfo[BoneIndex].BoneOffset = AIMatrixToGLMMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);
            this->mMapBoneNameToBoneIndex[BoneName] = BoneIndex;
        }
        else
        {
            BoneIndex = it->second;
        }

        for (unsigned int WeightIndex = 0; WeightIndex != mesh->mBones[boneIndex]->mNumWeights; WeightIndex++)
        {
            unsigned int VertexID = mesh->mBones[boneIndex]->mWeights[WeightIndex].mVertexId;
            float Weight = mesh->mBones[boneIndex]->mWeights[WeightIndex].mWeight;
            vertexBoneData[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
    return;
}

void cSkinnedMesh::sVertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
    for (unsigned int Index = 0; Index < sizeof(this->ids) / sizeof(this->ids[0]); Index++)
    {
        if (this->weights[Index] == 0.0f)
        {
            this->ids[Index] = (float)BoneID;
            this->weights[Index] = Weight;
            return;
        }
    }
}
