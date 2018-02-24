#include "cModelAssetLoader.h"

#include <sstream>

cModelAssetLoader::cModelAssetLoader()
{
	return;
}

void cModelAssetLoader::setBasePath( std::string fullPathWITHslash )
{
	this->m_basePath = fullPathWITHslash;
	return;
}

std::string cModelAssetLoader::getBasePath(void)
{
	return this->m_basePath;
}


void cModelAssetLoader::m_ReadFileToToken( std::ifstream &file, std::string token )
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if ( garbage == token )
		{
			return;
		}
	}while( bKeepReading );
	return;
}



// Takes a file name, loads a mesh
bool cModelAssetLoader::LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"

	std::string fileWithPath = this->m_basePath + filename;

	std::ifstream plyFile( fileWithPath.c_str() );

	if ( ! plyFile.is_open() )
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	this->m_ReadFileToToken( plyFile, "vertex" );
//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	this->m_ReadFileToToken( plyFile, "face" );
//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;		

	this->m_ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z;//, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
//		plyFile >> confidence;
//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for ( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	theMesh.CalculateNormals();

	return true;
}

// Takes a file name, loads a mesh
bool cModelAssetLoader::LoadPlyFileIntoMeshWithNormals( std::string filename, cMesh &theMesh )
{
	// Load the vertices


	// c_str() changes a string to a "c style char* string"
	std::string fileWithPath = this->m_basePath + filename;
	std::ifstream plyFile( fileWithPath.c_str() );

	if ( ! plyFile.is_open() )
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	this->m_ReadFileToToken( plyFile, "vertex" );
//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	this->m_ReadFileToToken( plyFile, "face" );
//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;		

	this->m_ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, nx, ny, nz;//, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		plyFile >> nx >> ny >> nz;
//		plyFile >> confidence;
//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for ( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

//	theMesh.CalculateNormals();

	return true;
}

bool cModelAssetLoader::LoadPlyFileIntoMeshWithNormals_and_colours(std::string filename, cMesh & theMesh)
{
    // Load the vertices


    // c_str() changes a string to a "c style char* string"
    std::string fileWithPath = this->m_basePath + filename;
    std::ifstream plyFile(fileWithPath.c_str());

    if (!plyFile.is_open())
    {	// Didn't open file, so return
        return false;
    }
    // File is open, let's read it

    this->m_ReadFileToToken(plyFile, "vertex");
    //	int numVertices = 0;
    plyFile >> theMesh.numberOfVertices;

    this->m_ReadFileToToken(plyFile, "face");
    //	int numTriangles = 0;
    plyFile >> theMesh.numberOfTriangles;

    this->m_ReadFileToToken(plyFile, "end_header");

    // Allocate the appropriate sized array (+a little bit)
    //theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
    theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
    theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

    // Read vertices
    for (int index = 0; index < theMesh.numberOfVertices; index++)
    {
        //end_header
        //-0.0312216 0.126304 0.00514924 0.850855 0.5 		
        float x, y, z, nx, ny, nz;//, confidence, intensity;
        float r, g, b, a;

        plyFile >> x;
        plyFile >> y;
        plyFile >> z;
        plyFile >> nx >> ny >> nz;
        plyFile >> r >> g >> b >> a;
        //		plyFile >> confidence;
        //		plyFile >> intensity;

        theMesh.pVertices[index].x = x;	// vertices[index].x = x;
        theMesh.pVertices[index].y = y;	// vertices[index].y = y;
        theMesh.pVertices[index].z = z;
        theMesh.pVertices[index].r = r / 255.0f;	// vertices[index].g = 1.0f;
        theMesh.pVertices[index].g = g / 255.0f;	// vertices[index].b = 1.0f;
        theMesh.pVertices[index].b = b / 255.0f;	// vertices[index].r = 1.0f;
        theMesh.pVertices[index].a = 1.0f;
        theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
        theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
        theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;
    }

    // Load the triangle (or face) information, too
    for (int count = 0; count < theMesh.numberOfTriangles; count++)
    {
        // 3 164 94 98 
        int discard = 0;
        plyFile >> discard;									// 3
        plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
        plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
        plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
    }

    //	theMesh.CalculateNormals();

    return true;
}

// Takes a file name, loads a mesh
bool cModelAssetLoader::LoadPlyFileIntoMeshWith_Normals_and_UV( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::string fileWithPath = this->m_basePath + filename;
	std::ifstream plyFile( fileWithPath.c_str() );

	if ( ! plyFile.is_open() )
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	this->m_ReadFileToToken( plyFile, "vertex" );
//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	this->m_ReadFileToToken( plyFile, "face" );
//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;		

	this->m_ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, nx, ny, nz;
		// Added
		float u, v;		// Model now has texture coordinate

		// Typical vertex is now... 
		// 29.3068 -5e-006 24.3079 -0.949597 0.1875 -0.251216 0.684492 0.5

		plyFile >> x >> y >> z;
		plyFile >> nx >> ny >> nz;
		// 
		plyFile >> u >> v;			// ADDED
			

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;

		// Only store the 1st UV.
		theMesh.pVertices[index].u1 = u;
		theMesh.pVertices[index].v1 = v;
	}

	// Load the triangle (or face) information, too
	for ( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

//	theMesh.CalculateNormals();

	return true;
}
