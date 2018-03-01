#ifndef _CTextureManager_HG_
#define _CTextureManager_HG_

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the MIT License:
// http://opensource.org/licenses/MIT
// (Also, it would be Supah Cool if I heard this was remotely useful!)
// Use this code at your own risk. It is indented only as a learning aid.

// Loads various textures into memory
// Prepares and loads textures into OpenGL texture units
// Stores texture to textureID look up

#include <string>
#include "CTextureFromBMP.h"
#include <map>
#include <string>
#include "../Error/COpenGLError.h"
#include <vector>

#include "CTexUnitInfoBrief.h"
//// Used in the GetBoundTextureUnitsFromTextureNames() method
//// (required for getting a list of the texture names and what unit they are bound to)
//class CTexUnitInfoBrief
//{
//public:
//	CTexUnitInfoBrief() : number(0), 
//		                  texUnitNum(INVALIDORUNKNOWN), 
//						  texUnitNumOffsetBy_GL_TEXTURE0(INVALIDORUNKNOWN), 
//	                      texType(INVALIDORUNKNOWN) {};
//	static const int INVALIDORUNKNOWN = INT_MAX;	// Used for texture numbers and units
//	std::string name;
//	GLint number;		// Not really needed, only every used for debugging since we are using the texture unit in the sampler
//	GLenum texUnitNum;
//	GLenum texUnitNumOffsetBy_GL_TEXTURE0;
//	GLenum texType;			// GL_TEXTURE_CUBE_MAP, GL_TEXTURE_2D, or GL_SAMPLER_2D_SHADOW for now
//};

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	static const int INVALIDORUNKNOWN = INT_MAX;	// Used for texture numbers and units

	void setBasePath( std::string basePath );
	bool loadTexture( std::string fileName );

	bool Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap );
	bool Create2DTexturesFromBMPFiles( std::vector<std::string> vecTextures, bool bGenerateMIPMap, std::string &error );
	bool CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
		                                std::string posX_fileName, std::string negX_fileName, 
	                                    std::string posY_fileName, std::string negY_fileName, 
										std::string posZ_fileName, std::string negZ_fileName, 
										bool bGenerateMipmap, bool bIsSeamless );
	// returns zero if no texture
	CTextureFromBMP* getTextureFromTextureName( std::string textureName );
	// returns -1 if no texture
	int getTextureIDFromTextureName( std::string textureName );

	// Loads the texture bindings alphabetically (i.e. how they are in the map)
	// (This is OK since you'd be doing a lookup on the texture by file name)
	bool UpdateTextureBindingsByTextureNameSimple(void);
	// TODO: Fancier method: Only bind needed textures, reusing bindings that are
	//	already present
	bool UpdateTextureBindings(std::vector<std::string> vecTextureNamesInUse);	// Slower
	bool UpdateTextureBindings(std::vector<int> vecTextureIDsInUse);

	// Returns false if the texture isn't bound to anything
	bool GetBoundTextureUnitFromTextureName( std::string textureName, GLenum &textureUnit, bool bOffsetFrom_GL_TEXTURE0 = false );
	// Returns a map of texture names and the texture binding information
	bool GetBoundTextureUnitsByTextureNames( std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo );

	void GetLoadedTextures(std::vector<std::string> &vecTextures);

	int getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS(bool bMakeGLCallNow = false);
	int getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS(bool bMakeGLCallNow = false);
	//int getOpenGL_GL_MAX_TEXTURE_UNITS(void);	// Fixed function only, so even need this?

	// TODO (INCOMPLETE): Allows the texture manager to create and manage framebuffers
	class CFrameBufferInfo
	{
	public:
		CFrameBufferInfo(): 
			ID(0), width(0.0f), height(0.0f), bHadColourBuffers(false), 
			bHasDepthBuffer(false), depthBufferInternalFormat(0), depthBuffer_texture_ID(0) {};
		//~CFramebufferInfo();
		std::string name;
		GLuint ID;			// From OpenGL
		float width;
		float height;
		//
		bool bHadColourBuffers;
		class CColourBuffer
		{
		public:
			CColourBuffer() : colourBufferAttachment(0), colourBufferInternalFormat(0), colourBuffer_texture_ID(0) {};
			GLenum colourBufferAttachment;
			GLenum colourBufferInternalFormat;
			GLuint colourBuffer_texture_ID;		// From OpenGL
		};
		std::map< GLuint, CColourBuffer > mapColourBuffersByID;
		//
		bool bHasDepthBuffer;		GLenum depthBufferInternalFormat;
		GLuint depthBuffer_texture_ID;	
	};
	// This uses the height and width from the frameBufferInfo, but bases on the rest of the information
	bool createNewOffscreenFrameBuffer(CFrameBufferInfo &frameBufferInfo, GLenum colourBuffersInternalFormat, GLuint numberOfColourBuffers, bool bHasDepthBuffer);
	// This bases the framebuffer information from what's contained in the frameBufferInfo object
	bool createNewOffscreenFrameBuffer(CFrameBufferInfo &frameBufferInfo);
	bool deleteOffscreenFrameBuffer( std::string name );
	bool deleteOffscreenFrameBuffer( GLuint ID );
	GLuint getFrameBufferIDFromName( std::string name );
	bool getFrameBufferInfoFromName( std::string name, CFrameBufferInfo &frameBufferInfo );
	bool getFrameBufferInfoFromID( GLuint ID, CFrameBufferInfo &frameBufferInfo );

	std::string getLastError(void);

	//// Used for offscreen rendering
	//GLuint getNextFreeTextureUnit(void);
	//bool reserveNextFreeTextureUnit(void);

	void ShutDown(void);

	// TODO: Update these maps by texture ID, rather than name
	//       then add lookups from name to ID. 
	std::map< std::string, CTextureFromBMP* > m_map_TexNameToTexture;
	std::map< GLenum /*textureUnit*/, std::string /*textureName*/ >		m_map_TexUnitToTexName;
	// The texture units can have a number of "types" of textures (2D, skymap, etc.)
	// For instance, texture unit 1 could have a 2D texture one pass, but a cubemap the next
	class CTextureUnitInfo
	{
	public:
		// Texture unit 9999 doesn't exist (likely, or we're not using 9999 textures
		static const unsigned int INVLAID_TEXTURE_NUMBER = 9999;
		CTextureUnitInfo(): texUnitNumber(INVLAID_TEXTURE_NUMBER), texUnitNumOffsetBy_GL_TEXTURE0(INVLAID_TEXTURE_NUMBER), pTexture(0) {};
		GLenum texUnitNumber;
		unsigned int texUnitNumOffsetBy_GL_TEXTURE0;
		CTextureFromBMP* pTexture;
	};
	//std::map< std::string /*textureName*/, GLenum /*textureUnit*/ >		m_map_TexNameToTexUnit;
	std::map< std::string /*textureName*/, CTextureUnitInfo /*textureUnit*/ >		m_map_TexNameToTexUnit;
//	std::map< int /*textureID*/, CTextureUnitInfo /*textureUnit*/ >		m_map_TexNameToTexUnit;

private:
	GLuint	m_currentFrameBuffer;		// Zero for default

	//GLuint m_nextTextureUnitOffset;
	//static const GLuint m_BASETEXTURE = GL_TEXTURE0;
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );

	int m_MaxCombinedTextureImageUnits;
	int m_MaxTextureImageUnits;

	//COpenGLError m_GLErrorMotron;
};

#endif




