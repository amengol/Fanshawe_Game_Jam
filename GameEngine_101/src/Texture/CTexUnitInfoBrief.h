#ifndef _CTexUnitInfoBrief_HG_
#define _CTexUnitInfoBrief_HG_

#include "../globalOpenGL_GLFW.h"

// Used in the GetBoundTextureUnitsFromTextureNames() method
// (required for getting a list of the texture names and what unit they are bound to)
class CTexUnitInfoBrief
{
public:
	CTexUnitInfoBrief() : number(0), 
		                  texUnitNum(INVALIDORUNKNOWN), 
						  texUnitNumOffsetBy_GL_TEXTURE0(INVALIDORUNKNOWN), 
	                      texType(INVALIDORUNKNOWN) {};
	static const int INVALIDORUNKNOWN = INT_MAX;	// Used for texture numbers and units
	std::string name;
	GLint number;		// Not really needed, only every used for debugging since we are using the texture unit in the sampler
	GLenum texUnitNum;
	GLenum texUnitNumOffsetBy_GL_TEXTURE0;
	// This is the texture unit type: GL_TEXTURE_CUBE_MAP, GL_TEXTURE_2D, or GL_SAMPLER_2D_SHADOW for now (could also be GL_TEXTURE_1D)
	// i.e. the format of the image as loaded into GPU memory
	GLenum texType;		

	// This is the shader sampler type that is required or that can read it
	// Note: It's unclear about some of these, as there are many more sampler types that texture unit type. 
	// Some of them might be completely compatible
//	GLenum samplerTypeNeeded;	// GL_SAMPLER_2D, GL_SAMPLER_CUBE for now, but there are many others
	// Is this the place to set this, or can there be a range of samplers that can be used? 
	// GL_SAMPLER_1D	- **GL_TEXTURE_1D**
	// GL_SAMPLER_2D	- **GL_TEXTURE_2D**
	// GL_SAMPLER_3D, 
	// GL_SAMPLER_CUBE  - **GL_TEXTURE_CUBE_MAP**
	// 
	// These seem to be OpenGL ES 3.0, maybe?
	// GL_INT_SAMPLER_1D	- compatible with GL_TEXTURE_1D??
	// GL_INT_SAMPLER_2D	- compatible with GL_TEXTURE_2D??
	// GL_INT_SAMPLER_3D 
	// GL_INT_SAMPLER_CUBE	- compatible with GL_TEXTURE_CUBE_MAP??
	// 
	// GL_UNSIGNED_INT_SAMPLER_1D	- compatible with GL_TEXTURE_1D??
	// GL_UNSIGNED_INT_SAMPLER_2D	- compatible with GL_TEXTURE_2D??
	// GL_UNSIGNED_INT_SAMPLER_3D 
	// GL_UNSIGNED_INT_SAMPLER_CUBE	- compatible with GL_TEXTURE_2D??
	// 
	// GL_SAMPLER_1D_SHADOW
	// GL_SAMPLER_2D_SHADOW		- **GL_TEXTURE_2D**
	// GL_SAMPLER_CUBE_SHADOW	- compatible with GL_TEXTURE_2D??
};


#endif
