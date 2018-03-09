#ifndef _cModelAssetLoader_HG_
#define _cModelAssetLoader_HG_

#include <string>
#include <fstream>

class cMesh;

class cModelAssetLoader
{
public:
	cModelAssetLoader();


	// Note the & symbol which is "by reference" which means
	//	it's NOT a copy, but referring to the original object.
	// ALSO note that this is a C++ thing (& isn't a thing in C)
	bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh );
	bool LoadPlyFileIntoMeshWithNormals( std::string filename, cMesh &theMesh );
    bool LoadPlyFileIntoMeshWithNormals_and_colours(std::string filename, cMesh &theMesh);
	bool LoadPlyFileIntoMeshWith_Normals_and_UV( std::string filename, cMesh &theMesh );
	
	// Is the full path, so requires end slash. 
	// Is directly pre-pended to any file that's being loaded.
	void setBasePath( std::string fullPathWITHslash );
	std::string getBasePath(void);

private:
	void m_ReadFileToToken( std::ifstream &file, std::string token );

	std::string m_basePath;
};

#endif
