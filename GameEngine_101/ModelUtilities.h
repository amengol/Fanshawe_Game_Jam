#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <fstream>
#include <string>
#include "cMesh.h"

class cVAOMeshManager;

// Reads a file up to a token
void ReadFileToToken( std::ifstream &file, std::string token );

// Loads a PLY file into a cMesh class
bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh );
bool LoadPlyFileIntoMeshWithNormals(std::string filename, cMesh &theMesh);
bool Load3DModelsIntoMeshManager(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);

#endif
