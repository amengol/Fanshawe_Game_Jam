#include "cAnimationCollection.h"

bool cAnimationCollection::addAnimationCollection(std::string name, const aiScene* anim)
{
    if (anim == NULL)
        return false;
    this->mMapNameToAnimations[name] = anim;
    return true;
}

bool cAnimationCollection::addSkinnedMesh(std::string name, cSkinnedMesh* skinnedMesh)
{
    if (skinnedMesh == NULL)
        return false;
    this->mMapNameToVecSkinnedMesh[name] = skinnedMesh;
    return true;
}

const aiScene* cAnimationCollection::getAnimation(std::string name)
{
    return this->mMapNameToAnimations[name];
}

cSkinnedMesh* cAnimationCollection::getSkinnedMeshes(std::string name)
{
    return this->mMapNameToVecSkinnedMesh[name];
}
