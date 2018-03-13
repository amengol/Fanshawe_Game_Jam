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
    std::map <std::string, const aiScene*>::iterator it = mMapNameToAnimations.find(name);
    if (it == mMapNameToAnimations.end())
        return NULL;
    return it->second;
}

cSkinnedMesh* cAnimationCollection::getSkinnedMeshes(std::string name)
{
    std::map <std::string, cSkinnedMesh*>::iterator it = mMapNameToVecSkinnedMesh.find(name);
    if (it == mMapNameToVecSkinnedMesh.end())
        return NULL;
    return it->second;
}
