#include "cShaderManager.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

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
	glUseProgram(ID);
	return true;
}

bool cShaderManager::useShaderProgram( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	
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
	{
		return 0;
	}
	return itShad->second;
}

const unsigned int MAXLINELENGTH = 65536;

void cShaderManager::setBasePath( std::string basepath )
{
	this->m_basepath = basepath;
	return;
}

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
		shader.vecSource.push_back( tempString );		
	}
	
	theFile.close();
	return true;
}

// Returns empty string if no error too
bool cShaderManager::m_wasThereACompileError( unsigned int shaderID, std::string &errorText )
{
	errorText = "";	// No error

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);		
		
		errorText.append(pLogText);				
		
		// Put the error also in the m_lastError
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );

		delete [] pLogText;
		// There WAS an error
		return true;	
	}
	// There WASN'T an error
	return false; 
}

// Returns empty string if no error too
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
		
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		
		errorText.append(pLogText);

		// Put the error also in the m_lastError
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

bool cShaderManager::m_compileShaderFromSource( cShaderManager::cShader &shader, std::string &error )
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	char** arraySource = new char*[numberOfLines];
	
	// Clear array to all zeros
	memset( arraySource, 0, numberOfLines );	

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{
		// Number of characters in a vecSource array position
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];	// For the '\n' and '\0' at end
		// Clears the created array of chars
		memset( arraySource[indexLine], 0, numCharacters + 2 );

		// Copy line of source into array
		for ( unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++ )
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}
		
		// Add a '\n' and a '\0' at end of each line
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

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
    glCompileShader(shader.ID); 

	// Get rid of the temp source "c" style array
	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{	
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

		// There WAS an error
		return false;
	}
	// There WASN'T an error
	return true;
}

bool cShaderManager::createProgramFromFile( std::string friendlyName, cShader &vertexShad, cShader &fragShader )
{
	std::string errorText = "";

	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	
	if ( ! this->m_loadSourceFromFile( vertexShad ) )
	{
		return false;
	}

	errorText = "";
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}

    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;

	if ( ! this->m_loadSourceFromFile( fragShader ) )
	{
		return false;
	}

	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}


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

		// There WAS a link error
		return false;
	}

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	// There WASN'T an error
	return true;
}

bool cShaderManager::createProgramFromSource(
    std::string friendlyName,
    cShader &vertexShad,
    cShader &fragShader)
{
    std::string errorText = "";


    // Shader loading happening before vertex buffer array
    vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
    vertexShad.shaderType = cShader::VERTEX_SHADER;
    if (!this->m_compileShaderFromSource(vertexShad, errorText))
    {
        this->m_lastError = errorText;
        return false;
    }//if ( this->m_compileShaderFromSource(...

    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
    fragShader.shaderType = cShader::FRAGMENT_SHADER;
    if (!this->m_compileShaderFromSource(fragShader, errorText))
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
    if (this->m_wasThereALinkError(curProgram.ID, errorText))
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
