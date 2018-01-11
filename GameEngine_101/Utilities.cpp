#include "Utilities.h"
#include <cstdlib>	// rand() and RAND_MAX
#include <time.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\vector_angle.hpp>

#include <string>
#include <fstream>
#include <iostream>

void createRamdomGameObjects(int numOfObjects,
                             std::vector<cGameObject*>& theVecGO,
                             std::vector<GameObjectsInfo> info,
                             float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ)
{
    srand(time(NULL));

    for(int i = 0; i < numOfObjects; i++)
    {
        cGameObject* theGO = new cGameObject();

        // Pic one object

        int objIndex = std::rand() % info.size();

        theGO->meshName = info[objIndex].meshName;
        theGO->textureBlend[0] = 1.0f;
        theGO->textureNames[0] = info[objIndex].texture;
        theGO->textureBlend[1] = 0.0f;
        theGO->textureNames[1] = info[objIndex].alpha;
        theGO->bIsUpdatedInPhysics = false;
        theGO->bIsWireFrame = false;
        theGO->hasAlpha = true;
        theGO->useDiscardAlpha = false;
        theGO->cullFace = false;
        theGO->rotateToCamera = true;
        theGO->typeOfObject = TERRAIN;

        // Calculate a random position
        int rangeX = maxX - minX;
        int x = (std::rand() % rangeX) + minX;
        int rangeY = maxY - minY;
        int y = (std::rand() % rangeY) + minY;
        int rangeZ = maxZ - minZ;
        int z = (std::rand() % rangeZ) + minZ;

        theGO->position = glm::vec3((float)x, (float)y, (float)z);


        theVecGO.push_back(theGO);
    }

    return;
}

void turnGameObjectToCamera(cGameObject* theGO, glm::vec3 cameraPosition)
{
    glm::vec3 dir = cameraPosition - theGO->position;
    // Discard X rotations
    glm::vec3 clippedD = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
    glm::quat qOrientation = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), clippedD);
    glm::mat4 orientation = glm::toMat4(qOrientation);
    theGO->orientation = orientation;
}

bool loadFileIntoString(std::string& theString, std::string fileName)
{   

    std::ifstream file(fileName.c_str());

    if(!file.is_open())
    {
        return false;
    }

    std::string token;
    file >> token;

    while(!file.eof())
    {
        
        // Ignore lines starting with #
        if(token.at(0) == '#')
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            file >> token;
            continue;
        }

        theString.append(token);

        file >> token;

    }

    // For the last token
    // Ignore lines starting with #
    if(theString != "")
    {
        if(token.at(0))
        {
            return true;
        }
    }
    

    theString.append(token);

    if(theString != "")
    {
        return true;
    }
    else
    {
        return false;
    }
    

    
}
