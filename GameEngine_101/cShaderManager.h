#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_

#include <string>
#include <vector>
#include <map>

// Manages all shaders related tuff
class cShaderManager
{
public:

	// Keeps information about shader sources
	class cShader  {
	public:
		cShader();
		~cShader();
		enum eShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN
		};
		eShaderType shaderType;
		std::string getShaderTypeString(void);

		// ID is a "name" from OpenGL
		unsigned int ID;
		std::vector<std::string> vecSource;
		bool bSourceIsMultiLine;
		std::string fileName;
	};

	// Shader Programs
	class cShaderProgram {
	public:
		cShaderProgram() : ID(0) {};
		~cShaderProgram() {};
		// ID is a "name" from OpenGL
		unsigned int ID;
		std::string friendlyName;
	};

	cShaderManager();
	~cShaderManager();

	bool useShaderProgram( unsigned int ID );
	bool useShaderProgram( std::string friendlyName );

	// Create a shader program from cShader object (source)
	bool createProgramFromFile( std::string friendlyName, cShader &vertexShad, cShader &fragShader );
	
	void setBasePath( std::string basepath );
	unsigned int getIDFromFriendlyName( std::string friendlyName );

	std::string getLastError(void);

private:
	
	// Reads the source information from a file into cShader::vecSource
	// Returns an empty string if it didn't work
	bool m_loadSourceFromFile( cShader &shader );
	
	std::string m_basepath;

	// Compile shader from Cshader object (source)
	bool m_compileShaderFromSource( cShader &shader, std::string &error );
	
	bool m_wasThereACompileError( unsigned int shaderID, std::string &errorText );
	bool m_wasThereALinkError( unsigned int progID, std::string &errorText );

	std::string m_lastError;

	std::map< unsigned int /*ID*/, cShaderProgram > m_ID_to_Shader;
	std::map< std::string /*name*/, unsigned int /*ID*/ > m_name_to_ID;
};

#endif