#ifndef _cVertex_xyz_rgb_n_HG_
#define _cVertex_xyz_rgb_n_HG_

// Contains vertex information, generally loaded from a file
class cVertex_xyz_rgb_n
{
public:
	cVertex_xyz_rgb_n();
	~cVertex_xyz_rgb_n();
	float x, y, z;		
	float r, g, b;
	float nx, ny, nz;
};

#endif
