#include "cAssimpAssetLoader.h"

#include <fstream>
#include <iostream>

// assimp include files.
#include "assimp/postprocess.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

cAssimAssetLoader::cAssimAssetLoader()
{
    this->createAILogger();
    this->scene = NULL;
}

cAssimAssetLoader::~cAssimAssetLoader()
{
}

void cAssimAssetLoader::logInfo(std::string logString)
{
    // Will add message to File with "info" Tag
    Assimp::DefaultLogger::get()->info(logString.c_str());
}

bool cAssimAssetLoader::Import3DFromFile(const std::string & pFile)
{
    // Check if file exists
    std::ifstream fin(pFile.c_str());
    if (!fin.fail())
    {
        fin.close();
    }
    else
    {
        std::cout << "Couldn't open file: " + pFile;
        logInfo(importer.GetErrorString());
        return false;
    }

    scene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if (!scene)
    {
        logInfo(importer.GetErrorString());
        return false;
    }

    // Now we can access the file's contents.
    logInfo("Import of scene " + pFile + " succeeded.");

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

const aiScene* cAssimAssetLoader::getScene()
{
    return this->scene;
}

void cAssimAssetLoader::createAILogger()
{
    // Change this line to normal if not wanting to analyse the import process
    //Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
    Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

    // Create a logger instance for Console Output
    Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);

    // Create a logger instance for File Output (found in project folder or near .exe)
    Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);

    // Now I am ready for logging my stuff
    Assimp::DefaultLogger::get()->info("this is my info-call");
}
