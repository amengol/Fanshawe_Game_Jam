#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>
#include <vector>
#include "cGameObject.h"

cModelAssetLoader* g_pModelAssetLoader = NULL;

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects)
{
    // Linear search, baby!
    unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (vec_pGameObjects[index]->friendlyName == friendlyName)
        {
            return vec_pGameObjects[index];
        }
    }
    // Didn't find it
    return NULL;
}

cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects)
{
    // Linear search, baby!
    unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
    for (unsigned int index = 0; index != numObjects; index++)
    {
        if (ID = vec_pGameObjects[index]->getUniqueID())
        {
            return vec_pGameObjects[index];
        }
    }
    // Didn't find it
    return NULL;
}

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
 
    {
        cMesh Delorean;
        Delorean.name = "Delorean";
        if (!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals_and_colours("Delorean.ply", Delorean))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << Delorean.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if (!pVAOManager->loadMeshIntoVAO(Delorean, shaderID))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << Delorean.name << "< into VAO" << std::endl;
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