#include "Utilities.h"
#include <cstdlib>	// rand() and RAND_MAX
#include <time.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <string>
#include <fstream>
#include <sstream>

bool InitialConfig(const std::string& fileName, const std::string& titleEnd, int& width, int& height, std::string& title)
{
    // Sanity check
    if (width < 0 || height < 0)
    {
        return false;
    }

    std::string theTitle = title;

    std::ifstream infoFile(fileName);

    if (!infoFile.is_open())
    {    // File didn't open...
        return false;
    }
    
    // File DID open, so read it... 
    std::string a;

    infoFile >> a;    // "Game"
    infoFile >> a;    // "Config"
    infoFile >> a;    // "width"

    infoFile >> width;

    infoFile >> a;    // "height"

    infoFile >> height;

    infoFile >> a;    // Title_Start

    std::stringstream ssTitle;
    bool bKeepReading = true;
    do
    {
        infoFile >> a;
        if (a != titleEnd)
        {
            ssTitle << a << " ";
        }
        else
        {    // it IS the end! 
            bKeepReading = false;
            title = ssTitle.str();
        }
    } while (bKeepReading);

    return true;
}

void CreateRamdomGameObjects(const int numOfObjects,
                             std::vector<cGameObject*>& theVecGo,
                             std::vector<GameObjectsInfo> info,
                             const float minX,
                             const float maxX,
                             const float minY,
                             const float maxY,
                             const float minZ,
                             const float maxZ)
{
    srand(time(nullptr));

    for(int i = 0; i < numOfObjects; i++)
    {
        cGameObject* gameObject = new cGameObject();

        // Pic one object

        const int objIndex = std::rand() % info.size();

        gameObject->meshName = info[objIndex].meshName;
        gameObject->textureBlend[0] = 1.0f;
        gameObject->textureNames[0] = info[objIndex].texture;
        gameObject->textureBlend[1] = 0.0f;
        gameObject->textureNames[1] = info[objIndex].alpha;
        gameObject->bIsUpdatedInPhysics = false;
        gameObject->bIsWireFrame = false;
        gameObject->hasAlpha = true;
        gameObject->useDiscardAlpha = false;
        gameObject->cullFace = false;
        gameObject->rotateToCamera = true;
        gameObject->typeOfObject = TERRAIN;

        // Calculate a random position
        const int rangeX = maxX - minX;
        const int x = (std::rand() % rangeX) + minX;
        const int rangeY = maxY - minY;
        const int y = (std::rand() % rangeY) + minY;
        const int rangeZ = maxZ - minZ;
        const int z = (std::rand() % rangeZ) + minZ;

        gameObject->position = glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));


        theVecGo.push_back(gameObject);
    }

    return;
}

void TurnGameObjectToCamera(cGameObject* theGo, const glm::vec3 cameraPosition, const bool resetPitch)
{
    const glm::vec3 dir = normalize(cameraPosition - theGo->position);
    if (resetPitch)
    {
        // Discard X rotations
        const glm::vec3 clippedD = normalize(glm::vec3(dir.x, 0.0f, dir.z));
        const glm::quat qOrientation = rotation(glm::vec3(0.0f, 0.0f, 1.0f), clippedD);
        const glm::mat4 orientation = toMat4(qOrientation);
        theGo->orientation = orientation;
    }
    else
    {
        const glm::quat qOrientation = rotation(glm::vec3(0.0f, 0.0f, 1.0f), dir);
        const glm::mat4 orientation = toMat4(qOrientation);
        theGo->orientation = orientation;
    }
}

glm::mat4 GetMatrixFromVector(glm::vec3 xzDirection)
{
    // Discard y values
    xzDirection.y = 0;

    // Mage sure it is normalizard
    xzDirection = glm::normalize(xzDirection);

    const glm::quat qOrientation = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), xzDirection);
    return glm::toMat4(qOrientation);
}

bool LoadFileIntoString(std::string& theString, std::string fileName)
{   

    std::ifstream file(fileName.c_str());

    if(!file.is_open())
    {
        return false;
    }


    while(!file.eof())
    {
        std::string token = "";
        file >> token;
        
        // Ignore lines starting with #
        if(token != "")
        {
            if(token.at(0) == '#')
            {
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                //file >> token;
                continue;
            }
        }
        

        if (token != "")
            theString.append(token);

    }

    if(theString != "")
    {
        return true;
    }
    else
    {
        return false;
    }
    

    
}
