#pragma once
#include <map>
#include <vector>
#include <string>
#include "assimp/scene.h"
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

class cAnimationCollection
{
public:
    // Maps a vector of animations to a name
    void addAnimationCollection(std::string name, const aiScene* anim);

    // Maps a vector of skinned meshes to a name
    void addSkinnedMesh(std::string name, cSimpleAssimpSkinnedMesh* skinnedMesh);

    // Returns all animations associated with the name
    const aiScene* getAnimation(std::string name);

    // Returns all skinned meshes associated with the name
    cSimpleAssimpSkinnedMesh* getSkinnedMeshes(std::string name);

private:
    // Animation collection
    std::map <std::string, const aiScene*> mMapNameToAnimations;

    // Skinned Meshes collection
    std::map <std::string, cSimpleAssimpSkinnedMesh*> mMapNameToVecSkinnedMesh;
};

