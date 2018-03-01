#include "cAnimationCollection.h"



cAnimationCollection::cAnimationCollection()
{
}


cAnimationCollection::~cAnimationCollection()
{
}

void cAnimationCollection::addAnimationCollection(std::string name, std::vector<aiAnimation*> collection)
{
    mMapNameToVecAnimations[name] = collection;
}
