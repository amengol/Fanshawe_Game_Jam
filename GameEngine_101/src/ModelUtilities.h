#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <string>
#include "cVAOMeshManager.h"

bool Load3DModelsIntoMeshManager(int shaderId,
                                 cVAOMeshManager* pVaoManager,
                                 std::string &error);

#endif
