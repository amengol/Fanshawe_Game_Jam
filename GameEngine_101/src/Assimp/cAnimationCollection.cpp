#include "cAnimationCollection.h"

void cAnimationCollection::addAnimationCollection(std::string name, const aiScene* anim)
{
    this->mMapNameToAnimations[name] = anim;
}

void cAnimationCollection::addSkinnedMesh(std::string name, cSkinnedMesh* skinnedMesh)
{
    this->mMapNameToVecSkinnedMesh[name] = skinnedMesh;
}

const aiScene* cAnimationCollection::getAnimation(std::string name)
{
    return this->mMapNameToAnimations[name];
}

cSkinnedMesh* cAnimationCollection::getSkinnedMeshes(std::string name)
{
    return this->mMapNameToVecSkinnedMesh[name];
}
