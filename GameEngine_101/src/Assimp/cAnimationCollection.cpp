#include "cAnimationCollection.h"

void cAnimationCollection::addAnimationCollection(std::string name, aiScene* vecAnim)
{
    this->mMapNameToAnimations[name] = vecAnim;
}

void cAnimationCollection::addSkinnedMesh(std::string name, cSimpleAssimpSkinnedMesh* vecSkinnedMesh)
{
    this->mMapNameToVecSkinnedMesh[name] = vecSkinnedMesh;
}

aiScene* cAnimationCollection::getAnimation(std::string name)
{
    return this->mMapNameToAnimations[name];
}

cSimpleAssimpSkinnedMesh* cAnimationCollection::getSkinnedMeshes(std::string name)
{
    return this->mMapNameToVecSkinnedMesh[name];
}
