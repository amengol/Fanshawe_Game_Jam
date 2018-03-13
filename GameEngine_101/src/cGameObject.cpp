#include "cGameObject.h"
#include <fmod\fmod.hpp>
#include "cMesh.h"
#include "cSoundObject.h"
#include "sCollisionTriangle.h"
#include "Assimp\cSkinnedMesh.h"
#include "globalGameStuff.h"

cGameObject::cGameObject()
{
	this->scale = 1.0f;
    this->rateOfTurnX = 0.0f;
    this->rateOfTurnY = 0.0f;
    this->rateOfTurnZ = 0.0f;
	this->position = glm::vec3(0.0f);
    this->previousPosition = glm::vec3(0.0f);
	this->orientation = glm::mat4x4(1.0f);
    this->vecOrientation = glm::vec3(0.0f);
	this->vel = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->diffuseColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->bIsUpdatedInPhysics = true;
    this->hadAlreadyCollided = false;
	this->radius = 0.0f;
	this->typeOfObject = eTypeOfObject::UNKNOWN;
    this->mGOSound = NULL;
    this->mHasSound = false;
	this->bIsWireFrame = false;
    this->renderable = true;
    this->hasColour = false;
    this->hasAlpha = false;
    this->useDiscardAlpha = true;
    this->cullFace = true;
    this->hasReflection = false;
    this->isDebugAABBActive = false;
    this->hasLights = false;
    this->rotateToCamera = false;
    this->hasAI = false;
    this->target = glm::vec3(0.0f);
    this->rigidBody = NULL;
    this->softBody = NULL;
    this->bt_rigidBody = NULL;
    this->pSimpleSkinnedMesh = NULL;
    this->pAniState = NULL;	
    this->startDisplacement;
    this->animationsInitiated = false;

#ifdef _DEBUG
 	//this->m_pTheDR = cDebugRenderer::getInstance();
 #endif

	return;
}

cGameObject::~cGameObject()
{
	return;
}

void cGameObject::SetPostiion(glm::vec3 pos)
{
    this->position = pos;
    this->startDisplacement = pos;
}

void cGameObject::setCollisionGeometry(cMesh mesh)
{
    sCollisionGeometry collGeometry;

    // Put each triangle of the mesh in a sCollisionTriangle
    for (int i = 0; i < mesh.numberOfTriangles; i++)
    {
        // Reconstrunct the triangle
        glm::vec3 vertA;
        glm::vec3 vertB;
        glm::vec3 vertC;
        glm::vec3 normA;
        glm::vec3 normB;
        glm::vec3 normC;

        vertA.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].x;
        vertA.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].y;
        vertA.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].z;
        normA.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].nx;
        normA.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].ny;
        normA.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_0].nz;

        vertB.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].x;
        vertB.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].y;
        vertB.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].z;
        normB.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].nx;
        normB.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].ny;
        normB.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_1].nz;

        vertC.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].x;
        vertC.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].y;
        vertC.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].z;
        normC.x = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].nx;
        normC.y = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].ny;
        normC.z = mesh.pVertices[mesh.pTriangles[i].vertex_ID_2].nz;

        glm::vec3 faceNormal = glm::normalize(normA + normB + normC);

        sCollisionTriangle* tri = new sCollisionTriangle();
        tri->setTriangle(vertA, vertB, vertC);
        tri->faceNormal = faceNormal;

        collGeometry.meshName = mesh.name;
        collGeometry.collisionTriangles.push_back(tri);
    }

    this->contacPoints.push_back(collGeometry);

    
}

void cGameObject::DebugUpdate(double deltaTime)
 {
 #ifdef _DEBUG
 	/*this->m_pTheDR->AddLine( this->position, 
 							 this->position + this->vel, 
 							 this->diffuseColour );*/
 #endif
 	return;
 }
 
 //void cGameObject::Update(float deltaTime, glm::vec3 GRAVITY)
 //{
 //    // Remember the last position
 //    this->previousPosition = this->position;

 //    // Explicit Euler  (RK4)
 //    // New position is based on velocity over time
 //    // Velocity is based on local axis
 //    glm::vec3 newVel = this->orientation * glm::vec4(this->vel, 0.0f);
 //    glm::vec3 deltaPosition = (float)deltaTime * newVel;
 //    this->position += deltaPosition;

 //    // New velocity is based on acceleration over time
 //    // Acceleration is based on local axis
 //    //glm::vec3 newAccel = this->orientation * glm::vec4(this->accel, 0.0f);
 //    glm::vec3 deltaVelocity = ((float)deltaTime * /*newAccel*/this->accel)
 //        + ((float)deltaTime * GRAVITY);
 //    this->vel += deltaVelocity;

 //    //------------------------------------------------------------------------
 //    // Change orientation according to the Rate Of Turn per minute     
 //    if (this->rateOfTurnX != 0.0f)
 //    {
 //        float theTurnX = (this->rateOfTurnX * deltaTime) / 60.0f;         
 //        this->orientation = glm::rotate(this->orientation, glm::radians(theTurnX), glm::vec3(1.0f, 0.0f, 0.0f));
 //    }
 //    if (this->rateOfTurnY != 0.0f)
 //    {
 //        float theTurnY = (this->rateOfTurnY * deltaTime) / 60.0f;
 //        this->orientation = glm::rotate(this->orientation, glm::radians(theTurnY), glm::vec3(0.0f, 1.0f, 0.0f));
 //    }
 //    if (this->rateOfTurnZ != 0.0f)
 //    {
 //        float theTurnZ = (this->rateOfTurnZ * deltaTime) / 60.0f;
 //        this->orientation = glm::rotate(this->orientation, glm::radians(theTurnZ), glm::vec3(0.0f, 0.0f, 1.0f));
 //    }
 //    //------------------------------------------------------------------------ 
 //    // Update sound Objects
 //    if(this->mHasSound)
 //        this->mGOSound->setPosition(this->position);
 //    //------------------------------------------------------------------------ 
 // }

 void cGameObject::rotateX(float degreesX)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesX), glm::vec3(1.0f, 0.0f, 0.0f));
 }

 void cGameObject::rotateY(float degreesY)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesY), glm::vec3(0.0f, 1.0f, 0.0f));
 }

 void cGameObject::rotateZ(float degreesZ)
 {
     this->orientation = glm::rotate(this->orientation, glm::radians(degreesZ), glm::vec3(0.0f, 0.0f, 1.0f));
 }

 bool cGameObject::initSoundObject(std::string name)
 {
     if(!this->mHasSound) {
         cSoudObject* theSO = new cSoudObject();
         theSO->setFMODType(FMOD_3D);
         theSO->setPosition(this->position);
         theSO->setMovType(2);
         theSO->setfriendlyName(name);
         this->mGOSound = theSO;
         mHasSound = true;
         return true;
     }
     return false;
 }

 bool cGameObject::hasSound()
 {
     return this->mHasSound;
 }

 cSoudObject* cGameObject::getSoundObject()
 {
     if(this->mHasSound)
         return this->mGOSound;
     else {
         this->initSoundObject("Generic");
         return this->mGOSound;
     }

 }

 std::string cGameObject::getSoundName()
 {
     if(this->mHasSound)
         return this->mGOSound->getFriendlyName();
 }

 void cGameObject::setGOSoundPos()
 {
     this->mGOSound->setPosition(this->position);
 }

 bool cGameObject::InitCharacterAnimations(std::string & error)
 {
     // If we already initiated, don't bother...
     if (animationsInitiated)
         return true;

     bool allIsGood = true;
     std::string animationName;

     animationName = animations.walking;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }

     animationName = animations.walking_backwards;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.running;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.jump;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.jump_forward;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.left_strafe;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.left_strafe_walking;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }

     animationName = animations.left_turn;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.left_turn_90;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.right_strafe;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.right_strafe_walking;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.right_turn;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationName = animations.right_turn_90;
     if (animationName != "")
     {
         const aiScene* anim = g_animationCollection.getAnimation(animationName);

         if (anim != NULL)
         {
             this->pSimpleSkinnedMesh->AddAnimationScene(anim, animationName);
         }
         else
         {
             error.append("The animation " + animationName + " was not found in the Animation Collection class\n");
             allIsGood = false;
         }
     }
     
     animationsInitiated = allIsGood;
     return allIsGood;
 }
