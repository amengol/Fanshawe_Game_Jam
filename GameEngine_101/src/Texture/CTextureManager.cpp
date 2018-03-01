#include "CTextureManager.h"
#include <sstream>

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.ca
// It may be distributed under the terms of the MIT License:
// http://opensource.org/licenses/MIT
// (Also, it would be Supah Cool if I heard this was remotely useful!)
// Use this code at your own risk. It is indented only as a learning aid.

CTextureManager::CTextureManager()
{
	this->m_currentFrameBuffer = 0;	// Zero is default framebuffer

	this->getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS(true);
	this->getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS(true);

	return;
}

CTextureManager::~CTextureManager()
{
	return;
}

int CTextureManager::getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS(bool bMakeGLCallNow /*= false*/)
{
	if ( bMakeGLCallNow )
	{
		glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &(this->m_MaxCombinedTextureImageUnits) );
	}
	return this->m_MaxCombinedTextureImageUnits;
}

int CTextureManager::getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS(bool bMakeGLCallNow /*= false*/)
{
	if ( bMakeGLCallNow )
	{
		glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &(this->m_MaxTextureImageUnits) );
	}
	return this->m_MaxTextureImageUnits;
}


void CTextureManager::setBasePath( std::string basePath )
{
	this->m_basePath = basePath;
	return;
}

void CTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

void CTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}

// Make an alternative to this, where the textures "in use" are bound, but 
//	anything else that's loaded isn't.
bool CTextureManager::UpdateTextureBindingsByTextureNameSimple(void)
{
	// Go through the map of textures and set the textures in order.
	
	// Clear the "texture unit to texture name" map (as it's likely invalid)
	this->m_map_TexUnitToTexName.clear();

	// Starting with the texture unit #0 (GL_TEXTURE0), bind the textures
	//	to the texture units in the order that they appear in the map
	GLenum textureUnitIndex = 0;
	for ( std::map< std::string, CTextureFromBMP* >::iterator itTexture = this->m_map_TexNameToTexture.begin();
		  itTexture != this->m_map_TexNameToTexture.end(); itTexture++, textureUnitIndex++ )
	{
		// Activate the texture unit
		glActiveTexture( textureUnitIndex + GL_TEXTURE0 );

		if ( itTexture->second->getIs2DTexture() )
		{
			std::string textureNameDebug = itTexture->second->getTextureName();
			GLuint textureNumber = itTexture->second->getTextureNumber();
			// Set this texture to the active texture unit
			glBindTexture( GL_TEXTURE_2D, textureNumber );
		}
		else
		{
			glBindTexture( GL_TEXTURE_CUBE_MAP, itTexture->second->getTextureNumber() );
		}// if ( itTexture->second->getIs2DTexture() )

		this->m_map_TexUnitToTexName[textureUnitIndex + GL_TEXTURE0] = itTexture->second->getTextureName();
	}// for ( std::map< std::string, CTextureFromBMP* >::iterator...

	// Now update the other "name to texture unit" map
	this->m_map_TexNameToTexUnit.clear();
	for ( std::map< GLenum /*textureUnit*/, std::string /*textureName*/ >::iterator itTextName = this->m_map_TexUnitToTexName.begin();
		  itTextName != this->m_map_TexUnitToTexName.end(); itTextName++ )
	{
		//this->m_map_TexNameToTexUnit[ itTextName->second ] = itTextName->first;
		CTextureUnitInfo textureInfo;
		textureInfo.texUnitNumber = itTextName->first;
		textureInfo.texUnitNumOffsetBy_GL_TEXTURE0 = itTextName->first - GL_TEXTURE0;		// Simply for debugging, I guess
		textureInfo.pTexture = this->m_map_TexNameToTexture.find( itTextName->second )->second;
		this->m_map_TexNameToTexUnit[ itTextName->second ] = textureInfo;
	}

	return true;
}


void CTextureManager::GetLoadedTextures(std::vector<std::string> &vecTextures)
{
	vecTextures.clear();

	for ( std::map< std::string, CTextureFromBMP* >::iterator itTex = this->m_map_TexNameToTexture.begin();
		  itTex != this->m_map_TexNameToTexture.end(); itTex++ )
	{
		vecTextures.push_back( itTex->first );
	}
	return;
}


// Returns true if the texture is present and bound to something
// textureUnit is undefined if return is false
bool CTextureManager::GetBoundTextureUnitFromTextureName( std::string textureName, GLenum &textureUnit, bool bOffsetFrom_GL_TEXTURE0 /*= false*/ )
{
	if ( textureName == "" )
	{
		return false;
	}

	//std::map< std::string /*textureName*/, GLenum /*textureUnit*/ >::iterator itTexture = this->m_map_TexNameToTexUnit.find( textureName );
	std::map< std::string /*textureName*/, CTextureUnitInfo /*textureUnit*/ >::iterator itTexture = this->m_map_TexNameToTexUnit.find( textureName );
	if ( itTexture != this->m_map_TexNameToTexUnit.end() )
	{
		textureUnit = itTexture->second.texUnitNumber;
		if ( bOffsetFrom_GL_TEXTURE0 )
		{
			textureUnit -= GL_TEXTURE0;
		}
		return true;
	}
	return false;
}

// Left off here. Was getting the map of textures and texture units from the texture manager.
// This thing will take these and map them to the uniforms that are in the actual shader. 
// Returns a map of texture names and the texture binding information
// Note: this stores the info in the CTexUnitInfoBrief class because the "heavy" CTextureUnitInfo is used internally.
bool CTextureManager::GetBoundTextureUnitsByTextureNames( std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo )
{
	mapTexAndUnitInfo.clear();

	std::map< std::string /*textureName*/, CTextureUnitInfo /*textureUnit*/ >::iterator itTexture = this->m_map_TexNameToTexUnit.begin();
	for ( ; itTexture != this->m_map_TexNameToTexUnit.end(); itTexture++ )
	{
		CTexUnitInfoBrief curTexInfo;
		curTexInfo.name = itTexture->first;
		curTexInfo.number = itTexture->second.pTexture->getTextureNumber();
		curTexInfo.texUnitNum = itTexture->second.texUnitNumber;
		curTexInfo.texUnitNumOffsetBy_GL_TEXTURE0 = itTexture->second.texUnitNumOffsetBy_GL_TEXTURE0;
		if ( itTexture->second.pTexture->getIs2DTexture() )
		{
			curTexInfo.texType = GL_TEXTURE_2D;
			// TODO: This could be GL_SAMPLER_2D, GL_INT_SAMPLER_2D (ES30), GL_UNSIGNED_INT_SAMPLER_2D (ES30), or GL_SAMPLER_2D_SHADOW
			// How can you tell, for matching uniforms? 
		}
		else if ( itTexture->second.pTexture->getIsCubeMap() )
		{
			curTexInfo.texType = GL_TEXTURE_CUBE_MAP;
		}
		mapTexAndUnitInfo[itTexture->first] = curTexInfo;
	}

	return true;
}

	
bool CTextureManager::Create2DTexturesFromBMPFiles( std::vector<std::string> vecTextures, bool bGenerateMIPMap, std::string &error )
{
	std::stringstream ssErrors;

	bool bAllGood = true;

	for ( std::vector<std::string>::iterator itTexture = vecTextures.begin(); itTexture != vecTextures.end(); itTexture++ )
	{
		if ( ! this->Create2DTextureFromBMPFile( *itTexture, bGenerateMIPMap ) )
		{
			bAllGood = false;
			ssErrors << "Error: Could not load texture \"" << *itTexture << "\".\n";
		}
	}
	error = ssErrors.str();
	return bAllGood;
}


bool CTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;

	// #define GL_TEXTURE0 0x84C0
	//GLuint textureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
	//
	//// Gone over the GL_TEXTURE31? ('cause we can't load any more, then)
	//if ( textureUnit > GL_TEXTURE31 )		// #define GL_TEXTURE31 0x84DF
	//{
	//	this->m_appendErrorStringLine( "Too many textures are loaded. GL_TEXTURE31 is maximum." );
	//	return false;
	//}

	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoadFullPath );
		this->m_appendErrorString( "\n" );
		return false;
	}

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ textureFileName ] = pTempTexture;

	return true;
}



bool CTextureManager::CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
													 std::string posX_fileName, std::string negX_fileName, 
	                                                 std::string posY_fileName, std::string negY_fileName, 
									                 std::string posZ_fileName, std::string negZ_fileName, 
									                 bool bGenerateMipmap, bool bIsSeamless )
{
	std::string posX_fileName_FULL = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileName_FULL = this->m_basePath + "/" + negX_fileName;
	std::string posY_fileName_FULL = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileName_FULL = this->m_basePath + "/" + negY_fileName;
	std::string posZ_fileName_FULL = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileName_FULL = this->m_basePath + "/" + negZ_fileName;


	// #define GL_TEXTURE0 0x84C0
	//GLuint textureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
	
	//// Gone over the GL_TEXTURE31? ('cause we can't load any more, then)
	//if ( textureUnit > GL_TEXTURE31 )		// #define GL_TEXTURE31 0x84DF
	//{
	//	this->m_appendErrorStringLine( "Too many textures are loaded. GL_TEXTURE31 is maximum." );
	//	return false;
	//}

	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	GLenum errorEnum = GL_NO_ERROR;
	std::string errorString;
	std::string errorDetails;
	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( cubeMapName,
		                                                   posX_fileName_FULL, negX_fileName_FULL,
		                                                   posY_fileName_FULL, negY_fileName_FULL,
														   posZ_fileName_FULL, negZ_fileName_FULL,
														   /*textureUnit,*/ bIsSeamless, 
														   errorEnum, errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( cubeMapName );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorStringLine( errorString );
		this->m_appendErrorStringLine( errorDetails );
		return false;
	}



	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ cubeMapName ] = pTempTexture;

	return true;
}


// Returns zero if invalid
CTextureFromBMP* CTextureManager::getTextureFromTextureName( std::string textureName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture = this->m_map_TexNameToTexture.find(textureName);

	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}

	return itTexture->second;
}

// returns -1 if no texture
int CTextureManager::getTextureIDFromTextureName( std::string textureName )
{
	CTextureFromBMP* pTex = this->getTextureFromTextureName(textureName);
	if ( pTex )
	{
		return pTex->getTextureNumber();
	}
	return -1;
}


std::string CTextureManager::getLastError(void)
{
	std::string errorText = this->m_lastError;
	this->m_lastError = "";
	return errorText;
}

//// These are used for off-screen rendering, or any time that you want to 
//// manually set a texture unit
//GLuint CTextureManager::getNextFreeTextureUnit(void)
//{
//	// #define GL_TEXTURE0 0x84C0
//	GLuint nextTextureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
//	return nextTextureUnit;
//}

//bool CTextureManager::reserveNextFreeTextureUnit(void)
//{
//	GLuint nextTextureUnit = this->m_BASETEXTURE + this->m_nextTextureUnitOffset;
//	// Gone over the GL_TEXTURE31? ('cause we can't load any more, then)
//	if ( nextTextureUnit > GL_TEXTURE31 )		// #define GL_TEXTURE31 0x84DF
//	{
//		this->m_appendErrorStringLine( "Too many textures are loaded. GL_TEXTURE31 is maximum." );
//		return false;
//	}
//	// There's room to allocate another texture unit
//	this->m_nextTextureUnitOffset++;
//	return true;
//}
//	

	// NEW: February, 2015: Allows the texture manager to create and manage framebuffers
	class CFramebufferInfo
	{
	public:
		CFramebufferInfo(): 
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

bool CTextureManager::createNewOffscreenFrameBuffer(CFrameBufferInfo &frameBufferInfo, GLenum colourBuffersInternalFormat, 
													GLuint numberOfColourBuffers, bool bHasDepthBuffer)
{
	// TODO: Not implemented, yet
	return false;
}

// This bases the framebuffer information from what's contained in the frameBufferInfo object
bool CTextureManager::createNewOffscreenFrameBuffer(CFrameBufferInfo &frameBufferInfo)
{

	return false;

	// Is the size OK
	if ( ( frameBufferInfo.width <= 0.0f ) || ( frameBufferInfo.height <= 0.0f ) )
	{
		this->m_appendErrorStringLine( "Framebuffer width of height is zero (0.0f) or less" );
		return false;
	}


	// Create the actual offscreen texture
	//::g_FrameBufferHeight = static_cast<float>(::g_screenHeight);
	//::g_FrameBufferWidth = static_cast<float>(::g_screenWidth);

	glGenFramebuffers(1, &(frameBufferInfo.ID) );
	glBindFramebuffer( GL_FRAMEBUFFER, frameBufferInfo.ID );

	//if ( frameBufferInfo.mapColourBuffersByID.count() != 0 )
	GLenum colourBufferID = GL_COLOR_ATTACHMENT0;
	// Note: We've already checked that there are no more than 16 colour buffers (so last will be GL_COLOR_ATTACHMENT15 )
	for ( std::map< GLuint, CFrameBufferInfo::CColourBuffer >::iterator itCB = frameBufferInfo.mapColourBuffersByID.begin();
		  itCB != frameBufferInfo.mapColourBuffersByID.end(); itCB++ )
	{

	}
	//for ( GLuint count = 0; count != numberOfColourBuffers; count++ )
	//{
	//	CFrameBufferInfo::CColourBuffer tempColourBuffer;
	//	tempColourBuffer.colourBufferAttachment = GL_COLOR_ATTACHMENT0 + count;
	//	tempColourBuffer.colourBufferInternalFormat 

	//// Create the texture to write the "colour" stuff to 
	//glGenTextures( 1, &g_FrameBufferColourTexture );
	//glBindTexture( GL_TEXTURE_2D, g_FrameBufferColourTexture );
	//// Should really be a square, and a power of 2
	//glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, static_cast<GLsizei>(::g_FrameBufferWidth), static_cast<GLsizei>(::g_FrameBufferHeight) );
	////glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, ::g_screenWidth, ::g_screenWidth );

	//// Turn off MipMapping
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//// Create the depth texture we are going to write to (we'll need that as well)
	//glGenTextures( 1, &g_FrameBufferDepthTexture );
	//glBindTexture( GL_TEXTURE_2D, g_FrameBufferDepthTexture );
	//glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, static_cast<GLsizei>(::g_FrameBufferWidth), static_cast<GLsizei>(::g_FrameBufferHeight) );
	////glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, ::g_screenWidth, ::g_screenWidth );

	//// Attach the colour and depth textures to the frame buffer object
	//glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ::g_FrameBufferColourTexture, 0 );
	//glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ::g_FrameBufferDepthTexture, 0 );

	//this->getLastError(
	return true;
}

void CTextureManager::ShutDown(void)
{
	// TODO: Implement this

	return;
}

//bool CTextureManager::deleteOffscreenFrameBuffer( std::string name )
//bool CTextureManager::deleteOffscreenFrameBuffer( GLuint ID )
//GLuint CTextureManager::getFrameBufferIDFromName( std::string name )
//bool CTextureManager::getFrameBufferInfoFromName( std::string name, CFrameBufferInfo &frameBufferInfo )
//bool CTextureManager::getFrameBufferInfoFromID( GLuint ID, CFrameBufferInfo &frameBufferInfo )
