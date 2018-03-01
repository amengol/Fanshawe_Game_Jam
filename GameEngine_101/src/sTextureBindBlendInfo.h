#ifndef _sTextureBindBlendInfo_HG_
#define _sTextureBindBlendInfo_HG_

// Used to hold texture and (basic) blend information
// This is pretty basic: any textures are modulated (multiplied) 
//	together using the textureBlendRatio for each. 
// So all the blend ratios, together, should add to 1.0f

#include <string>

struct sTextureBindBlendInfo
{
	static const int INVALID_TEXTURE_ID = -1;
	sTextureBindBlendInfo() : texLookupCachedID(INVALID_TEXTURE_ID), blendRatio(0.0f) {};
	sTextureBindBlendInfo(std::string textureName)
	{
		this->texLookupCachedID = INVALID_TEXTURE_ID;
		this->blendRatio = 1.0f;
		this->name = textureName;
	};
	sTextureBindBlendInfo(std::string textureName, float blendRatio)
	{
		this->texLookupCachedID = INVALID_TEXTURE_ID;
		this->blendRatio = blendRatio;
		this->name = textureName;
	};
	std::string name;
	int texLookupCachedID;			// For faster lookup
	float blendRatio;			// 0.0 to 1.0f
		                        // All blend ratios should add to 1.0
};

#endif
