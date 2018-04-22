#include "cCharacterManager.h"
#include "cCharacterControl.h"
#include "..\cGameObject.h"
#include "..\Utilities.h"

cCharacterManager::cCharacterManager()
{
    mActiveCharacter = NULL;
}


cCharacterManager::~cCharacterManager()
{
}

bool cCharacterManager::AddCharacter(cCharacterControl* cc, std::string& error)
{
    if (cc->GetCharacter()->friendlyName == "")
    {
        error = "The Character Gameobject number is missing its friendly name!";
        return false;
    }

    cc->SetCharacter(cc->GetCharacter());

    mMapNameToCharacters[cc->GetCharacter()->friendlyName] = cc;
    return true;
}

bool cCharacterManager::SetControlledCharacter(std::string friendlyName, std::string & error)
{
        if (friendlyName == mActiveCharacterName)
            return true;
    
        std::map<std::string, cCharacterControl*>::iterator 
            it = mMapNameToCharacters.find(friendlyName);
    
        if (it == mMapNameToCharacters.end())
        {
            error = "The name of the character was not found!";
            return false;
        }
    
        // Init all animations
        cGameObject* GO = it->second->GetCharacter();
    
        // Make sure the GameObject has their animations ready
        if (!GO->InitCharacterAnimations(error))
        {
            return false;
        }
    
        mActiveCharacter = it->second;
        mActiveCharacterName = friendlyName;
        return true;
}

std::vector<cCharacterControl*> cCharacterManager::GetNPCs()
{
        std::vector<cCharacterControl*> npcs;
    
        std::map<std::string, cCharacterControl*>::iterator it = mMapNameToCharacters.begin();
    
        for (; it != mMapNameToCharacters.end(); it++)
        {
            if (it->second != mActiveCharacter)
            {
                // HACK for now
                if (it->second->GetCharacter()->friendlyName == "Hero01"
                    || it->second->GetCharacter()->friendlyName == "Hero02")
                    continue;
                npcs.push_back(it->second);
            }
        }
    
        return npcs;
}

void cCharacterManager::UpdateCollisions(float deltaTime)
{
    //std::vector<cCharacterControl*> vecCharacters;

    //std::map<std::string, cCharacterControl*>::iterator it = mMapNameToCharacters.begin();
    //for (; it != mMapNameToCharacters.end(); it++)
    //{
    //    vecCharacters.push_back(it->second);
    //}

    //for (size_t i = 0; i < vecCharacters.size(); i++)
    //{
    //    cCharacterControl* control1 = vecCharacters[i];
    //    cGameObject* character1 = control1->GetCharacter();

    //    for (size_t j = 0; j < vecCharacters.size(); j++)
    //    {
    //        cCharacterControl* control2 = vecCharacters[j];
    //        cGameObject* character2 = control2->GetCharacter();

    //        if (character1 == character2)
    //            continue;

    //        // Character 1 spheres
    //        std::vector<cGameObject::sContactSpheres*> vecCh1_spheres;
    //        std::map<std::string, cGameObject::sContactSpheres*>::iterator itSpheres = 
    //            character1->mMapBoneNameTOMeshName.begin();
    //        for (;itSpheres != character1->mMapBoneNameTOMeshName.end(); itSpheres++)
    //        {
    //            vecCh1_spheres.push_back(itSpheres->second);
    //        }

    //        // Character 2 spheres
    //        std::vector<cGameObject::sContactSpheres*> vecCh2_spheres;
    //        itSpheres = character2->mMapBoneNameTOMeshName.begin();
    //        for (; itSpheres != character2->mMapBoneNameTOMeshName.end(); itSpheres++)
    //        {
    //            vecCh2_spheres.push_back(itSpheres->second);
    //        }

    //        // Now the "Collision"
    //        for (size_t s1Index = 0; s1Index < vecCh1_spheres.size(); s1Index++)
    //        {
    //            cGameObject::sContactSpheres* sphere1 = vecCh1_spheres[s1Index];
    //            
    //            for (size_t s2Index = 0; s2Index < vecCh2_spheres.size(); s2Index++)
    //            {
    //                cGameObject::sContactSpheres* sphere2 = vecCh2_spheres[s2Index];

    //                if (glm::length(sphere1->position - sphere2->position) <= sphere1->radius + sphere2->radius)
    //                {
    //                    // HACK: the names are hard coded...

    //                    // if one chacater is in idle, he can't hi by standing still
    //                    if (character1->characterAnim != eCharacterAnim::IDLE
    //                        && character1->characterAnim != eCharacterAnim::STUNNED)
    //                    {
    //                        // For now, only torax will lit
    //                        if (sphere1->meshName == "sphere_fist")
    //                        {
    //                            if (sphere2->meshName != "sphere_fist")
    //                            {
    //                                sphere2->colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //                                if (!sphere2->hasCollided)
    //                                {
    //                                    sphere2->collisionPosition = sphere1->position;
    //                                }
    //                                sphere2->hasCollided = true;
    //                                sphere2->elapseTime = 0.0f;
    //                                
    //                                float hit = getRandInRange(0.05f, 0.1f);
    //                                control2->Hurt(hit * deltaTime);
    //                                if (control2->GetHealth() <= 0.1f)
    //                                    control2->Dying();
    //                                else
    //                                    control2->Stunned();
    //                            }
    //                        }
    //                    }
    //                    else if (character2->characterAnim != eCharacterAnim::IDLE
    //                             && character2->characterAnim != eCharacterAnim::STUNNED)
    //                    {
    //                        // For now, only torax will lit
    //                        if (sphere2->meshName == "sphere_fist")
    //                        {
    //                            if (sphere1->meshName != "sphere_fist")
    //                            {
    //                                sphere1->colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //                                if (!sphere1->hasCollided)
    //                                {
    //                                    sphere1->collisionPosition = sphere2->position;
    //                                }
    //                                sphere1->hasCollided = true;
    //                                sphere1->elapseTime = 0.0f;
    //                                
    //                                float hit = getRandInRange(0.05f, 0.1f);
    //                                control1->Hurt(hit * deltaTime);
    //                                if (control1->GetHealth() <= 0.1f)
    //                                    control1->Dying();
    //                                else
    //                                    control1->Stunned();
    //                            }
    //                        }
    //                    }
    //                    else
    //                    {
    //                        // For now, only torax will lit
    //                        if (sphere1->meshName == "sphere_fist")
    //                        {
    //                            if (sphere2->meshName != "sphere_fist")
    //                            {
    //                                sphere2->colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //                                if (!sphere2->hasCollided)
    //                                {
    //                                    sphere2->collisionPosition = sphere1->position;
    //                                }
    //                                sphere2->hasCollided = true;
    //                                sphere2->elapseTime = 0.0f;
    //                                
    //                                float hit = getRandInRange(0.05f, 0.1f);
    //                                control2->Hurt(hit * deltaTime);
    //                                if (control2->GetHealth() <= 0.1f)
    //                                    control2->Dying();
    //                                else
    //                                    control2->Stunned();
    //                            }
    //                        }
    //                        if (sphere2->meshName == "sphere_fist")
    //                        {
    //                            if (sphere1->meshName != "sphere_fist")
    //                            {
    //                                sphere1->colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //                                if (!sphere1->hasCollided)
    //                                {
    //                                    sphere1->collisionPosition = sphere2->position;
    //                                }
    //                                sphere1->hasCollided = true;
    //                                sphere1->elapseTime = 0.0f;
    //                                
    //                                float hit = getRandInRange(0.05f, 0.1f);
    //                                control1->Hurt(hit * deltaTime);
    //                                if (control1->GetHealth() <= 0.1f)
    //                                    control1->Dying();
    //                                else
    //                                    control1->Stunned();
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

}
