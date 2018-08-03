#ifndef _cModelAssetLoader_HG_
#define _cModelAssetLoader_HG_

#include <string>

class cMesh;

class cModelAssetLoader
{
public:
    cModelAssetLoader();

    bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh ) const;
    bool LoadPlyFileIntoMeshWithNormals( std::string filename, cMesh &theMesh ) const;
    bool LoadPlyFileIntoMeshWithNormalsAndColours(std::string filename, cMesh &theMesh) const;
    bool LoadPlyFileIntoMeshWithNormalsAndUv( std::string filename, cMesh &theMesh ) const;
    
    void SetBasePath( std::string fullPath );
    std::string GetBasePath() const;

private:
    static void ReadFileToToken( std::ifstream &file, std::string token );
    std::string basePath_;
};

#endif
