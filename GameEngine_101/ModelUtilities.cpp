#include "ModelUtilities.h"
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>

//extern cMesh g_MeshFractalTerrain;

void ReadFileToToken( std::ifstream &file, std::string token )
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;
		if ( garbage == token )
		{
			return;
		}
	}while( bKeepReading );
	return;
}

bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	std::ifstream plyFile( filename.c_str() );

	if ( ! plyFile.is_open() )
	{	
		return false;
	}

	ReadFileToToken( plyFile, "vertex" );
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken( plyFile, "face" );
	plyFile >> theMesh.numberOfTriangles;		

	ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
        float x, y, z; // In case we need-> , confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;

		theMesh.pVertices[index].x = x;
		theMesh.pVertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;
		theMesh.pVertices[index].g = 1.0f;
		theMesh.pVertices[index].b = 1.0f;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++)
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;
	}

	theMesh.CalculateNormals();

	return true;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMeshWithNormals( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	std::ifstream plyFile( filename.c_str() );

	if ( ! plyFile.is_open() )
	{
		return false;
	}
	// File is open, let's read it

	ReadFileToToken( plyFile, "vertex" );
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken( plyFile, "face" );
	plyFile >> theMesh.numberOfTriangles;		

	ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{		
        float x, y, z, nx, ny, nz; // In case we need-> , confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		plyFile >> nx >> ny >> nz;

		theMesh.pVertices[index].x = x;
		theMesh.pVertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;	
		theMesh.pVertices[index].g = 1.0f;	
		theMesh.pVertices[index].b = 1.0f;	
		theMesh.pVertices[index].nx = nx;	
		theMesh.pVertices[index].ny = ny;	
		theMesh.pVertices[index].nz = nz;	
	}

	// Load the triangle (or face) information, too
	for ( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		int discard = 0;
		plyFile >> discard;									
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	
	}

	return true;
}


bool Load3DModelsIntoMeshManager(int shaderID, cVAOMeshManager* pVAOManager, std::string &error)
{
	std::stringstream ssError;
	bool bAnyErrors = false;
    	
	//// This is our Fractal terrain model
	//{
	//	::g_MeshFractalTerrain.name = "FractalTerrain";
	//	if ( ! LoadPlyFileIntoMeshWithNormals( "FractalTerrain2_xyz_n.ply", g_MeshFractalTerrain ) )
	//	{ 
	//		ssError << "Didn't load model >" << g_MeshFractalTerrain.name << "<" << std::endl;
	//		bAnyErrors = true;
	//	}
	//	if ( ! pVAOManager->loadMeshIntoVAO( g_MeshFractalTerrain, shaderID ) )
	//	{
	//		ssError << "Could not load mesh >" << g_MeshFractalTerrain.name << "< into VAO" << std::endl;
	//		bAnyErrors = true;
	//	}
	//		
	//	// For physics, generate the triangles we need for physics
	//	::g_MeshFractalTerrain.GeneratePhysicsTriangles();

	//}	// ENDOF: load models

	{
		cMesh theMesh;
        theMesh.name = "Galactica";
		if ( ! LoadPlyFileIntoMeshWithNormals( "Battlestar_Galactica_ASCII_Res_2_468281_faces_Scale50x.ply", theMesh) )
        //if ( ! LoadPlyFileIntoMeshWithNormals( "Battlestar_Galactica_ASCII_Res_4_116617_faces_Scale50x.ply", theMesh) )
		{ 
			ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
			bAnyErrors = true;
		}
		if ( ! pVAOManager->loadMeshIntoVAO(theMesh, shaderID ) )
		{
			ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
			bAnyErrors = true;
		}
	}

    {
        cMesh theMesh;
        theMesh.name = "Viper";
        if (!LoadPlyFileIntoMeshWithNormals("Viper_MkVII_ASCII_UVTex.ply", theMesh))
        {
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAnyErrors = true;
        }
        if (!pVAOManager->loadMeshIntoVAO(theMesh, shaderID))
        {
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAnyErrors = true;
        }
    }

    {
        cMesh theMesh;
        theMesh.name = "Raider";
        if (!LoadPlyFileIntoMeshWithNormals("Raider_ASCII_UVtex_Scale8x.ply", theMesh))
        {
            ssError << "Didn't load model >" << theMesh.name << "<" << std::endl;
            bAnyErrors = true;
        }
        if (!pVAOManager->loadMeshIntoVAO(theMesh, shaderID))
        {
            ssError << "Could not load mesh >" << theMesh.name << "< into VAO" << std::endl;
            bAnyErrors = true;
        }
    }
		

	if ( ! bAnyErrors ) 
	{
		// Copy the error string stream into the error string that
		//	gets "returned" (through pass by reference)
		error = ssError.str();
	}

	return bAnyErrors;
}