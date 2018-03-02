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
    void addAnimationCollection(std::string name, std::vector<aiAnimation*> vecAnim);

    // Maps a vector of skinned meshes to a name
    void addSkinnedMesh(std::string name, std::vector<cSkinnedMesh*> vecSkinnedMesh);

    // Returns all animations associated with the name
    std::vector<aiAnimation*> getAnimations(std::string name);

    // Returns all skinned meshes associated with the name
    std::vector<cSkinnedMesh*> getSkinnedMeshes(std::string name);

private:
    // Animation collection
    std::map <std::string, std::vector<aiAnimation*>> mMapNameToVecAnimations;

    // Skinned Meshes collection
    std::map <std::string, std::vector<cSkinnedMesh*>> mMapNameToVecSkinnedMesh;
};

