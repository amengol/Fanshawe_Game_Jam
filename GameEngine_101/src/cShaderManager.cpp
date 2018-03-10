#include "cShaderManager.h"
#include "globalOpenGL_GLFW.h"
#include <fstream>
#include <sstream>		
#include <vector>
#include <algorithm>	
#include <iterator>		
#include "Texture/CTextureManager.h"
#include "sTextureBindBlendInfo.h"

cShaderManager::cShaderManager()
{
	return;
}

cShaderManager::~cShaderManager()
{
	return;
}


bool cShaderManager::useShaderProgram( unsigned int ID )
{
	// Use the number directy... 
	// TODO: Might do a lookup to see if we really have that ID...
	glUseProgram(ID);
	return true;
}

bool cShaderManager::useShaderProgram( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	// Didn't find it
		// Maybe set glUseProgram(0)....?
		return false;
	}
	glUseProgram(itShad->second);

	return true;
}

unsigned int cShaderManager::getIDFromFriendlyName( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	// Didn't find it
		return 0;
	}
	return itShad->second;
}

// Returns NULL if not found
cShaderManager::cShaderProgram* cShaderManager::getShaderProgPointerFromID(unsigned int shaderID)
{
	std::map< unsigned int /*ID*/, cShaderProgram >::iterator
			itShad = this->m_ID_to_Shader.find(shaderID);

	if ( itShad == this->m_ID_to_Shader.end() )
	{	// Didn't find it
		return 0;
	}
	return &(itShad->second);
}

cShaderManager::cShaderProgram* cShaderManager::getShaderProgPointerFromFriendlyName( std::string friendlyName )
{
	unsigned int shaderID = this->getIDFromFriendlyName( friendlyName );
	return this->getShaderProgPointerFromID( shaderID );
}


const unsigned int MAXLINELENGTH = 65536;		// 16x1024

void cShaderManager::setBasePath( std::string basepath )
{
	this->m_basepath = basepath;
	return;
}


// Returns bool if didn't load
bool cShaderManager::m_loadSourceFromFile( cShader &shader )
{
	std::string fullFileName = this->m_basepath + shader.fileName;

	std::ifstream theFile( fullFileName.c_str() );
	if ( ! theFile.is_open() )
	{
		return false;
	}

	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = {0};
	while ( theFile.getline( pLineTemp, MAXLINELENGTH ) )
	{
		std::string tempString(pLineTemp);
		//if ( tempString != "" )
		//{	// Line isn't empty, so add
			shader.vecSource.push_back( tempString );
		//}
	}
	
	theFile.close();
	return true;		// Return the string (from the sstream)
}

// Returns empty string if no error
// returns false if no error
bool cShaderManager::m_wasThereACompileError( unsigned int shaderID, 
											  std::string &errorText )
{
	errorText = "";	// No error

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;	// Oops

		return true;	// There WAS an error
	}
	return false; // There WASN'T an error
}

bool cShaderManager::m_wasThereALinkError( unsigned int programID, std::string &errorText )
{
	errorText = "";	// No error

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if(wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;	

		// There WAS an error
		return true;	
	}
	
	// There WASN'T an error
	return false; 
}


std::string cShaderManager::getLastError(void)
{
	std::string lastErrorTemp = this->m_lastError;
	this->m_lastError = "";
	return lastErrorTemp;
}

#include <iostream>

bool cShaderManager::m_compileShaderFromSource( cShaderManager::cShader &shader, std::string &error )
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char*[numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset( arraySource, 0, numberOfLines );	

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset( arraySource[indexLine], 0, numCharacters + 2 );

		// Copy line of source into array
		for ( unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++ )
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}//for ( unsigned int indexChar = 0...
		
		// At a '\0' at end (just in case)
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

	}//for ( unsigned int indexLine = 0...

//******************************************************************************************
//	// Print it out (for debug)
//	std::cout << "Source (from char** array):" << std::endl;
//	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
//	{
//		std::cout << "(" << indexLine << "):\t" << arraySource[indexLine] << std::endl;
//	}
//	std::cout << "END" << std::endl;
//******************************************************************************************

	//const char* tempVertChar = "Hey there;";

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
    glCompileShader(shader.ID); 

	// Get rid of the temp source "c" style array
	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{	// Delete this line
		delete [] arraySource[indexLine];
	}
	// And delete the original char** array
	delete [] arraySource;

	// Did it work? 
	std::string errorText = "";
	if ( this->m_wasThereACompileError( shader.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << shader.getShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}

	return true;
}

bool cShaderManager::createProgramFromSource( 
			std::string friendlyName, 
			cShader &vertexShad,
			cShader &fragShader )
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...

    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	cShaderProgram curProgram;
    curProgram.ID = glCreateProgram();

    glAttachShader(curProgram.ID, vertexShad.ID);
    glAttachShader(curProgram.ID, fragShader.ID);
    glLinkProgram(curProgram.ID);

	// Was there a link error? 
	errorText = "";
	if ( this->m_wasThereALinkError( curProgram.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}


bool cShaderManager::createProgramFromFile( 
	        std::string friendlyName,
			cShader &vertexShad, 
			cShader &fragShader )
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	//  char* vertex_shader_text = "wewherlkherlkh";
	// Load some text from a file...
	if ( ! this->m_loadSourceFromFile( vertexShad ) )
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	errorText = "";
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...



    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if ( ! this->m_loadSourceFromFile( fragShader ) )
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	cShaderProgram curProgram;
    curProgram.ID = glCreateProgram();

    glAttachShader(curProgram.ID, vertexShad.ID);
    glAttachShader(curProgram.ID, fragShader.ID);
    glLinkProgram(curProgram.ID);

	// Was there a link error? 
	errorText = "";
	if ( this->m_wasThereALinkError( curProgram.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}
	curProgram.loadUniforms();

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}

bool cShaderManager::createProgramFromFile( 
	        std::string friendlyName,
			cShader &vertexShad, 
	        cShader &geomShad,				// **NEW!!**// 
			cShader &fragShader )
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	//  char* vertex_shader_text = "wewherlkherlkh";
	// Load some text from a file...
	if ( ! this->m_loadSourceFromFile( vertexShad ) )
	{
		this->m_lastError = "Can't load vertex shader file";
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	errorText = "";
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	//*******************************************************
	geomShad.ID = glCreateShader(GL_GEOMETRY_SHADER);
	geomShad.shaderType = cShader::GEOMETRY_SHADER;
	if ( ! this->m_loadSourceFromFile(geomShad) )
	{
		this->m_lastError = "Can't load geometry shader from file";
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	if ( ! this->m_compileShaderFromSource(geomShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...
	//*******************************************************

    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if ( ! this->m_loadSourceFromFile( fragShader ) )
	{
		this->m_lastError = "Can't load fragment shader from file";
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	cShaderProgram curProgram;
    curProgram.ID = glCreateProgram();

    glAttachShader(curProgram.ID, vertexShad.ID);
	// *******************************************************
    glAttachShader(curProgram.ID, geomShad.ID);			// **NEW**
	// *******************************************************
    glAttachShader(curProgram.ID, fragShader.ID);
    glLinkProgram(curProgram.ID);

	// Was there a link error? 
	errorText = "";
	if ( this->m_wasThereALinkError( curProgram.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}
	curProgram.loadUniforms();

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}

//	     ___  _              _            __  __                                      
//	 __ / __|| |_   __ _  __| | ___  _ _ |  \/  | __ _  _ _   __ _  __ _  ___  _ _    
//	/ _|\__ \| ' \ / _` |/ _` |/ -_)| '_|| |\/| |/ _` || ' \ / _` |/ _` |/ -_)| '_|   
//	\__||___/|_||_|\__,_|\__,_|\___||_|  |_|  |_|\__,_||_||_|\__,_|\__, |\___||_|     
//	(_)(_) __ / __|| |_   __ _  __| | ___  _ _ | _ \ _ _  ___  __ _|___/  __ _  _ __  
//	 _  _ / _|\__ \| ' \ / _` |/ _` |/ -_)| '_||  _/| '_|/ _ \/ _` || '_|/ _` || '  \ 
//	(_)(_)\__||___/|_||_|\__,_|\__,_|\___||_|  |_|  |_|  \___/\__, ||_|  \__,_||_|_|_|
//                                                          |___/                   
cShaderManager::cShaderProgram::cShaderProgram()
{
	this->ID = 0;	// Invalid shader program ID
//	this->loadUniforms();
	return;
}

cShaderManager::cShaderProgram::~cShaderProgram()
{
	return;
}

void cShaderManager::cShaderProgram::loadUniforms(void)
{
	// HACK: We are manually loading the texture units here!
	// TODO: Use getActiveUniforms(), etc. to load them correctly
	{
		sShaderTextureState tex00;
		tex00.sampler_LocID = glGetUniformLocation(this->ID, "tex2D00");
		tex00.blend_LocID = glGetUniformLocation(this->ID, "texBlend00");
		this->vec2DTexBindings.push_back(tex00);
	}
	{
		sShaderTextureState tex01;
		tex01.sampler_LocID = glGetUniformLocation(this->ID, "tex2D01");
		tex01.blend_LocID = glGetUniformLocation(this->ID, "texBlend01");
		this->vec2DTexBindings.push_back(tex01);
	}
	{
		sShaderTextureState tex02;
		tex02.sampler_LocID = glGetUniformLocation(this->ID, "tex2D02");
		tex02.blend_LocID = glGetUniformLocation(this->ID, "texBlend02");
		this->vec2DTexBindings.push_back(tex02);
	}
	{
		sShaderTextureState tex03;
		tex03.sampler_LocID = glGetUniformLocation(this->ID, "tex2D03");
		tex03.blend_LocID = glGetUniformLocation(this->ID, "texBlend03");
		this->vec2DTexBindings.push_back(tex03);
	}

	return;
}




// Searches the shader texture list, looking for a match. 
// If found, assigns the blend function and returns true.
// if not found, returns false.
bool cShaderManager::cShaderProgram::m_FindAndAssign2DTexture( sTextureBindBlendInfo *pTexBlendInfo )
{
	unsigned int bindIndex = 0;
	unsigned int numberOfTextureSlots = (unsigned int)this->vec2DTexBindings.size();
	for ( ; bindIndex != numberOfTextureSlots; bindIndex++ )
	{
		sShaderTextureState& textureSlot = this->vec2DTexBindings[bindIndex];
		if ( textureSlot.texID == pTexBlendInfo->texLookupCachedID )
		{
			// Indicate it's in use and copy the blend value
			textureSlot.bIsInUseThisPass = true;
			textureSlot.blendValue = pTexBlendInfo->blendRatio;
			// NOTE: If this is already present, it is ALREADY 
			//	bound inside the shader, so we don't have to do that.
			return true;
		}
	}
	return false;
}

bool cShaderManager::cShaderProgram::m_Assign2DTextureToFirstEmptySlot( sTextureBindBlendInfo *pTexBlendInfo )
{
	unsigned int bindIndex = 0;
	unsigned int numberOfTextureSlots = (unsigned int)this->vec2DTexBindings.size();
	for ( ; bindIndex != numberOfTextureSlots; bindIndex++ )
	{
		sShaderTextureState& textureSlot = this->vec2DTexBindings[bindIndex];
		if ( ! textureSlot.bIsInUseThisPass )
		{
			// Copy the settings from wanted texture and indicate it's in use
			textureSlot.texID = pTexBlendInfo->texLookupCachedID;	// Copy texture ID (name of texture)
			textureSlot.bIsInUseThisPass = true;					// Indicate this is in use
			textureSlot.blendValue = pTexBlendInfo->blendRatio;		// Copy the texture blend ratio
			textureSlot.name = pTexBlendInfo->name;

			// Slot IS available, so bind
			glActiveTexture(GL_TEXTURE0 + bindIndex);
			textureSlot.texUnit = bindIndex;
			// Bind texture unit to texture...
			glBindTexture( GL_TEXTURE_2D, textureSlot.texID );
			// Set sampler to texture unit
			glUniform1f( textureSlot.sampler_LocID, (float)bindIndex );
			// Set the blend info
			glUniform1f( textureSlot.sampler_LocID, textureSlot.blendValue );
			return true;
		}
	}
	// If we got here, then there are no empty slots left
	return false;
}


// Here's the steps:
// 1. Make a copy of the OBJECT's wanted texture list
//    (NOTE: this copy is a vector of POINTERS to the original; 
//     any items that point to something still need to be matched, 
//     otherwise (if NULL), they ARE matched up. At the start, 
//     all the items point to something, though)
// 2. Set all the SHADER texture binding to "not in use this pass"
//     - This will indicate that they can be "freed up"
//     - Set the blend value to 0.0, so if there are some still 
//       bound, then will blend into black (not impact the result)
// 3. For each of the OBJECT's wanted textures, look through the
//     SHADER's ALREADY bound textures to see if there is a match.
//     - if there IS, then copy the blending value, indicate
//       that the SHADER binding IS in use, and set the copy of 
//       OBJECT pointers to NULL (meaning they ARE matched)
//     - if there ISN't a match, do nothing (leaving them on the 
//       OBJECT list. 
// 4. For each of the OBJECT's wanted textures that are remaining 
//     in the list (i.e. aren't NULL):
//     - Seach for an "empty" slot in the SHADER (i.e. a slot that's 
//       still got bIsInUseThisPass = false. 
//       - if one is found: 
//         - set bIsInUseThisPass = true
//         - Bind the texture to a texture unit (based on the slot index)
//         - Set the sampler to the same texture unit (based on the slot index)
//         - Copy the blend ratio to the sampler
//         - Set the OBJECT's wanted texture to NULL (indicating it's assigned)
//        - if you DIDN'T find a free slot, then there aren't enough 
//           texture samplers in the sahder. 
// 
void cShaderManager::cShaderProgram::set2DTextureBindings( std::vector<sTextureBindBlendInfo> &vec2DBindings, CTextureManager* pTextureManager )
{
	// Update the objects texture IDs if they are invalid
	// NOTE: This should only have to happen once
	for ( std::vector<sTextureBindBlendInfo>::iterator itTex = vec2DBindings.begin(); 
		  itTex != vec2DBindings.end(); itTex++ )
	{
		if ( itTex->texLookupCachedID == sTextureBindBlendInfo::INVALID_TEXTURE_ID )
		{
//			itTex->texLookupCachedID = pTextureManager->getTextureIDFromName( itTex->name );
			itTex->texLookupCachedID = pTextureManager->getTextureIDFromTextureName( itTex->name );
		}
	}


	// Make a copy of the OBJECT's texture list
	std::vector<sTextureBindBlendInfo*> vecTemp_p2DNeededTextures;
	// We are pushing back the POINTER to the reqested texture
	// This way, we can see if the texture has already been bound:
	// - points to something = NOT bound
	// - NULL - IS bound
	for ( std::vector<sTextureBindBlendInfo>::iterator itTN = vec2DBindings.begin(); itTN != vec2DBindings.end(); itTN++ )
	{
		vecTemp_p2DNeededTextures.push_back( &(*itTN) );
	}

	// Clear the shader's IN USE flag AND set blending to 0.0f (assume they AREN'T in use)
	
	unsigned int numberOfShaderTexSlots = (unsigned int)this->vec2DTexBindings.size();
	for ( unsigned int shaderTexSlotIndex = 0; shaderTexSlotIndex != numberOfShaderTexSlots; shaderTexSlotIndex++ )
	{
		sShaderTextureState& shaderTexSlot = this->vec2DTexBindings[shaderTexSlotIndex];
		shaderTexSlot.bIsInUseThisPass = false;
		shaderTexSlot.blendValue = 0.0f;		
	}

	// From the OBJECT's list, search for already bound textures IN THE SHADER
	// - If found, set blend and DELETE-CLEAR from OBJECT's list
	// - If NOT found, leave
	unsigned int numObjectTexturesWanted = (unsigned int)vecTemp_p2DNeededTextures.size();
	for ( unsigned int wantedTextureIndex = 0; wantedTextureIndex != numObjectTexturesWanted; wantedTextureIndex++ )
	{
		sTextureBindBlendInfo* pWantedTexture = vecTemp_p2DNeededTextures[wantedTextureIndex];

		if ( this->m_FindAndAssign2DTexture( pWantedTexture ) )
		{	// Texture was matched, to remove the pointer from the vector
			pWantedTexture = NULL;
		}
	}

	// At this point:
	//	- what's in the vecTemp_p2DNeededTextures still need to be bound
	//  - anything left in the this->vec2DTexBindings that has an bIsInUseThisPass = false is an available slot

	// Now go through the REMAINING OBJECT's list
	// - look for NOT used SHADER textures (search)
	//   - Set values from OBJECT's texture info
	//   - if NONE remaining, then there aren't enough slots
	// - Set values from Object
	for ( unsigned int wantedTextureIndex = 0; wantedTextureIndex != numObjectTexturesWanted; wantedTextureIndex++ )
	{
		sTextureBindBlendInfo* pWantedTexture = vecTemp_p2DNeededTextures[wantedTextureIndex];
		if ( pWantedTexture != NULL )
		{	// Search for this texture
			if ( this->m_Assign2DTextureToFirstEmptySlot( pWantedTexture ) )
			{
				// Set the objects wanted texture to NULL, indicating it's found
				pWantedTexture = NULL;
			}
		}
	}

	// At this point, it there's any items in the vecTemp_p2DNeededTextures
	//	that DON'T point to NULL, then they haven't been assigned 
	// (not enough shader samplers in the shader)


	return;
}


