#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>

cModelAssetLoader* g_pModelAssetLoader = NULL;

bool Load3DModelsIntoMeshManager(int shaderID,
    cVAOMeshManager* pVAOManager,
    cModelAssetLoader* pModelAssetLoader,
    std::string &error)
{
    std::stringstream ssError;
    bool bAllGood = true;

    {
        cMesh fractalTerrain;
        fractalTerrain.name = "FractalTerrain";
        if (!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals("FractalTerrain_xyz_n_4xBigger.ply", fractalTerrain))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << fractalTerrain.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if (!pVAOManager->loadMeshIntoVAO(fractalTerrain, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << fractalTerrain.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }
    

    if (!bAllGood)
    {
        // Copy the error string stream into the error string that
        //	gets "returned" (through pass by reference)
        error = ssError.str();
    }

    return bAllGood;
}