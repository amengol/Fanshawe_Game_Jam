#ifndef _sVertex_HG_
#define _sVertex_HG_

// This represents the vertex as used in the vertex layout.
// It's also the vertex representation used in cMesh
// (it replaces the cVertex in the cVAOMeshManager and the cVertex_xyz_rgb_n structures)

#include <cstring>		// for memset

static const unsigned int NUMBEROFBONES = 4;

struct sVertex
{
	sVertex() :
		x(0.0f), y(0.0f), z(0.0f),
		r(0.0f), g(0.0f), b(0.0f), a(1.0f),		// Note alpha is 1.0
		nx(0.0f), ny(0.0f), nz(0.0f),
		u1(0.0f), v1(0.0f), 
		u2(0.0f), v2(0.0f),
		tx(0.0f), ty(0.0f), tz(0.0f),
		bx(0.0f), by(0.0f), bz(0.0f)
    {
//#ifdef _DEBUG
        memset(this->boneID, 0, sizeof(unsigned int) * NUMBEROFBONES);
        memset(this->boneWeights, 0, sizeof(float) * NUMBEROFBONES);
//#endif // DEBUG
    };

	float x, y, z;
	float r, g, b, a;
	float nx, ny, nz;
	float u1, v1;
	float u2, v2;		
	float tx, ty, tz;	// tangent
	float bx, by, bz;	// bi-normal
    // For the 4 bone skinned mesh information
	float boneID[NUMBEROFBONES]; 		// New		// starts at 80
	float boneWeights[NUMBEROFBONES];	// New		// starts at 96
};


#endif