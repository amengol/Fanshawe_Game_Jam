// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the MIT License:
// http://opensource.org/licenses/MIT
// (Also, it would be Supah Cool if I heard this was remotely useful!)
// Use this code at your own risk. It is indented only as a learning aid.

//
#include "CTextureFromBMP.h"
#include <iostream>

#include <fstream>
#include <iostream>

//#define DEBUG_MODE_BITMAPREAD
//#define DEBUG_MODE_BITMAPDISPLAY
//#define DEBUG_MODE_TEXTUREREPLACEMENT

// SOIL doesn't work - requires wgl functions from windows
//#include "../include/SOIL.h"


CTextureFromBMP::CTextureFromBMP()
: m_Height(0), m_Width(0), m_OriginalHeight(0), m_OriginalWidth(0), m_p_theImages(0),
  m_lastErrorNum(0), m_FileSize(0), m_offsetInBits(0), m_headerSize(0),
  m_numberOfRows(0), m_numberOfColumns(0), m_numberOfPlanes(0), 
  m_bitPerPixel(0), m_compressionMode(0), m_imageSizeInBytes(0),
  m_PixelsPerMeterX(0), m_PixelsPerMeterY(0), 
  m_numberOfLookUpTableEntries(0), m_numberOfImportantColours(0),
  m_textureNumber(0), m_bHave_cout_output(false), /*m_textureUnit(0),*/
  m_bIsCubeMap(false), m_bIs2DTexture(false)
{
	return;
}

CTextureFromBMP::~CTextureFromBMP()
{
	return;
}

void CTextureFromBMP::SetDebug_cout_output( bool bHave_cout_output )
{
	this->m_bHave_cout_output = bHave_cout_output;
}


GLuint CTextureFromBMP::getTextureNumber(void)
{
	return this->m_textureNumber;
}

std::string CTextureFromBMP::getTextureName(void)
{
	return this->m_textureName;
}

std::string CTextureFromBMP::getFileNameFullPath(void)
{
	return this->m_fileNameFullPath;
}

//bool CTextureFromBMP::CreateNewTextyreFromBMPFileSOIL( std::string textureName, std::string fileNameFullPath, bool bGenerateMIPMap )
//{
//	GLuint tex_2d = SOIL_load_OGL_texture
//	(
//		fileNameFullPath.c_str(),
//		SOIL_LOAD_AUTO,
//		SOIL_CREATE_NEW_ID,
//		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
//	);
//
//	if ( tex_2d == 0 )
//	{
//		return false;
//	}
//
//	this->m_textureName = textureName;
//	this->m_textureNumber = tex_2d;
//
//	return true;
//}


bool CTextureFromBMP::CreateNewTextureFromBMPFile2( std::string textureName, std::string fileNameFullPath, 
												    /*GLenum textureUnit,*/ bool bGenerateMIPMap )	
{
	bool bReturnVal = true;

	// Clear any old openGL errors???
	int IHateYou = glGetError();

	// Pick a texture number...
//	GLuint textureNum = 0;
	this->m_textureNumber = 0;
	glGenTextures( 1, &(this->m_textureNumber) );
	// Worked?
	if ( ( glGetError() & GL_INVALID_VALUE ) == GL_INVALID_VALUE )
	{
		bReturnVal = false;
		return false;
	}


	if ( !this->LoadBMP2( fileNameFullPath ) )
	{
		return false;
	}

	this->m_fileNameFullPath = fileNameFullPath;
	this->m_textureName = textureName;

	// Good to go (valid texture ID and loaded bitmap...
	// Now set the texture...
	//glActiveTexture( textureUnit );	// GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture(GL_TEXTURE_2D, this->m_textureNumber);
	//glTexImage2D( GL_TEXTURE_2D,		// target (2D, 3D, etc.)
	//			 0,					// MIP map level 
	//			 GL_RGBA,			// internal format
	//			 this->m_numberOfColumns,	// width (pixels)
	//			 this->m_numberOfRows,	// height (pixels)
	//			 0,					// border (0 or 1)
	//			 GL_RGB,			// format of pixel data
	//			 GL_UNSIGNED_BYTE,	// type of pixel data
	//			 this->m_p_theImages);	// pointer to data in memory
	glTexStorage2D( GL_TEXTURE_2D, 
		            1,			// MIP map
					GL_RGBA8, 
					this->m_numberOfColumns, 
					this->m_numberOfRows );

	if ( this->bWasThereAnOpenGLError() )	{ return false;	}

	//for ( int index = 0; index != 400; index++ )
	//{
	//	std::cout << (int)this->m_p_theImages[index].redPixel << ", ";
	//	std::cout << (int)this->m_p_theImages[index].greenPixel << ", ";
	//	std::cout << (int)this->m_p_theImages[index].bluePixel << std::endl;
	//}

//	glTextureSubImage2D( 
	glTexSubImage2D( GL_TEXTURE_2D, 
		             0,		// Level 0
					 0, 0,	// Offset of 0,0
					 this->m_numberOfColumns, 
					 this->m_numberOfRows,
					 GL_RGB,			// Pixel data format
					 GL_UNSIGNED_BYTE,	// Pixel data type  
					 this->m_p_theImages );

	if ( this->bWasThereAnOpenGLError() )	{ return false;	}

	this->ClearBMP();


	if ( bGenerateMIPMap )
	{
		glGenerateMipmap( GL_TEXTURE_2D );		// OpenGL 4.0
	}

	//if ( this->bWasThereAnOpenGLError() )	{ return false;	}


	//glBindTexture(GL_TEXTURE_2D, m_textureNumber);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

//	GLfloat largest_supported_anisotropy;
//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);

	//if ( this->bWasThereAnOpenGLError() )	{ return false;	}

	//this->m_textureUnit = textureUnit;

	this->m_bIs2DTexture = true;

	//glBindTexture(GL_TEXTURE_2D, 0);

	return bReturnVal;
}


bool CTextureFromBMP::CreateNewCubeTextureFromBMPFiles( std::string cubeMapName, 
													    std::string posX_fileName, std::string negX_fileName, 
		                                                std::string posY_fileName, std::string negY_fileName, 
														std::string posZ_fileName, std::string negZ_fileName, 
														/*GLenum textureUnit, */bool bIsSeamless,
														GLenum &errorEnum, std::string &errorString, std::string &errorDetails )
{

	bool bReturnVal = true;

	// Do the magic...

	// Pick a texture number...
	//GLuint textureNum = 0;
	glGenTextures( 1, &(this->m_textureNumber) );
	// Worked?
	if ( ( glGetError() & GL_INVALID_VALUE ) == GL_INVALID_VALUE )
	{
		bReturnVal = false;
		return false;
	}

	//
	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture( textureUnit );	// GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture( GL_TEXTURE_CUBE_MAP, this->m_textureNumber ); 

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
 
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/ );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/ );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE /*GL_REPEAT*/ );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	// IF YOU DON'T GENERATE MIP MAPS, then you can use LINEAR filtering
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	if ( bIsSeamless )
	{
		glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );	// OpenGL 4.1, maybe
	}

	if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
					
	// Positive X image...
	// Assume all the images are the same size. If not, then it will screw up
	if ( this->LoadBMP( posX_fileName ) )
	{
		glTexStorage2D( GL_TEXTURE_CUBE_MAP, 
		            10, // Mipmap levels
					GL_RGBA8,	// Internal format
					this->m_numberOfColumns,	// width (pixels)
					this->m_numberOfRows );		// height (pixels)

		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	// Positive X image...
	glTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
		             0,   // Level
					 0, 0, // Offset
					 this->m_numberOfColumns,	// width
					 this->m_numberOfRows,		// height
					 GL_RGB, 
					 GL_UNSIGNED_BYTE,
					 this->m_p_theImages );
	this->ClearBMP();
	if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }


	// Negative X image...
	if ( this->LoadBMP( negX_fileName ) )
	{
		glTexSubImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
		this->ClearBMP();
		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	// Positive Y image...
	if ( this->LoadBMP( posY_fileName ) )
	{
		glTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
		this->ClearBMP();
		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	// Negative Y image...
	if ( this->LoadBMP( negY_fileName ) )
	{
		glTexSubImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
		this->ClearBMP();
		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	// Positive Z image...
	if ( this->LoadBMP( posZ_fileName ) )
	{
		glTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
		this->ClearBMP();
		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	// Negative Z image...
	if ( this->LoadBMP( negZ_fileName ) )
	{
		glTexSubImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
		this->ClearBMP();
		if ( this->bWasThereAnOpenGLError(errorEnum, errorString,  errorDetails) ) { return false; }
	}
	else
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}

	this->m_textureName = cubeMapName;

	//this->m_textureUnit = textureUnit;

	this->m_bIsCubeMap = true;

	// End of Do the magic...
	
	return bReturnVal;
}

// Same but generates a texture number automatically
bool CTextureFromBMP::CreateNewTextureFromBMPFile( std::string textureName, std::string fileNameFullPath /*, GLenum textureUnit*/ )	
{
	bool bReturnVal = true;

	// Pick a texture number...
	GLuint textureNum = 0;
	glGenTextures( 1, &(this->m_textureNumber) );
	// Worked?
	if ( ( glGetError() & GL_INVALID_VALUE ) == GL_INVALID_VALUE )
	{
		bReturnVal = false;
		return false;
	}
	
	// 
	// Read a 24 bit bitmap from a file and save it to an arrau
	std::ifstream theFile;
	// Indicate we are reading...
#ifdef DEBUG_MODE_BITMAPREAD	
	std::cout << "Reading texture file: " << fileName;
#endif
	// Note: This is set to binary mode - i.e. read the chars (bytes) as 
	//	binary - set in the ios_base (i/o stream base class 'openmode' setting.
	theFile.open(fileNameFullPath.c_str(), std::ios_base::binary);
	// Did it open?
	if (!theFile)
	{
		bReturnVal = false;
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
	}
	else
	{	// File openned. Now read it.
		char letter1, letter2;
		letter1 = ReadAByte(theFile);
		letter2 = ReadAByte(theFile);
		if ((letter1 != 'B') && (letter2 != 'M'))
		{
			bReturnVal = false;
			this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
		}
		else
		{	// File is BMP
			this->m_FileSize = ReadAnUnsignedLong(theFile);
			this->m_reserved1 = ReadAnUnsignedShort(theFile);
			this->m_reserved2 = ReadAnUnsignedShort(theFile);
			this->m_offsetInBits = ReadAnUnsignedLong(theFile);
            this->m_headerSize = ReadAnUnsignedLong(theFile);
            this->m_numberOfRows = ReadAnUnsignedLong(theFile);
			this->m_Height = m_OriginalHeight = m_numberOfRows;
            this->m_numberOfColumns = ReadAnUnsignedLong(theFile);
			this->m_Width = m_OriginalWidth = m_numberOfColumns;
            this->m_numberOfPlanes = ReadAnUnsignedShort(theFile);
            this->m_bitPerPixel = ReadAnUnsignedShort(theFile);
			// Is is a 24 bit bitmap?
			if (this->m_bitPerPixel != 24)
			{
				bReturnVal = false;
				this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
			}
			else
			{	// It's a 24 bit bitmap...
				this->m_compressionMode = ReadAnUnsignedLong(theFile);
				this->m_imageSizeInBytes = ReadAnUnsignedLong(theFile);
				this->m_PixelsPerMeterX = ReadAnUnsignedLong(theFile);
				this->m_PixelsPerMeterY = ReadAnUnsignedLong(theFile);
				this->m_numberOfLookUpTableEntries = ReadAnUnsignedLong(theFile);
				this->m_numberOfImportantColours = ReadAnUnsignedLong(theFile);
				// 
				long bytesPerRow = ((3 * m_numberOfRows + 3) / 4) * 4;
				long numberOfPaddingBytes = bytesPerRow - 3 * m_numberOfColumns;
				// Allocate enough space...
				this->m_p_theImages = new C24BitBMPpixel[m_numberOfRows * m_numberOfColumns];
				//m_p_theImages = new C32BitBMPpixel[m_numberOfRows * m_numberOfColumns];
				// Did we run out of memory?
				if (!this->m_p_theImages)
				{
					bReturnVal = false;
					m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
				}
				else
				{	// Read the bitmap into memory...
					double bytesRead = 0;
					double totalBytesInFile = m_numberOfRows * m_numberOfColumns;
					long pixelCount = 0;
					for (unsigned long row = 0; row < m_numberOfRows; row++)
					{
#ifdef DEBUG_MODE_BITMAPREAD
						std::cout << "Row:" << row << std::endl;
#endif
						for (unsigned long col = 0; col < m_numberOfColumns; col++)
						{
							char thered, thegreen, theblue;
							thered = theblue = thegreen = 0;
							theFile.get(theblue);
							theFile.get(thegreen);
							theFile.get(thered);
							this->m_p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
							this->m_p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
							this->m_p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);
							//m_p_theImages[pixelCount].alphaPixel = 255;
							
#ifdef DEBUG_MODE_BITMAPREAD
							std::cout << (static_cast<unsigned int>(thered) & 0x00FF) << ", ";
							std::cout << (static_cast<unsigned int>(thegreen) & 0x00FF) << ", ";
							std::cout << (static_cast<unsigned int>(theblue) & 0x00FF) << "\t";
#endif
							pixelCount++;
							bytesRead += 3;	// Add three bytes
						}
#ifdef DEBUG_MODE_BITMAPREAD
						std::cout << std::endl;
#endif
						// We are at the end of the row and there will 
						//	still be bytes padding up to a multiple of 4...
						// Read and discard these...
						for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
						{
							char tempChar;
							theFile >> tempChar;
						}
						// How much have we read?
						// NOTE: This really isn't percent, but it does the job of 
						//	printing out fewer dots.
						int percentRead = static_cast<int>( (static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
						if ((percentRead % 25) == 0)
						{
#ifdef DEBUG_MODE_BITMAPREAD
							std::cout << ".";
#endif
						}
					}
#ifdef DEBUG_MODE_BITMAPREAD
					std::cout << "complete." << std::endl;

#endif
					// BMP file is read....

					// Now set the texture...
					glBindTexture(GL_TEXTURE_2D, m_textureNumber);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT );
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT );
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
					glTexImage2D( GL_TEXTURE_2D,		// target (2D, 3D, etc.)
								 0,					// MIP map level 
								 GL_RGBA,			// internal format
								 m_numberOfColumns,	// width (pixels)
								 m_numberOfRows,	// height (pixels)
								 0,					// border (0 or 1)
								 GL_RGB,			// format of pixel data
								 GL_UNSIGNED_BYTE,	// type of pixel data
								 m_p_theImages);	// pointer to data in memory
					glBindTexture(GL_TEXTURE_2D, m_textureNumber);

					this->m_bIs2DTexture = true;

				} // if (!m_p_theImages) // Run out of memory?
			} // if (m_bitPerPixel != 24) // Not a 24 bit bitmap
		} // if ((letter1 != 'B') && (letter2 != 'M'))	// Not a bitmap file...
		// Close the file
		theFile.close();
		delete [] this->m_p_theImages;
	} // if (!theFile)   // Did the file open?

	this->m_fileNameFullPath = fileNameFullPath;
	this->m_textureName = textureName;

	// All done.
	return bReturnVal;
}

bool CTextureFromBMP::ClearBMP( void )
{
	delete [] this->m_p_theImages;
	this->m_p_theImages = 0;
	return true;
}

// Some of this taken from: http://www.allegro.cc/forums/thread/601551
bool CTextureFromBMP::CreateNewBMPFromCurrentTexture( int mipMapLevel )
{
	GLint textureWidth, textureHeight;

	glGetTexLevelParameteriv(GL_TEXTURE_2D, mipMapLevel, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, mipMapLevel, GL_TEXTURE_HEIGHT, &textureHeight);

	int numberOfPixels = textureWidth * textureHeight;

	//GLubyte *buffer = (GLubyte *)malloc(textureSize);
	//GLubyte *buffer = new GLubyte[ textureSize ];
	this->m_p_theImages = new C24BitBMPpixel[ numberOfPixels ];

	//GLubyte buffer[ 5000000 ] = {0};
	//
	glGetTexImage(GL_TEXTURE_2D, mipMapLevel, GL_RGB, GL_UNSIGNED_BYTE, this->m_p_theImages );
	
	// Now fill in all the BMP stuff...
	this->m_reserved1 = this->m_reserved2 = 0;
	this->m_offsetInBits = 54;	// (always 54);
	this->m_headerSize = 40;	// (always 40);
	this->m_numberOfPlanes = 1;	// (always 1);
	this->m_bitPerPixel = 24;	// (always 24);

	this->m_compressionMode = 0;	// (0);
	this->m_imageSizeInBytes = numberOfPixels * sizeof( C24BitBMPpixel );
	this->m_PixelsPerMeterX = 2880;		// Not sure why...
	this->m_PixelsPerMeterY = 2880;		// Not sure why...
	this->m_numberOfLookUpTableEntries = 0;	// Always seem to be zero.
	this->m_numberOfImportantColours = 0;	// Always seem to be zero.

	this->m_numberOfColumns = textureHeight;
	this->m_numberOfRows = textureWidth;

	long bytesPerRow = ((3 * this->m_numberOfRows + 3) / 4) * 4;
	this->m_FileSize = this->m_numberOfRows * bytesPerRow + 54;

	return true;
}


bool CTextureFromBMP::SaveBMP( std::string fileName )
{
	std::ofstream theFile;
	theFile.open( fileName.c_str(), std::ios_base::binary );
	if ( !theFile.is_open() )
	{
		return false;
	}

	//unsigned char *img = NULL;
	//int filesize = 54 + 3*w*h


	std::cout << "Writing texture to BMP" << std::endl;
	theFile << 'B' << 'M';
	this->WriteAsUnsignedLong( this->m_FileSize, theFile );	//	theFile << this->m_FileSize;
	this->WriteAsUnsignedShort( this->m_reserved1, theFile );			// 0
	this->WriteAsUnsignedShort( this->m_reserved2, theFile );			// 0
	this->WriteAsUnsignedLong( this->m_offsetInBits, theFile );			//54
	this->WriteAsUnsignedLong( this->m_headerSize, theFile );			//40
	this->WriteAsUnsignedLong( this->m_numberOfRows, theFile );
	this->WriteAsUnsignedLong( this->m_numberOfColumns, theFile );
	this->WriteAsUnsignedShort( this->m_numberOfPlanes, theFile );		// 1
	this->WriteAsUnsignedShort( this->m_bitPerPixel, theFile );			// 24
	this->WriteAsUnsignedLong( this->m_compressionMode, theFile );
	this->WriteAsUnsignedLong( this->m_imageSizeInBytes, theFile );
	this->WriteAsUnsignedLong( this->m_PixelsPerMeterX, theFile );
	this->WriteAsUnsignedLong( this->m_PixelsPerMeterY, theFile );
	this->WriteAsUnsignedLong( this->m_numberOfLookUpTableEntries, theFile );
	this->WriteAsUnsignedLong( this->m_numberOfImportantColours, theFile );


	long bytesPerRow = ((3 * this->m_numberOfRows + 3) / 4) * 4;
	long numberOfPaddingBytes = bytesPerRow - 3 * this->m_numberOfColumns;

	// Write the bitmap data to the file... 
	long bytesWritten = 0;
	long totalBytesInFile = this->m_numberOfRows * this->m_numberOfColumns;
	long pixelCount = 0;
	
	for (unsigned long row = 0; row < this->m_numberOfRows; row++)
	{
#ifdef DEBUG_MODE_BITMAPREAD
		std::cout << "Row:" << row << std::endl;
#endif
		for (unsigned long col = 0; col < this->m_numberOfColumns; col++)
		{
			char thered   = static_cast<unsigned char>( this->m_p_theImages[pixelCount].redPixel );
			char thegreen = static_cast<unsigned char>( this->m_p_theImages[pixelCount].greenPixel );
			char theblue  = static_cast<unsigned char>( this->m_p_theImages[pixelCount].bluePixel );
			theFile.put(theblue);
			theFile.put(thegreen);
			theFile.put(thered);

			pixelCount++;
			bytesWritten += 3;	// Add three bytes
		}
#ifdef DEBUG_MODE_BITMAPREAD
		std::cout << std::endl;
#endif
		// We are at the end of the row and there will 
		//	still be bytes padding up to a multiple of 4...
		// Write thes padding bytes here...
		for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
		{
			char tempChar = 0;
			theFile.put( tempChar );
		}
	}

	theFile.close();
	return true;
}

// These are used to read the data from a file (LoadBMP2 method).
// * index is incremented appropriate amount (to next char)
// * no bounds checking for array
char CTextureFromBMP::ReadNextChar(char* data, unsigned long &index)
{
	char returnVal = static_cast<unsigned char>(data[index]);
	index++;
	return returnVal;
}

unsigned short CTextureFromBMP::ReadNextUnsignedShort(char* data, unsigned long &index)
{
	unsigned short usTheReturnVal = 0;
	usTheReturnVal = this->ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	usTheReturnVal = this->ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	return usTheReturnVal;
}

unsigned long CTextureFromBMP::ReadNextUnsignedLong(char* data, unsigned long &index)
{
	unsigned long ulTheReturnVal = 0;
	// Read four bytes, but return them in reverse order.
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 2);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 3);
	index++;
	return ulTheReturnVal;
}

// Loads it in one "go" instead of streaming it
bool CTextureFromBMP::LoadBMP2( std::string fileName )
{
	std::ifstream theFile;
	if ( this->m_bHave_cout_output )
	{
		std::cout << "Reading texture file: " << fileName;
	}
	theFile.open(fileName.c_str(), std::ios_base::binary);
	if ( !theFile )
	{
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		return false;
	}
	// Scan to the end to get the size
	theFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = theFile.tellg();
	// Return to start
	theFile.seekg(0, std::ios::beg);
		
	char* pRawData = new char[ static_cast<unsigned int>(fileSize) ];
	
	theFile.read( pRawData, fileSize );
	theFile.close();

	// Now go through and decode the BMP file.
	unsigned long curIndex = 0;
	char letter1 = this->ReadNextChar( pRawData, curIndex ); 
	char letter2 = this->ReadNextChar( pRawData, curIndex );
	if ((letter1 != 'B') && (letter2 != 'M'))
	{
		this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
		return false;
	}
	// File is BMP
	this->m_FileSize = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_reserved1 = this->ReadNextUnsignedShort(pRawData, curIndex);
	this->m_reserved2 = this->ReadNextUnsignedShort(pRawData, curIndex);
	this->m_offsetInBits = this->ReadNextUnsignedLong(pRawData, curIndex);
    this->m_headerSize = this->ReadNextUnsignedLong(pRawData, curIndex);
    this->m_numberOfRows = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_Height = this->m_OriginalHeight = this->m_numberOfRows;
    this->m_numberOfColumns = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_Width = this->m_OriginalWidth = this->m_numberOfColumns;
    this->m_numberOfPlanes = this->ReadNextUnsignedShort(pRawData, curIndex);
    this->m_bitPerPixel = this->ReadNextUnsignedShort(pRawData, curIndex);
	// Is is a 24 bit bitmap?
	if (this->m_bitPerPixel != 24)
	{
		this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
		return false;
	}

	this->m_compressionMode = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_imageSizeInBytes = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_PixelsPerMeterX = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_PixelsPerMeterY = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_numberOfLookUpTableEntries = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_numberOfImportantColours = this->ReadNextUnsignedLong(pRawData, curIndex);
				
	// 
	long bytesPerRow = ((3 * this->m_numberOfRows + 3) / 4) * 4;
	long numberOfPaddingBytes = bytesPerRow - 3 * this->m_numberOfColumns;

	// Allocate enough space...
	this->m_p_theImages = new C24BitBMPpixel[this->m_numberOfRows * this->m_numberOfColumns];
	
	// Did we run out of memory?
	if (!this->m_p_theImages)
	{
		this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
		return false;
	}


	// Read the bitmap into memory...
	unsigned long bytesRead = 0;
	unsigned long totalBytesInFile = this->m_numberOfRows * this->m_numberOfColumns;
	unsigned long pixelCount = 0;

	for (unsigned long row = 0; row < m_numberOfRows; row++)
	{
		for (unsigned long col = 0; col < m_numberOfColumns; col++)
		{
			char theblue = this->ReadNextChar(pRawData, curIndex);
			char thegreen = this->ReadNextChar(pRawData, curIndex);
			char thered = this->ReadNextChar(pRawData, curIndex);

			this->m_p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
			this->m_p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
			this->m_p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);
							//m_p_theImages[pixelCount].alphaPixel = 255;
							
#ifdef DEBUG_MODE_BITMAPREAD
			std::cout << (static_cast<unsigned int>(thered) & 0x00FF) << ", ";
			std::cout << (static_cast<unsigned int>(thegreen) & 0x00FF) << ", ";
			std::cout << (static_cast<unsigned int>(theblue) & 0x00FF) << "\t";
#endif
			pixelCount++;
			bytesRead += 3;	// Add three bytes
		}// for (unsigned long col
#ifdef DEBUG_MODE_BITMAPREAD
		std::cout << std::endl;
#endif
		// We are at the end of the row and there will 
		//	still be bytes padding up to a multiple of 4...
		// Read and discard these...
		for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
		{
			char tempChar = this->ReadNextChar(pRawData, curIndex);
		}
		// How much have we read?
		// NOTE: This really isn't percent, but it does the job of 
		//	printing out fewer dots.
		int percentRead = static_cast<int>( (static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
		if ((percentRead % 25) == 0)
		{
			if ( this->m_bHave_cout_output )
			{
				std::cout << ".";
			}
		}
	}// for (unsigned long row...
	if ( this->m_bHave_cout_output )
	{
		std::cout << "complete." << std::endl;
	}
	// BMP file is read....

	return true;
}


// When done, this->m_p_theImages contains a valid, loaded 24 bit BMP.
bool CTextureFromBMP::LoadBMP( std::string fileName )
{
	bool bReturnVal = true;

	// 
	// Read a 24 bit bitmap from a file and save it to an arrau
	std::ifstream theFile;
	// Indicate we are reading...
	if ( this->m_bHave_cout_output )
	{
		std::cout << "Reading texture file: " << fileName;
	}
	// Note: This is set to binary mode - i.e. read the chars (bytes) as 
	//	binary - set in the ios_base (i/o stream base class 'openmode' setting.
	theFile.open(fileName.c_str(), std::ios_base::binary);
	// Did it open?
	if (!theFile)
	{
		bReturnVal = false;
		this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
	}
	else
	{	// File openned. Now read it.
		char letter1, letter2;
		letter1 = ReadAByte(theFile);
		letter2 = ReadAByte(theFile);
		if ((letter1 != 'B') && (letter2 != 'M'))
		{
			bReturnVal = false;
			this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
		}
		else
		{	// File is BMP
			this->m_FileSize = ReadAnUnsignedLong(theFile);
			this->m_reserved1 = ReadAnUnsignedShort(theFile);
			this->m_reserved2 = ReadAnUnsignedShort(theFile);
			this->m_offsetInBits = ReadAnUnsignedLong(theFile);
            this->m_headerSize = ReadAnUnsignedLong(theFile);
            this->m_numberOfRows = ReadAnUnsignedLong(theFile);
			this->m_Height = m_OriginalHeight = m_numberOfRows;
            this->m_numberOfColumns = ReadAnUnsignedLong(theFile);
			this->m_Width = m_OriginalWidth = m_numberOfColumns;
            this->m_numberOfPlanes = ReadAnUnsignedShort(theFile);
            this->m_bitPerPixel = ReadAnUnsignedShort(theFile);
			// Is is a 24 bit bitmap?
			if (this->m_bitPerPixel != 24)
			{
				bReturnVal = false;
				this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
			}
			else
			{	// It's a 24 bit bitmap...
				this->m_compressionMode = ReadAnUnsignedLong(theFile);
				this->m_imageSizeInBytes = ReadAnUnsignedLong(theFile);
				this->m_PixelsPerMeterX = ReadAnUnsignedLong(theFile);
				this->m_PixelsPerMeterY = ReadAnUnsignedLong(theFile);
				this->m_numberOfLookUpTableEntries = ReadAnUnsignedLong(theFile);
				this->m_numberOfImportantColours = ReadAnUnsignedLong(theFile);
				// 
				long bytesPerRow = ((3 * m_numberOfRows + 3) / 4) * 4;
				long numberOfPaddingBytes = bytesPerRow - 3 * m_numberOfColumns;
				// Allocate enough space...
				this->m_p_theImages = new C24BitBMPpixel[m_numberOfRows * m_numberOfColumns];
				//m_p_theImages = new C32BitBMPpixel[m_numberOfRows * m_numberOfColumns];
				// Did we run out of memory?
				if (!this->m_p_theImages)
				{
					bReturnVal = false;
					m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
				}
				else
				{	// Read the bitmap into memory...
					double bytesRead = 0;
					double totalBytesInFile = m_numberOfRows * m_numberOfColumns;
					long pixelCount = 0;
					for (unsigned long row = 0; row < m_numberOfRows; row++)
					{
#ifdef DEBUG_MODE_BITMAPREAD
						std::cout << "Row:" << row << std::endl;
#endif
						for (unsigned long col = 0; col < m_numberOfColumns; col++)
						{
							char thered, thegreen, theblue;
							thered = theblue = thegreen = 0;
							theFile.get(theblue);
							theFile.get(thegreen);
							theFile.get(thered);
							this->m_p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
							this->m_p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
							this->m_p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);
							//m_p_theImages[pixelCount].alphaPixel = 255;
							
#ifdef DEBUG_MODE_BITMAPREAD
							std::cout << (static_cast<unsigned int>(thered) & 0x00FF) << ", ";
							std::cout << (static_cast<unsigned int>(thegreen) & 0x00FF) << ", ";
							std::cout << (static_cast<unsigned int>(theblue) & 0x00FF) << "\t";
#endif
							pixelCount++;
							bytesRead += 3;	// Add three bytes
						}
#ifdef DEBUG_MODE_BITMAPREAD
						std::cout << std::endl;
#endif
						// We are at the end of the row and there will 
						//	still be bytes padding up to a multiple of 4...
						// Read and discard these...
						for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
						{
							char tempChar;
							theFile >> tempChar;
						}
						// How much have we read?
						// NOTE: This really isn't percent, but it does the job of 
						//	printing out fewer dots.
						int percentRead = static_cast<int>( (static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
						if ((percentRead % 25) == 0)
						{
							if ( this->m_bHave_cout_output )
							{
								std::cout << ".";
							}
						}
					}
					if ( this->m_bHave_cout_output )
					{
						std::cout << "complete." << std::endl;
					}
					// BMP file is read....

					//// Now set the texture...
					//glBindTexture(GL_TEXTURE_2D, m_textureNumber);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT );
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT );
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
					//glTexImage2D( GL_TEXTURE_2D,		// target (2D, 3D, etc.)
					//			 0,					// MIP map level 
					//			 GL_RGBA,			// internal format
					//			 m_numberOfColumns,	// width (pixels)
					//			 m_numberOfRows,	// height (pixels)
					//			 0,					// border (0 or 1)
					//			 GL_RGB,			// format of pixel data
					//			 GL_UNSIGNED_BYTE,	// type of pixel data
					//			 m_p_theImages);	// pointer to data in memory
					//glBindTexture(GL_TEXTURE_2D, m_textureNumber);

				} // if (!m_p_theImages) // Run out of memory?
			} // if (m_bitPerPixel != 24) // Not a 24 bit bitmap
		} // if ((letter1 != 'B') && (letter2 != 'M'))	// Not a bitmap file...
		// Close the file
		theFile.close();
	} // if (!theFile)   // Did the file open?

	// All done.
	return bReturnVal;
}


////  ____                _       _   _               _____         _                  _____                    ____  __  __ ____  _____ _ _
//// / ___|_ __ ___  __ _| |_ ___| \ | | _____      _|_   _|____  _| |_ _   _ _ __ ___|  ___| __ ___  _ __ ___ | __ )|  \/  |  _ \|  ___(_) | ___
////| |   | '__/ _ \/ _` | __/ _ \  \| |/ _ \ \ /\ / / | |/ _ \ \/ / __| | | | '__/ _ \ |_ | '__/ _ \| '_ ` _ \|  _ \| |\/| | |_) | |_  | | |/ _ \
////| |___| | |  __/ (_| | ||  __/ |\  |  __/\ V  V /  | |  __/>  <| |_| |_| | | |  __/  _|| | | (_) | | | | | | |_) | |  | |  __/|  _| | | |  __/
//// \____|_|  \___|\__,_|\__\___|_| \_|\___| \_/\_/   |_|\___/_/\_\\__|\__,_|_|  \___|_|  |_|  \___/|_| |_| |_|____/|_|  |_|_|   |_|   |_|_|\___|
////
//// Returns true if able to load texture and store it
//bool CTextureFromBMP::CreateNewTextureFromBMPFile_OLD(std::string fileName, GLuint textureNumber)
//{
//	bool bReturnVal = true;
//	// 
//	// Read a 24 bit bitmap from a file and save it to an arrau
//	std::ifstream theFile;
//	// Indicate we are reading...
//	std::cout << "Reading texture file: " << fileName;
//	// Note: This is set to binary mode - i.e. read the chars (bytes) as 
//	//	binary - set in the ios_base (i/o stream base class 'openmode' setting.
//	theFile.open(fileName.c_str(), std::ios_base::binary);
//	// Did it open?
//	if (!theFile)
//	{
//		bReturnVal = false;
//		m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
//	}
//	else
//	{	// File openned. Now read it.
//		char letter1, letter2;
//		letter1 = ReadAByte(theFile);
//		letter2 = ReadAByte(theFile);
//		if ((letter1 != 'B') && (letter2 != 'M'))
//		{
//			bReturnVal = false;
//			m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
//		}
//		else
//		{	// File is BMP
//			m_FileSize = ReadAnUnsignedLong(theFile);
//			m_reserved1 = ReadAnUnsignedShort(theFile);
//			m_reserved2 = ReadAnUnsignedShort(theFile);
//			m_offsetInBits = ReadAnUnsignedLong(theFile);
//            m_headerSize = ReadAnUnsignedLong(theFile);
//            m_numberOfRows = ReadAnUnsignedLong(theFile);
//			m_Height = m_OriginalHeight = m_numberOfRows;
//            m_numberOfColumns = ReadAnUnsignedLong(theFile);
//			m_Width = m_OriginalWidth = m_numberOfColumns;
//            m_numberOfPlanes = ReadAnUnsignedShort(theFile);
//            m_bitPerPixel = ReadAnUnsignedShort(theFile);
//			// Is is a 24 bit bitmap?
//			if (m_bitPerPixel != 24)
//			{
//				bReturnVal = false;
//				m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
//			}
//			else
//			{	// It's a 24 bit bitmap...
//				m_compressionMode = ReadAnUnsignedLong(theFile);
//				m_imageSizeInBytes = ReadAnUnsignedLong(theFile);
//				m_PixelsPerMeterX = ReadAnUnsignedLong(theFile);
//				m_PixelsPerMeterY = ReadAnUnsignedLong(theFile);
//				m_numberOfLookUpTableEntries = ReadAnUnsignedLong(theFile);
//				m_numberOfImportantColours = ReadAnUnsignedLong(theFile);
//				// 
//				long bytesPerRow = ((3 * m_numberOfRows + 3) / 4) * 4;
//				//long bytesPerRow = ((4 * m_numberOfRows + 4) / 4) * 4;
//				long numberOfPaddingBytes = bytesPerRow - 3 * m_numberOfColumns;
//				//long numberOfPaddingBytes = bytesPerRow - 4 * m_numberOfColumns;
//				// Allocate enough space...
//				m_p_theImages = new C24BitBMPpixel[m_numberOfRows * m_numberOfColumns];
//				//m_p_theImages = new C32BitBMPpixel[m_numberOfRows * m_numberOfColumns];
//				// Did we run out of memory?
//				if (!m_p_theImages)
//				{
//					bReturnVal = false;
//					m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
//				}
//				else
//				{	// Read the bitmap into memory...
//					double bytesRead = 0;
//					double totalBytesInFile = m_numberOfRows * m_numberOfColumns;
//					long pixelCount = 0;
//					for (unsigned long row = 0; row < m_numberOfRows; row++)
//					{
//#ifdef DEBUG_MODE_BITMAPREAD
//						std::cout << "Row:" << row << std::endl;
//#endif
//						for (unsigned long col = 0; col < m_numberOfColumns; col++)
//						{
//							char thered, thegreen, theblue;
//							thered = theblue = thegreen = 0;
//							theFile.get(theblue);
//							theFile.get(thegreen);
//							theFile.get(thered);
//							m_p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
//							m_p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
//							m_p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);
//							//m_p_theImages[pixelCount].alphaPixel = 255;
//							
//#ifdef DEBUG_MODE_BITMAPREAD
//							std::cout << (static_cast<unsigned int>(thered) & 0x00FF) << ", ";
//							std::cout << (static_cast<unsigned int>(thegreen) & 0x00FF) << ", ";
//							std::cout << (static_cast<unsigned int>(theblue) & 0x00FF) << "\t";
//#endif
//							pixelCount++;
//							bytesRead += 3;	// Add three bytes
//						}
//#ifdef DEBUG_MODE_BITMAPREAD
//						std::cout << std::endl;
//#endif
//						// We are at the end of the row and there will 
//						//	still be bytes padding up to a multiple of 4...
//						// Read and discard these...
//						for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
//						{
//							char tempChar;
//							theFile >> tempChar;
//						}
//						// How much have we read?
//						// NOTE: This really isn't percent, but it does the job of 
//						//	printing out fewer dots.
//						int percentRead = static_cast<int>( (static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
//						if ((percentRead % 25) == 0)
//						{
//							std::cout << ".";
//						}
//					}
//					std::cout << "complete." << std::endl;
//					// BMP file is read....
//
//					// Now set the texture...
//					m_textureNumber = textureNumber;
//					glBindTexture(GL_TEXTURE_2D, m_textureNumber);
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT );
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT );
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
//					glTexImage2D(GL_TEXTURE_2D,		// target (2D, 3D, etc.)
//								 0,					// MIP map level 
//								 GL_RGB,			// internal format
//								 //GL_RGBA,			// internal format
//								 m_numberOfColumns,	// width (pixels)
//								 m_numberOfRows,	// height (pixels)
//								 0,					// border (0 or 1)
//								 GL_RGB,			// format of pixel data
//								 //GL_RGBA,			// format of pixel data
//								 GL_UNSIGNED_BYTE,	// type of pixel data
//								 m_p_theImages);	// pointer to data in memory
//					//glBindTexture(GL_TEXTURE_2D, m_textureNumber);
//
//				} // if (!m_p_theImages) // Run out of memory?
//			} // if (m_bitPerPixel != 24) // Not a 24 bit bitmap
//		} // if ((letter1 != 'B') && (letter2 != 'M'))	// Not a bitmap file...
//		// Close the file
//		theFile.close();
//	} // if (!theFile)   // Did the file open?
//
//	// All done.
//	return bReturnVal;
//}

// __  __       _       _____         _                     _        _   _
//|  \/  | __ _| | ____|_   _|____  _| |_ _   _ _ __ ___   / \   ___| |_(_)_   _____
//| |\/| |/ _` | |/ / _ \| |/ _ \ \/ / __| | | | '__/ _ \ / _ \ / __| __| \ \ / / _ \
//| |  | | (_| |   <  __/| |  __/>  <| |_| |_| | | |  __// ___ \ (__| |_| |\ V /  __/
//|_|  |_|\__,_|_|\_\___||_|\___/_/\_\\__|\__,_|_|  \___/_/   \_\___|\__|_| \_/ \___|
//
// Returns true if texture exists and can be applied
bool CTextureFromBMP::MakeTextureActive(void)
{
	bool bReturnVal = true;
	// Check if texturing is enabled?
	GLboolean bOldTextureMode = glIsEnabled(GL_TEXTURE_2D);
	// Set the texture mode...
	if (bOldTextureMode != GL_FALSE)
	{
		glEnable(GL_TEXTURE_2D);
	}
	// No implementation yet.
	glBindTexture(GL_TEXTURE_2D, m_textureNumber);

	// Restore the initial texture mode...
	if (bOldTextureMode != GL_TRUE)
	{
		glDisable(GL_TEXTURE_2D);
	}
	return bReturnVal;
}
// This allows you to set the GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE modes
//	in OpenGL as a parameter
bool CTextureFromBMP::SetTextureEnvironmentMode(GLint textureReplacementMode)
{
	bool bReturnVal = true;
	// Is this a valid replacementMode?
	if ((textureReplacementMode != GL_MODULATE) &&
		(textureReplacementMode != GL_DECAL) &&
		(textureReplacementMode != GL_BLEND) &&
		(textureReplacementMode != GL_REPLACE))
	{
		textureReplacementMode = GL_REPLACE;
	}
#ifdef DEBUG_MODE_TEXTUREREPLACEMENT
	switch (textureReplacementMode)
	{
	case GL_MODULATE:
		std::cout << "Texture replacement mode is GL_MODULATE" << std::endl;
		break;
	case GL_DECAL:
		std::cout << "Texture replacement mode is GL_DECAL" << std::endl;
		break;
	case GL_BLEND:
		std::cout << "Texture replacement mode is GL_BLEND" << std::endl;
		break;
	case GL_REPLACE:
		std::cout << "Texture replacement mode is GL_REPLACE" << std::endl;
		break;
	default:
		std::cout << "Unknown Texture repalcement mode" << std::endl;
		break;
	}
#endif
	// Set the environment mode...
	// First, get the mode...
	glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &m_oldTextureEnvironmentMode);
	// Set the new mode...
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureReplacementMode);
	return bReturnVal;
}

bool CTextureFromBMP::RestoreOldTextureEnvironmentMode(void)
{
	bool bReturnVal = true;
	// Restore the old mode...
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_oldTextureEnvironmentMode);
	return bReturnVal;
}


// ____          _         ____  _ _
//|  _ \ ___ ___(_)_______| __ )(_) |_ _ __ ___   __ _ _ __
//| |_) / _ Y __| |_  / _ \  _ \| | __| '_ ` _ \ / _` | '_ \
//|  _ <  __|__ \ |/ /  __/ |_) | | |_| | | | | | (_| | |_) |
//|_| \_\___|___/_/___\___|____/|_|\__|_| |_| |_|\__,_| .__/
//                                                    |_|
// This filters a bitmap into a new size (likely powers of 2 in width or height)
bool CTextureFromBMP::ResizeBitmap(int DesiredHeight, int DesiredWidth)
{
	bool bReturnVal = true;
	// No implementation yet.
	return bReturnVal;
}

// This is for error handling.
//
int CTextureFromBMP::GetLastErrorNumber(void)
{
	return m_lastErrorNum;
}
//
std::string CTextureFromBMP::DecodeLastError(int errorNum)
{
	std::string lastErrorString;
	switch (m_lastErrorNum)
	{
	case CTextureFromBMP::ERORR_FILE_WONT_OPEN:
		lastErrorString = "ERROR: File does not exist or would not open";
		break;
	case CTextureFromBMP::ERROR_NOT_A_BMP_FILE:
		lastErrorString = "ERROR: File is not a BMP file - \'BM\' tag not present";
		break;
	case CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP:
		lastErrorString = "ERROR: Not a 24 Bit bitmap format";
		break;
	case CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP:
		lastErrorString = "ERROR: Not enought memory to hold bitmap";
		break;
	case CTextureFromBMP::ERROR_NO_ERROR:
	default:
		lastErrorString = "No error";
		break;
	}
	return lastErrorString;
}


// These functions shift numbers by one, two, and three bytes.
unsigned long CTextureFromBMP::ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there are only 4 bytes in a long, so max is 3)?
	if (bytesToShift > 3)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
	// Now, convert the uChar to a uLong, 
	//	shift it the number of bits,
	//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theULong = theULong | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theULong;
}

unsigned short CTextureFromBMP::ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there is only 2 bytes in a long, so max is 1)?
	if (bytesToShift > 1)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
	// Now, convert the uChar to a uLong, 
	//	shift it the number of bits,
	//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theUShort = theUShort | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theUShort;
}


// These are used to read the data from a file.
unsigned long CTextureFromBMP::ReadAnUnsignedLong(std::ifstream& theStream)
{
	unsigned long ulTheReturnVal = 0;
	// Read four bytes, but return them in reverse order.
	char TheByte = 0;
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 0);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 1);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 2);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 3);
	return ulTheReturnVal;
}





void CTextureFromBMP::WriteAsUnsignedShort( unsigned short value, std::ofstream& theStream )
{
	// AB --> BA
	char TheByte[2] = {0};
	TheByte[1] = static_cast<char>(( value & 0xFF00 ) >> 8 );
	TheByte[0] = static_cast<char>( value & 0x00FF );

	theStream.put( TheByte[0] );
	theStream.put( TheByte[1] );
}

void CTextureFromBMP::WriteAsUnsignedLong( unsigned long value, std::ofstream& theStream )
{
	// ABCD --> DCBA
	char TheByte[4] = {0};
	TheByte[3] = static_cast<char>(( value & 0xFF000000 ) >> 24 );
	TheByte[2] = static_cast<char>(( value & 0x00FF0000 ) >> 16 );
	TheByte[1] = static_cast<char>(( value & 0x0000FF00 ) >> 8 );
	TheByte[0] = static_cast<char>( value & 0x000000FF );

	theStream.put( TheByte[0] );
	theStream.put( TheByte[1] );
	theStream.put( TheByte[2] );
	theStream.put( TheByte[3] );
}

unsigned short CTextureFromBMP::ReadAnUnsignedShort(std::ifstream& theStream)
{
	unsigned short usTheReturnVal = 0;
	// Read two bytes, but reverse the order.
	char TheByte;
	theStream.get(TheByte);
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 0);
	theStream.get(TheByte);
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 1);
	return usTheReturnVal;
}

char CTextureFromBMP::ReadAByte(std::ifstream& theStream)
{
	char theByte = 0;
	theStream.get(theByte);
	return theByte;
}

// ____  _           _            _____         _                     _        ____           _            ____  _ _
//|  _ \(_)___ _ __ | | __ _ _   |_   _|____  _| |_ _   _ _ __ ___   / \   ___|  _ \ __ _ ___| |_ ___ _ __| __ )(_) |_ _ __ ___   __ _ _ __
//| | | | / __| '_ \| |/ _` | | | || |/ _ \ \/ / __| | | | '__/ _ \ / _ \ / __| |_) / _` / __| __/ _ \ '__|  _ \| | __| '_ ` _ \ / _` | '_ \
//| |_| | \__ \ |_) | | (_| | |_| || |  __/>  <| |_| |_| | | |  __// ___ \\__ \  _ < (_| \__ \ ||  __/ |  | |_) | | |_| | | | | | (_| | |_) |
//|____/|_|___/ .__/|_|\__,_|\__, ||_|\___/_/\_\\__|\__,_|_|  \___/_/   \_\___/_| \_\__,_|___/\__\___|_|  |____/|_|\__|_| |_| |_|\__,_| .__/
//            |_|            |___/                                                                                                    |_|
// This takes the bitmap that is read and displays it on the screen
//	as a raster image. i.e. it doesn't go through the transformations, 
//	etc., it just plops it on the screen at the current raster 
//	location. This is like bitmap fonts.
void CTextureFromBMP::DisplayTextureAsRasterBitmap(GLfloat xOrigin, GLfloat yOrigin, GLfloat xMove, GLfloat yMove)
{
	// convert the array of structures into a bitmap array
	GLubyte* p_theBitmap = new GLubyte[m_Width * m_Height * 4];	// 4 bytes / pixel
	unsigned long arrayIndex = 0;
#ifdef DEBUG_MODE_BITMAPDISPLAY
	static bool bDisplayedOnce = false;
#endif
	// copy the data into array...
	long pixeloffset = 0;
	for (unsigned long row = 0; row < m_Width; row++)
	{
#ifdef DEBUG_MODE_BITMAPDISPLAY
		if (!bDisplayedOnce)
		{
			std::cout << "Row = " << row << std::endl;
		}
#endif
		for (unsigned long column = 0; column < m_Height; column++)
		{
			arrayIndex = (row * m_Width) + column;
			double theRow = row;
			double theCol = column;
			p_theBitmap[arrayIndex] = m_p_theImages[pixeloffset].redPixel;
			p_theBitmap[arrayIndex + 1] = m_p_theImages[pixeloffset].greenPixel;
			p_theBitmap[arrayIndex + 2] = m_p_theImages[pixeloffset].bluePixel;
			p_theBitmap[arrayIndex + 3] = 0;
			pixeloffset++;

#ifdef DEBUG_MODE_BITMAPDISPLAY
			if (!bDisplayedOnce)
			{
				std::cout << "(R:" << static_cast<unsigned int>(p_theBitmap[arrayIndex]) << ",";
				std::cout << "G:" << static_cast<unsigned int>(p_theBitmap[arrayIndex + 1]) << ",";
				std::cout << "B:" << static_cast<unsigned int>(p_theBitmap[arrayIndex + 2]) << ",";
				std::cout << "A:" << static_cast<unsigned int>(p_theBitmap[arrayIndex + 3]);
				std::cout << ")" << std::endl;
			}
#endif
			arrayIndex++;
		}
	}
#ifdef DEBUG_MODE_BITMAPDISPLAY
	if (!bDisplayedOnce) 
	{
		bDisplayedOnce = true;
	}
#endif

	// Draw the image using the glDrawPixels instead of bitmap...
	glRasterPos2i(0,0);
	glPixelZoom(10, 10);
	glDrawPixels(m_Width, m_Height, GL_BGRA, GL_UNSIGNED_BYTE, p_theBitmap);

	//glBitmap(m_Width, m_Height, xOrigin, yOrigin, xMove, yMove, p_theBitmap);
	delete [] p_theBitmap;
	return;
}

bool CTextureFromBMP::bWasThereAnOpenGLError(void)
{
	GLenum errorEnum = GL_NO_ERROR;
	std::string errorString;
	std::string errorDetails;
	return this->bWasThereAnOpenGLError( errorEnum, errorString, errorDetails );
}

bool CTextureFromBMP::bWasThereAnOpenGLError(GLenum &errorEnum)
{
	std::string errorString;
	std::string errorDetails;
	return this->bWasThereAnOpenGLError( errorEnum, errorString, errorDetails );
}

bool CTextureFromBMP::bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorString)
{
	std::string errorDetails;
	return this->bWasThereAnOpenGLError( errorEnum, errorString, errorDetails );
}

// Reutn false if everything is OK
bool CTextureFromBMP::bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorString, std::string &errorDetails )
{
	errorEnum = glGetError();
	// from: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
	switch ( errorEnum )
	{
	case GL_NO_ERROR:
		errorString = "GL_NO_ERROR";
		errorDetails = "GL_NO_ERROR: No error has been recorded.";
		/******************/
		return false;
		/******************/
		break;
	case GL_INVALID_ENUM:
		errorString = "GL_INVALID_ENUM"; 
		errorDetails = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
		break;
	case GL_INVALID_VALUE:
		errorString = "GL_INVALID_VALUE"; 
		errorDetails = "GL_INVALID_VALUE: A numeric argument is out of range.";
		break;
	case GL_INVALID_OPERATION:
		errorString = "GL_INVALID_OPERATION"; 
		errorDetails = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorString = "GL_INVALID_FRAMEBUFFER_OPERATION"; 
		errorDetails = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
		break;
	case GL_OUT_OF_MEMORY:
		errorString = "GL_OUT_OF_MEMORY"; 
		errorDetails = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
		break;
	case GL_STACK_UNDERFLOW:
		errorString = "GL_STACK_UNDERFLOW"; 
		errorDetails = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
		break;
	case GL_STACK_OVERFLOW:
		errorString = "GL_STACK_OVERFLOW"; 
		errorDetails = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
		break;
	default:
		errorString = "Unknown error";
		errorDetails = "Unknown error";
		break;
	};

	return true;	// There -WAS- an error
}

//GLenum CTextureFromBMP::getTextureUnit(void)
//{
//	return this->m_textureUnit;
//}

bool CTextureFromBMP::getIsCubeMap(void)
{
	return this->m_bIsCubeMap;
}

bool CTextureFromBMP::getIs2DTexture(void)
{
	return this->m_bIs2DTexture;
}

// The actual image information
//C24BitBMPpixel m_p_theImages[];	
unsigned long CTextureFromBMP::GetHeight(void)
{
	return m_Height;
}

unsigned long CTextureFromBMP::GetWidth(void)
{
	return m_Width;
}

unsigned long CTextureFromBMP::GetOriginalHeight(void)
{
	return m_OriginalHeight;
}

unsigned long CTextureFromBMP::GetOriginalWidth(void)
{
	return m_OriginalWidth;
}

unsigned long CTextureFromBMP::GetFileSize(void)
{
	return m_FileSize;
}

unsigned long CTextureFromBMP::GetFileSizeInBytes(void)
{
	return m_FileSize;
}

unsigned short CTextureFromBMP::GetReserved1(void)
{
	return m_reserved1;
}

unsigned short CTextureFromBMP::GetReserved2(void)
{
	return m_reserved2;
}

unsigned long CTextureFromBMP::GetImageSizeInBytes(void)
{
	return m_imageSizeInBytes;
}

unsigned long CTextureFromBMP::GetBitsPerPixel(void)
{
	return m_bitPerPixel;
}

unsigned long CTextureFromBMP::GetOffsetInBits(void)
{
	return m_offsetInBits;
}

unsigned long CTextureFromBMP::GetCompressionMode(void)
{
	return m_compressionMode;
}

unsigned long CTextureFromBMP::GetPixelsPerMeterX(void)
{
	return m_PixelsPerMeterX;
}

unsigned long CTextureFromBMP::GetPixelsPerMeterY(void)
{
	return m_PixelsPerMeterY;
}

unsigned long CTextureFromBMP::GetNumberOfLookUpTables(void)
{
	return m_numberOfLookUpTableEntries;
}

unsigned long CTextureFromBMP::GetNumberOfImportantColours(void)
{
	return m_numberOfImportantColours;
}



