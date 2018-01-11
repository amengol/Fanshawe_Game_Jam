#ifndef _cSimpleAi_Manager_HG_
#define _cSimpleAi_Manager_HG_

#include "cGameObject.h"

void update_AI_Parameters(cGameObject* theGO, float threshold)
{
    float distanceToTarget = glm::length(theGO->target - theGO->position);

    if(distanceToTarget > threshold)
    {

        glm::vec3 direction = glm::normalize(theGO->target - theGO->position);
        glm::quat rot1 = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), direction);

        theGO->orientation = glm::toMat4(rot1);
    }
    else
    {
        unsigned int vecSize = theGO->wayPointList.size();
        if(theGO->currentTargetIndex == vecSize - 1)
        {
            theGO->currentTargetIndex = 0;
            theGO->target = theGO->wayPointList[0];
        }
        else
        {
            theGO->currentTargetIndex++;
            theGO->target = theGO->wayPointList[theGO->currentTargetIndex];
        }
    }
}

#endif // !_cSimpleAi_Manager_HG_

