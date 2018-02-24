#include "cShaderManager.h"

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader() 
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	return "UNKNOWN_SHADER_TYPE";
}

void cShaderManager::cShader::parseStringIntoMultiLine(std::string singleLineSouce)
{

    this->vecSource.clear();

    unsigned int stringsizeX4 = (unsigned int)singleLineSouce.size() * 4;
    char* charSourceString = new char[stringsizeX4];
    strcpy(charSourceString, singleLineSouce.c_str());

    char * pCharLine;
    pCharLine = strtok(charSourceString, "\n");
    while (pCharLine != NULL)
    {	// Push back this line
        this->vecSource.push_back(std::string(pCharLine));
        pCharLine = strtok(NULL, "\n");
    }
    delete[] charSourceString;
    return;
}