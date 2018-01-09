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

    //{
    //    cMesh fractalTerrain;
    //    fractalTerrain.name = "FractalTerrain";
    //    if (!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals("FractalTerrain_xyz_n_4xBigger.ply", fractalTerrain))
    //    {
    //        //std::cout << "Didn't load model" << std::endl;
    //        ssError << "Didn't load model >" << fractalTerrain.name << "<" << std::endl;
    //        bAllGood = false;
    //    }
    //    // ***********************************************************************
    //    // NOTE the TRUE so that it keeps the mesh!!!
    //    if (!pVAOManager->loadMeshIntoVAO(fractalTerrain, shaderID, true))
    //    {
    //        //std::cout << "Could not load mesh into VAO" << std::endl;
    //        ssError << "Could not load mesh >" << fractalTerrain.name << "< into VAO" << std::endl;
    //        bAllGood = false;
    //    }
    //    // ***********************************************************************
    //}
 
    //{
    //    cMesh Delorean;
    //    Delorean.name = "Delorean";
    //    if (!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals_and_colours("Delorean.ply", Delorean))
    //    {
    //        //std::cout << "Didn't load model" << std::endl;
    //        ssError << "Didn't load model >" << Delorean.name << "<" << std::endl;
    //        bAllGood = false;
    //    }
    //    // ***********************************************************************
    //    // NOTE the TRUE so that it keeps the mesh!!!
    //    if (!pVAOManager->loadMeshIntoVAO(Delorean, shaderID))
    //    {
    //        //std::cout << "Could not load mesh into VAO" << std::endl;
    //        ssError << "Could not load mesh >" << Delorean.name << "< into VAO" << std::endl;
    //        bAllGood = false;
    //    }
    //    // ***********************************************************************
    //}

    {
        cMesh testMesh;
        testMesh.name = "SkyBox";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals("SkyBox.ply", testMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(testMesh, shaderID, false))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "FacadeSets";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("FacadeSets.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "RoofsEtc";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("RoofsEtc.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Asphalt";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("Asphalt.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Concrete";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("Concrete.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Curbs";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("Curbs.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, false))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Ground1";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("Ground1.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Ground2";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("Ground2.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "StreetPart";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("StreetPart.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, true))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "SideWalkTree";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("SideWalkTree.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID, false))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "md500";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV("md500.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        // NOTE the TRUE so that it keeps the mesh!!!
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
    }

    {
        cMesh theMesh;
        theMesh.name = "Sphere";
        if(!pModelAssetLoader->LoadPlyFileIntoMeshWithNormals("Sphere.ply", theMesh))
        {
            //std::cout << "Didn't load model" << std::endl;
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAllGood = false;
        }
        // ***********************************************************************
        if(!pVAOManager->loadMeshIntoVAO(theMesh, shaderID))
        {
            //std::cout << "Could not load mesh into VAO" << std::endl;
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
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