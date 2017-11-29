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
	bool loadMeshIntoVAO( cMesh &theMesh, int shaderID );

	// During rendering (aka 'drawing'), get the info we need.
	bool lookupVAOFromName( std::string name, sVAOInfo &theVAOInfo );

	std::map<int, std::string> mapNumberToName;

private:
	// Look up from name to sVAOInfo
	// 1st is what I'm indexing by (i.e. type)
	// 2nd is what I'm actually storing (the type)
	std::map< std::string, sVAOInfo > m_mapNameToVAO;
};
#endif
