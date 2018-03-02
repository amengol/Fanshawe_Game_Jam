#include "cAnimationCollection.h"

void cAnimationCollection::addAnimationCollection(std::string name, std::vector<aiAnimation*> vecAnim)
{
    this->mMapNameToVecAnimations[name] = vecAnim;
}

void cAnimationCollection::addSkinnedMesh(std::string name, std::vector<cSkinnedMesh*> vecSkinnedMesh)
{
    this->mMapNameToVecSkinnedMesh[name] = vecSkinnedMesh;
}

std::vector<aiAnimation*> cAnimationCollection::getAnimations(std::string name)
{
    return this->mMapNameToVecAnimations[name];
}

std::vector<cSkinnedMesh*> cAnimationCollection::getSkinnedMeshes(std::string name)
{
    return this->mMapNameToVecSkinnedMesh[name];
}
