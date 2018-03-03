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
    void addAnimationCollection(std::string name, std::vector<aiAnimation*> vecAnim);

    // Maps a vector of skinned meshes to a name
    void addSkinnedMesh(std::string name, std::vector<cSimpleAssimpSkinnedMesh*> vecSkinnedMesh);

    // Returns all animations associated with the name
    std::vector<aiAnimation*> getAnimations(std::string name);

    // Returns all skinned meshes associated with the name
    std::vector<cSimpleAssimpSkinnedMesh*> getSkinnedMeshes(std::string name);

private:
    // Animation collection
    std::map <std::string, std::vector<aiAnimation*>> mMapNameToVecAnimations;

    // Skinned Meshes collection
    std::map <std::string, std::vector<cSimpleAssimpSkinnedMesh*>> mMapNameToVecSkinnedMesh;
};

