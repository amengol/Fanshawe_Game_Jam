#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <string>
#include "cMesh.h"

#include "cModelAssetLoader.h"
#include "cVAOMeshManager.h"

void LoadModelsIntoScene(void);
bool Load3DModelsIntoMeshManager(int shaderID,
    cVAOMeshManager* pVAOManager,
    cModelAssetLoader* pModelAssetLoader,
    std::string &error);

#endif
