#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_

#include <string>
#include <map>
#include "sVAOInfo.h"

class cMesh;

// This will handle: 
// - taking a cMesh and loading into the various buffers
// - take a mesh "name" and returning the desired VAO info 
//   (for rendering)
class cVAOMeshManager
{
public:
	cVAOMeshManager();
	~cVAOMeshManager();
	
	// Take name from mesh for lookup (for rendering)
	bool loadMeshIntoStaticVAO( cMesh& theMesh, int shaderID, bool bKeepMesh = false );

	// During rendering (aka 'drawing'), get the info we need.
	bool lookupStaticVAOFromName( std::string name, sVAOInfo &theVAOInfo );

    // Return mesh by name
    bool lookupMeshFromName(std::string name, cMesh &theMesh);

    // Generates a Dynamic VAO
    void genDynamicVAO(std::string name, int shaderID);

    // During rendering (aka 'drawing'), get the info we need.
    void lookupDynamicVAOFromName(std::string name, sVAOInfo &theVAOInfo);

    // Loads the mesh into the Dynamic VAO refering its SVAOInfo name
    void loadMeshIntoDynamicVAO(cMesh& theMesh, std::string SVAOInfoName);

	std::map<int, std::string> mapNumberToName;

private:
	// Look up from name to sVAOInfo
	// 1st is what I'm indexing by (i.e. type)
	// 2nd is what I'm actually storing (the type)
	std::map< std::string, sVAOInfo > m_mapNameToStaticVAO;

    std::map< std::string, sVAOInfo > m_mapNameToDynamicVAO;

    // Meshes that I want to keep around (like the terrain?)
    std::map< std::string, cMesh > m_mapNameToMesh;
};
#endif
