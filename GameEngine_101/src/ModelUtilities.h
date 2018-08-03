#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <string>
#include "cVAOMeshManager.h"

bool Load3DModelsIntoMeshManager(int shaderId,
                                 const std::string& filePath,
                                 cVAOMeshManager* pVaoManager,
                                 std::string &error);

#endif
