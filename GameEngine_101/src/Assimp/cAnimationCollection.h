#pragma once
#include <map>
#include <vector>
#include "assimp/scene.h"

class cAnimationCollection
{
public:
    cAnimationCollection();
    ~cAnimationCollection();

    void addAnimationCollection(std::string name, std::vector<aiAnimation*> collection);

private:
    // Animation collection
    std::map <std::string, std::vector<aiAnimation*>> mMapNameToVecAnimations;
};

