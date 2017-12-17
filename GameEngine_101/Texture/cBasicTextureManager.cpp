#include "cBasicTextureManager.h"

#include <sstream>

void cBasicTextureManager::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}


bool cBasicTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	//if ( ! pTempTexture->CreateNewTextyreFromBMPFileSOIL( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
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

bool cBasicTextureManager::CreateNewCubeTextureFromBMPFiles( std::string cubeMapName, 
		                                   std::string posX_fileName, std::string negX_fileName, 
		                                   std::string posY_fileName, std::string negY_fileName, 
										   std::string posZ_fileName, std::string negZ_fileName )
{
	std::string posX_fileFullPath = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileFullPath = this->m_basePath + "/" + negX_fileName; 
	std::string posY_fileFullPath = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileFullPath = this->m_basePath + "/" + negY_fileName; 
	std::string posZ_fileFullPath = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileFullPath = this->m_basePath + "/" + negZ_fileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	GLenum errorEnum = 0;
	std::string errorString;
	std::string errorDetails;
	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( cubeMapName, 
	                         posX_fileFullPath, negX_fileFullPath, 
	                         posY_fileFullPath, negY_fileFullPath, 
	                         posZ_fileFullPath, negZ_fileFullPath, 
	                         true /*is seamless*/, errorEnum, 
	                         errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( cubeMapName );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorString( errorString );
		this->m_appendErrorString( errorDetails );
		return false;
	}

	this->m_map_TexNameToTexture[ cubeMapName ] = pTempTexture;

	return true;
}

std::string cBasicTextureManager::getLastError( bool bClearError /*=true*/ )
{
	std::string errorString = this->m_lastError;
	if ( bClearError ) { this->m_lastError = ""; }
	return errorString;
}


void cBasicTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

GLuint cBasicTextureManager::getTextureIDFromName( std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );
	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}


void cBasicTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}