#pragma once
#include <map>
#include <vector>
#include <string>
#include "assimp/scene.h"
#include "cSkinnedMesh.h"

class cAnimationCollection
{
public:
    // Maps a vector of animations to a name
    bool addAnimationCollection(std::string name, const aiScene* anim);

    // Maps a vector of skinned meshes to a name
    bool addSkinnedMesh(std::string name, cSkinnedMesh* skinnedMesh);

    // Returns all animations associated with the name
    const aiScene* getAnimation(std::string name);

    // Returns all skinned meshes associated with the name
    cSkinnedMesh* getSkinnedMeshes(std::string name);

private:
    // Animation collection
    std::map <std::string, const aiScene*> mMapNameToAnimations;

    // Skinned Meshes collection
    std::map <std::string, cSkinnedMesh*> mMapNameToVecSkinnedMesh;
};

