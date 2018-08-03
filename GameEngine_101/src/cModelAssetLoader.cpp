#include "cModelAssetLoader.h"
#include "cMesh.h"
#include <fstream>
#include <sstream>

cModelAssetLoader::cModelAssetLoader()
{
    return;
}

void cModelAssetLoader::SetBasePath(const std::string fullPath)
{
    this->basePath_ = fullPath;
}

std::string cModelAssetLoader::GetBasePath() const
{
    return this->basePath_;
}


void cModelAssetLoader::ReadFileToToken(std::ifstream &file, const std::string token)
{
    const bool keepReading = true;
    std::string garbage;
    do
    {
        file >> garbage;
        if ( garbage == token )
        {
            return;
        }
    }while( keepReading );
}

bool cModelAssetLoader::LoadPlyFileIntoMesh(const std::string filename, cMesh& theMesh ) const
{
    std::string fileWithPath = this->basePath_ + filename;

    std::ifstream plyFile( fileWithPath.c_str() );

    if ( ! plyFile.is_open() )
    {	// Didn't open file, so return
        return false;
    }

    this->ReadFileToToken( plyFile, "vertex" );
    plyFile >> theMesh.numberOfVertices;

    this->ReadFileToToken( plyFile, "face" );
    plyFile >> theMesh.numberOfTriangles;		

    this->ReadFileToToken( plyFile, "end_header" );

    // Allocate the appropriate sized array (+a little bit)
    theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
    theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

    // Read vertices
    for ( int index = 0; index < theMesh.numberOfVertices; index++ )
    {
        //end_header
        //-0.0312216 0.126304 0.00514924 0.850855 0.5 		
        float x, y, z;

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
bool cModelAssetLoader::LoadPlyFileIntoMeshWithNormals(const std::string filename, cMesh &theMesh ) const
{
    std::string fileWithPath = this->basePath_ + filename;
    std::ifstream plyFile( fileWithPath.c_str() );

    if ( ! plyFile.is_open() )
    {	// Didn't open file, so return
        return false;
    }

    this->ReadFileToToken( plyFile, "vertex" );
    plyFile >> theMesh.numberOfVertices;

    this->ReadFileToToken( plyFile, "face" );
    plyFile >> theMesh.numberOfTriangles;		

    this->ReadFileToToken( plyFile, "end_header" );

    // Allocate the appropriate sized array (+a little bit)
    theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
    theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

    // Read vertices
    for ( int index = 0; index < theMesh.numberOfVertices; index++ )
    {
        //-0.0312216 0.126304 0.00514924 0.850855 0.5 		
        float x, y, z, nx, ny, nz;

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
        // 3 164 94 98 
        int discard = 0;
        plyFile >> discard;									// 3
        plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
        plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
        plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
    }

    return true;
}

bool cModelAssetLoader::LoadPlyFileIntoMeshWithNormalsAndColours(const std::string filename, cMesh & theMesh) const
{
    std::string fileWithPath = this->basePath_ + filename;
    std::ifstream plyFile(fileWithPath.c_str());

    if (!plyFile.is_open())
    {	// Didn't open file, so return
        return false;
    }

    this->ReadFileToToken(plyFile, "vertex");
    plyFile >> theMesh.numberOfVertices;

    this->ReadFileToToken(plyFile, "face");
    plyFile >> theMesh.numberOfTriangles;

    this->ReadFileToToken(plyFile, "end_header");

    // Allocate the appropriate sized array (+a little bit)
    theMesh.pVertices = new sVertex[theMesh.numberOfVertices];
    theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

    // Read vertices
    for (int index = 0; index < theMesh.numberOfVertices; index++)
    {
        //end_header
        float x, y, z, nx, ny, nz;//, confidence, intensity;
        float r, g, b, a;

        plyFile >> x;
        plyFile >> y;
        plyFile >> z;
        plyFile >> nx >> ny >> nz;
        plyFile >> r >> g >> b >> a;

        theMesh.pVertices[index].x = x;	
        theMesh.pVertices[index].y = y;	
        theMesh.pVertices[index].z = z;
        theMesh.pVertices[index].r = r / 255.0f;	
        theMesh.pVertices[index].g = g / 255.0f;	
        theMesh.pVertices[index].b = b / 255.0f;	
        theMesh.pVertices[index].a = 1.0f;
        theMesh.pVertices[index].nx = nx;	
        theMesh.pVertices[index].ny = ny;	
        theMesh.pVertices[index].nz = nz;	
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

    return true;
}

bool cModelAssetLoader::LoadPlyFileIntoMeshWithNormalsAndUv(const std::string filename, cMesh& theMesh ) const
{
    std::string fileWithPath = this->basePath_ + filename;
    std::ifstream plyFile( fileWithPath.c_str() );

    if ( ! plyFile.is_open() )
    {	// Didn't open file, so return
        return false;
    }

    this->ReadFileToToken( plyFile, "vertex" );
    plyFile >> theMesh.numberOfVertices;

    this->ReadFileToToken( plyFile, "face" );
    plyFile >> theMesh.numberOfTriangles;		

    this->ReadFileToToken( plyFile, "end_header" );

    // Allocate the appropriate sized array (+a little bit)
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
            

        theMesh.pVertices[index].x = x;	
        theMesh.pVertices[index].y = y;	
        theMesh.pVertices[index].z = z; 
        theMesh.pVertices[index].r = 1.0f;
        theMesh.pVertices[index].g = 1.0f;
        theMesh.pVertices[index].b = 1.0f;
        theMesh.pVertices[index].nx = nx;
        theMesh.pVertices[index].ny = ny;
        theMesh.pVertices[index].nz = nz;

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

    return true;
}
