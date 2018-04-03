#include "DrawCalls.h"
#include "cGameObject.h"
#include <cSoftBody.h>
#include <cCloth.h>
#include "globalOpenGL_GLFW.h"
#include <glm/gtc/type_ptr.hpp>
#include "globalGameStuff.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Assimp\cAnimationState.h"
#include "AI\cCharacterControl.h"
#include "Utilities.h"

bool g_clothDebug = false;

void ClothDraw(cGameObject* pTheGO)
{    

    nPhysics::cSoftBody* sb = static_cast<nPhysics::cSoftBody*>(pTheGO->softBody);

    if (sb == NULL)
        return;

    nPhysics::cCloth* cloth = static_cast<nPhysics::cCloth*>(sb->GetForm());

    if (cloth == NULL)
        return;

    //cloth->TimeStep(0.5f * 0.5f, 15);

    nPhysics::cClothMesh clothMesh;

    cloth->GetClothMesh(clothMesh);

    cMesh theMesh;
    theMesh.name = clothMesh.name;
    theMesh.scaleForUnitBox = clothMesh.scaleForUnitBox;
    theMesh.maxExtent = clothMesh.maxExtent;
    theMesh.maxExtentXYZ = clothMesh.maxExtentXYZ;
    theMesh.maxXYZ = clothMesh.maxXYZ;

    theMesh.numberOfVertices = clothMesh.numberOfVertices;
    sVertex* pVertices = new sVertex[theMesh.numberOfVertices];
    for (size_t i = 0; i < theMesh.numberOfVertices; i++)
    {
        pVertices[i].x = clothMesh.pVertices[i].x;
        pVertices[i].y = clothMesh.pVertices[i].y;
        pVertices[i].z = clothMesh.pVertices[i].z;

        pVertices[i].r = clothMesh.pVertices[i].r;
        pVertices[i].g = clothMesh.pVertices[i].g;
        pVertices[i].b = clothMesh.pVertices[i].b;
        pVertices[i].a = clothMesh.pVertices[i].a;

        pVertices[i].nx = clothMesh.pVertices[i].nx;
        pVertices[i].ny = clothMesh.pVertices[i].ny;
        pVertices[i].nz = clothMesh.pVertices[i].nz;

        pVertices[i].u1 = clothMesh.pVertices[i].u1;
        pVertices[i].v1 = clothMesh.pVertices[i].v1;

        if (g_clothDebug)
        {
            cGameObject* debugSphere = new cGameObject;
            debugSphere->meshName = "Low_Poly_Sphere";
            debugSphere->scale = 0.1f;
            debugSphere->bIsWireFrame = true;
            debugSphere->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            debugSphere->position.x = pVertices[i].x;
            debugSphere->position.y = pVertices[i].y;
            debugSphere->position.z = pVertices[i].z;

            DrawObject(debugSphere);

            delete debugSphere;
        }
    }
    delete[] clothMesh.pVertices;


    theMesh.numberOfTriangles = clothMesh.numberOfTriangles;
    cTriangle* pTriangles = new cTriangle[theMesh.numberOfTriangles];
    for (size_t i = 0; i < theMesh.numberOfTriangles; i++)
    {
        pTriangles[i].vertex_ID_0 = clothMesh.pTriangles[i].vertex_ID_0;
        pTriangles[i].vertex_ID_1 = clothMesh.pTriangles[i].vertex_ID_1;
        pTriangles[i].vertex_ID_2 = clothMesh.pTriangles[i].vertex_ID_2;

    }
    delete[] clothMesh.pTriangles;


    theMesh.pVertices = pVertices;
    theMesh.pTriangles = pTriangles;
    ::g_pVAOManager->loadMeshIntoDynamicVAO(theMesh, "Cloth");

    delete[] pTriangles;
    delete[] pVertices;

    // drawing
    glm::mat4x4 mModel = glm::mat4x4(1.0f);
    float finalScale = 1.0f;

    glm::mat4 matScale = glm::mat4x4(1.0f);
    matScale = glm::scale(matScale, glm::vec3(finalScale, finalScale, finalScale));
    mModel = mModel * matScale;

    glUniformMatrix4fv(g_uniLocHandler.mModel, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mModel));

    glm::mat4 mWorldInvTranpose = glm::inverse(glm::transpose(mModel));
    glUniformMatrix4fv(g_uniLocHandler.mWorldInvTrans, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mWorldInvTranpose));

    // Diffuse is often 0.2-0.3 the value of the diffuse
    glUniform1f(g_uniLocHandler.ambientToDiffuseRatio, 0.2f);

    // Specular: For now, set this colour to white, and the shininess to something high 
    //	it's an exponent so 64 is pretty shinny (1.0 is "flat", 128 is excessively shiny)
    glUniform4f(g_uniLocHandler.materialSpecular,
                0.0f,
                0.0f,
                0.0f,
                1.0f);


    glUniform4f(g_uniLocHandler.materialDiffuse,
                1.0f,
                0.0f,
                0.0f,
                1.0f);

    glUniform1f(g_uniLocHandler.hasColour, 0.0f);
    glUniform1f(g_uniLocHandler.hasAlpha, 0.0f);
    glUniform1f(g_uniLocHandler.useDiscardAlpha, 0.0f);
    glUniform1f(g_uniLocHandler.isASkyBox, GL_FALSE);
    glUniform1f(g_uniLocHandler.hasReflection, 0.0f);

    if (g_clothDebug)
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 0.0f);
    }



    // Set up the textures
    std::string textureName = "Fanshawe.bmp";
    GLuint texture00Number
        = ::g_pTextureManager->getTextureIDFromTextureName(textureName);
    //// Texture binding... (i.e. set the 'active' texture
    //GLuint texture00Unit = 0;							// Texture units go from 0 to 79 (at least)
    //glActiveTexture(texture00Unit + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
    //glBindTexture(GL_TEXTURE_2D, texture00Number);

    // 0 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(textureName));


    // Set sampler in the shader
    // NOTE: You shouldn't be doing this during the draw call...
    GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    GLint textSampler00_ID = glGetUniformLocation(curShaderID, "texSamp2D00");

    GLint textBlend00_ID = glGetUniformLocation(curShaderID, "texBlend00");


    // This connects the texture sampler to the texture units... 
    glUniform1i(textSampler00_ID, 0);

    // .. and so on

    // And the blending values
    glUniform1f(textBlend00_ID, 1.0f);


    GLint UniLoc_IsSkinnedMesh = glGetUniformLocation(curShaderID, "bIsASkinnedMesh");
    glUniform1f(UniLoc_IsSkinnedMesh, GL_FALSE);

    if (g_clothDebug)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }


    //GLint UniLoc_IsCloth = glGetUniformLocation(curShaderID, "isCloth");
    //glUniform1f(UniLoc_IsCloth, GL_TRUE);

    sVAOInfo VAODrawInfo;
    ::g_pVAOManager->lookupDynamicVAOFromName("Cloth", VAODrawInfo);

    glBindVertexArray(VAODrawInfo.VAO_ID);

    glDrawElements(GL_TRIANGLES, VAODrawInfo.numberOfIndices, GL_UNSIGNED_INT, 0);

    // Unbind that VAO
    glBindVertexArray(0);

    //glUniform1f(UniLoc_IsCloth, GL_FALSE);
}

//****************************************************************************************
//    ___  _    _                      _  __  __           _     
//   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_   
//   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \  
//   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_| 
//                                                               
void CalculateSkinnedMeshBonesAndLoad(cGameObject* pTheGO,
                                      unsigned int UniformLoc_numBonesUsed,
                                      unsigned int UniformLoc_bonesArray)
{

    std::string animationToPlay = "";
    float curFrameTime = 0.0;


    // See what animation should be playing... 
    cAnimationState* pAniState = pTheGO->pAniState;

    float deltaTime = glfwGetTime() - pAniState->activeAnimation.currentClockTime;

    // Update the next position
    if (pAniState->activeAnimation.IncrementTime(deltaTime))
    {
        // Jump only once!
        if (pAniState->activeAnimation.name == pTheGO->animations.jump)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
        }

        // Jump only once!
        if (pAniState->activeAnimation.name == pTheGO->animations.jump_forward_walking)
        {
            pAniState->activeAnimation.name = pTheGO->animations.walking;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.walking);
        }
        if (pAniState->activeAnimation.name == pTheGO->animations.jump_forward_running)
        {
            pAniState->activeAnimation.name = pTheGO->animations.running;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.running);
        }

        // Fight control
        if (pTheGO->characterAnim == eCharacterAnim::RIGHT_CROSS_PUNCH
            || pTheGO->characterAnim == eCharacterAnim::LEFT_CROSS_PUNCH)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }

        if (pTheGO->characterAnim == eCharacterAnim::STUNNED)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }

        if (pTheGO->characterAnim == eCharacterAnim::DYING_RISING)
        {
            pAniState->activeAnimation.name = pTheGO->animations.dying;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.dying);
            pAniState->activeAnimation.currentTime = pAniState->activeAnimation.totalTime - 0.001f;
            pTheGO->characterAnim = eCharacterAnim::DEAD;
        }
    }

    if (pTheGO->characterAnim == eCharacterAnim::DEAD)
    {
        pAniState->activeAnimation.name = pTheGO->animations.dying;
        pAniState->activeAnimation.totalTime =
            pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.dying);
        pAniState->activeAnimation.currentTime = pAniState->activeAnimation.totalTime - 0.001f;
        pTheGO->characterAnim = eCharacterAnim::DEAD;
    }

    animationToPlay = pAniState->activeAnimation.name;
    curFrameTime = pAniState->activeAnimation.currentTime;
    pAniState->activeAnimation.currentClockTime = glfwGetTime();


     // Set up the animation pose:
    std::vector< glm::mat4x4 > vecFinalTransformation;
    std::vector< glm::mat4x4 > vecObjectBoneTransformation;
    std::vector< glm::mat4x4 > vecOffsets;
    // Final transformation is the bone transformation + boneOffsetPerVertex
    // ObjectBoneTransformation (or "Global") is the final location of the bones
    // vecOffsets is the relative offsets of the bones from each other


    pTheGO->pSimpleSkinnedMesh->BoneTransform(curFrameTime,
                                              animationToPlay,		//**NEW**
                                              vecFinalTransformation,		// Final bone transforms for mesh
                                              vecObjectBoneTransformation,  // final location of bones
                                              vecOffsets);                 // local offset for each bone

    //=========================================================================
    // Contact points update
    std::map<std::string, cGameObject::sContactSpheres*>::iterator itGO = pTheGO->mMapBoneNameTOMeshName.begin();

    for (; itGO != pTheGO->mMapBoneNameTOMeshName.end(); itGO++)
    {
        std::string boneName = itGO->first;
        cGameObject::sContactSpheres* contactSphere = itGO->second;

        std::map<std::string, glm::mat4>::iterator itSkinned = 
            pTheGO->pSimpleSkinnedMesh->mMapBoneToLastLocalTranslation.find(boneName);

        if (itSkinned != pTheGO->pSimpleSkinnedMesh->mMapBoneToLastLocalTranslation.end())
        {
            glm::mat4 boneTranslation = itSkinned->second * pTheGO->scale;
            glm::mat4 GO_Orientation;
            pTheGO->rigidBody->GetMatOrientation(GO_Orientation);
            GO_Orientation *= boneTranslation;
            //boneTranslation *= GO_Orientation;

            glm::vec4 vecTrans(1.0f, 1.0f, 1.0f, 1.0f);
            glm::vec4 pos = GO_Orientation * vecTrans;

            //cGameObject* sphere = new cGameObject();
            //sphere->meshName = contactSphere->meshName;
            //sphere->hasColour = true;
            //sphere->diffuseColour = contactSphere->colour;
            //sphere->typeOfObject = eTypeOfObject::PLANE;
            //sphere->position = glm::vec3(pos.x, pos.y, pos.z) + pTheGO->position;
            
            // Update the contact sphere
            contactSphere->position = glm::vec3(pos.x, pos.y, pos.z) + pTheGO->position;;

            

            if (contactSphere->hasCollided)
            {
                contactSphere->elapseTime += deltaTime;
                if (contactSphere->elapseTime >= contactSphere->timeToFade)
                {
                    contactSphere->hasCollided = false;
                    contactSphere->elapseTime = 0.0f;
                }

                cGameObject* sphereX = new cGameObject();
                sphereX->meshName = "sphere_fist";
                sphereX->diffuseColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
                sphereX->bIsWireFrame = true;
                sphereX->typeOfObject = eTypeOfObject::PLANE;
                sphereX->position = contactSphere->collisionPosition;
                float scale = contactSphere->elapseTime / contactSphere->timeToFade;
                sphereX->scale += scale;

                DrawObject(sphereX);

                delete sphereX;

                //pAniState->activeAnimation.name = pTheGO->animations.stunned;
                //pAniState->activeAnimation.totalTime =
                //    pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.stunned);
                //pTheGO->characterAnim = eCharacterAnim::STUNNED;
            }
            else if (pTheGO->characterAnim == eCharacterAnim::STUNNED)
            {
                //pAniState->activeAnimation.name = pTheGO->animations.idle;
                //pAniState->activeAnimation.totalTime =
                //    pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
                //pTheGO->characterAnim = eCharacterAnim::IDLE;
            }
        }
    }

    //=========================================================================

    unsigned int numberOfBonesUsed = static_cast< unsigned int >(vecFinalTransformation.size());
    glUniform1i(UniformLoc_numBonesUsed, numberOfBonesUsed);

    glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);
    // UniformLoc_bonesArray is the getUniformLoc of "bones[0]" from
    //	uniform mat4 bones[MAXNUMBEROFBONES] 
    // in the shader
    glUniformMatrix4fv(UniformLoc_bonesArray, numberOfBonesUsed, GL_FALSE,
        (const GLfloat*)glm::value_ptr(*pBoneMatrixArray));

    return;
}

void DrawObject(cGameObject* pTheGO)
{

    // Is there a game object? 
    if (pTheGO == 0)
    {
        return;
    }

    // Is the game object renderable?
    if (!pTheGO->renderable)
    {
        return;
    }

    // Update lights
    if (pTheGO->hasLights)
    {
        for (int i = 0; i < pTheGO->vecLightsInfo.size(); i++)
        {
            lightInfo lightInfo = pTheGO->vecLightsInfo[i];
            glm::vec3 rotatedOffset = pTheGO->orientation * glm::vec4(lightInfo.offset, 0.0f);
            g_pLightManager->vecLights[lightInfo.index].position = pTheGO->position + rotatedOffset;
            // Update directly to not get a delay
            glUniform4f(g_pLightManager->vecLights[lightInfo.index].shaderlocID_position,
                        g_pLightManager->vecLights[lightInfo.index].position.x,
                        g_pLightManager->vecLights[lightInfo.index].position.y,
                        g_pLightManager->vecLights[lightInfo.index].position.z,
                        1.0f);
            if (lightInfo.type == SPOT)
            {
                glm::vec3 rotatedFocus = pTheGO->orientation * glm::vec4(lightInfo.focusDirection, 0.0f);
                g_pLightManager->vecLights[lightInfo.index].direction = rotatedFocus;
                // Update directly to not get a delay
                glUniform4f(g_pLightManager->vecLights[lightInfo.index].shaderlocID_direction,
                            g_pLightManager->vecLights[lightInfo.index].direction.x,
                            g_pLightManager->vecLights[lightInfo.index].direction.y,
                            g_pLightManager->vecLights[lightInfo.index].direction.z,
                            1.0f);
            }
        }
    }

    // Was near the draw call, but we need the mesh name
    std::string meshToDraw = pTheGO->meshName;

    sVAOInfo VAODrawInfo;
    if (::g_pVAOManager->lookupStaticVAOFromName(meshToDraw, VAODrawInfo) == false)
    {	// Didn't find mesh
        return;
    }

    GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    // ***************************************************
    //    ___  _    _                      _  __  __           _     
    //   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_   
    //   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \  
    //   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_| 
    //                                                               
    GLint UniLoc_IsSkinnedMesh = glGetUniformLocation(curShaderID, "bIsASkinnedMesh");

    if (pTheGO->pSimpleSkinnedMesh)
    {
        // Calculate the pose and load the skinned mesh stuff into the shader, too
        GLint UniLoc_NumBonesUsed = glGetUniformLocation(curShaderID, "numBonesUsed");
        GLint UniLoc_BoneIDArray = glGetUniformLocation(curShaderID, "bones");
        CalculateSkinnedMeshBonesAndLoad(pTheGO, UniLoc_NumBonesUsed, UniLoc_BoneIDArray);

        glUniform1f(UniLoc_IsSkinnedMesh, GL_TRUE);
    }
    else
    {
        glUniform1f(UniLoc_IsSkinnedMesh, GL_FALSE);
    }

    // ***************************************************

    // There IS something to draw

    glm::mat4x4 mModel = glm::mat4x4(1.0f);



    glm::mat4 trans = glm::mat4x4(1.0f);

    // Position by nPhysics?
    if (pTheGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
    {
        glm::vec3 rbPos;
        pTheGO->rigidBody->GetPostion(rbPos);
        trans = glm::translate(trans, rbPos);
    }
    else if (pTheGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
    {
        glm::vec3 rbPos;
        pTheGO->bt_rigidBody->GetPostion(rbPos);
        trans = glm::translate(trans, rbPos);
    }
    else
    {
        trans = glm::translate(trans, pTheGO->position);
    }

    mModel = mModel * trans;

    // Orientation by nPhysics?
    glm::mat4 orientation;
    if (pTheGO->rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.SUPERDUPER)
    {
        pTheGO->rigidBody->GetMatOrientation(orientation);
        mModel = mModel * orientation;
    }
    else if (pTheGO->bt_rigidBody != NULL && g_physicsSwitcher.gPhysicsEngine == g_physicsSwitcher.BULLET)
    {
        pTheGO->bt_rigidBody->GetMatOrientation(orientation);
        mModel = mModel * orientation;
    }
    else
    {
        mModel = mModel * pTheGO->orientation;
    }

    float finalScale = pTheGO->scale;

    glm::mat4 matScale = glm::mat4x4(1.0f);
    matScale = glm::scale(matScale, glm::vec3(finalScale, finalScale, finalScale));
    mModel = mModel * matScale;

    glUniformMatrix4fv(g_uniLocHandler.mModel, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mModel));

    glm::mat4 mWorldInvTranpose = glm::inverse(glm::transpose(mModel));
    glUniformMatrix4fv(g_uniLocHandler.mWorldInvTrans, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mWorldInvTranpose));

    // Diffuse is often 0.2-0.3 the value of the diffuse
    glUniform1f(g_uniLocHandler.ambientToDiffuseRatio, 0.2f);

    // Specular: For now, set this colour to white, and the shininess to something high 
    //	it's an exponent so 64 is pretty shinny (1.0 is "flat", 128 is excessively shiny)
    glUniform4f(g_uniLocHandler.materialSpecular,
                pTheGO->specular.x,
                pTheGO->specular.y,
                pTheGO->specular.z,
                pTheGO->specular.w);


    glUniform4f(g_uniLocHandler.materialDiffuse,
                pTheGO->diffuseColour.r,
                pTheGO->diffuseColour.g,
                pTheGO->diffuseColour.b,
                pTheGO->diffuseColour.a);

    if (pTheGO->hasColour)
    {
        glUniform1f(g_uniLocHandler.hasColour, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasColour, 0.0f);
    }

    if (pTheGO->hasAlpha)
    {
        glUniform1f(g_uniLocHandler.hasAlpha, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasAlpha, 0.0f);
    }

    if (pTheGO->useDiscardAlpha)
    {
        glUniform1f(g_uniLocHandler.useDiscardAlpha, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.useDiscardAlpha, 0.0f);
    }

    //...and all the other object material colours

    if (pTheGO->bIsWireFrame)
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 1.0f);	// TRUE
    }
    else
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 0.0f);	// FALSE
    }


    if (pTheGO->typeOfObject == SKYBOX)
    {
        glUniform1f(g_uniLocHandler.isASkyBox, GL_TRUE);
    }
    else
    {
        glUniform1f(g_uniLocHandler.isASkyBox, GL_FALSE);
    }

    if (pTheGO->hasReflection)
    {
        glUniform1f(g_uniLocHandler.hasReflection, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasReflection, 0.0f);
    }

    // Set up cube map...
    GLuint cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("space");
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

    // Set up the textures
    std::string textureName = pTheGO->textureNames[0];
    GLuint texture00Number
        = ::g_pTextureManager->getTextureIDFromTextureName(textureName);
    // Texture binding... (i.e. set the 'active' texture
    GLuint texture00Unit = 0;							// Texture units go from 0 to 79 (at least)
    glActiveTexture(texture00Unit + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, texture00Number);

    // 0 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[0]));
    // 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[1]));

    // 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[2]));
    // 2..  and so on... 

    // Set sampler in the shader
    // NOTE: You shouldn't be doing this during the draw call...
    GLint textSampler00_ID = glGetUniformLocation(curShaderID, "texSamp2D00");
    GLint textSampler01_ID = glGetUniformLocation(curShaderID, "texSamp2D01");
    GLint textSampler02_ID = glGetUniformLocation(curShaderID, "texSamp2D02");
    //// And so on (up to 10, or whatever number of textures)... 

    GLint textBlend00_ID = glGetUniformLocation(curShaderID, "texBlend00");
    GLint textBlend01_ID = glGetUniformLocation(curShaderID, "texBlend01");
    GLint textBlend02_ID = glGetUniformLocation(curShaderID, "texBlend02");

    GLint texSampCube00_LocID = glGetUniformLocation(curShaderID, "texSampCube00");
    GLint texSampCube01_LocID = glGetUniformLocation(curShaderID, "texSampCube00");
    GLint texSampCube02_LocID = glGetUniformLocation(curShaderID, "texSampCube00");
    GLint texSampCube03_LocID = glGetUniformLocation(curShaderID, "texSampCube00");

    GLint texCubeBlend00_LocID = glGetUniformLocation(curShaderID, "texCubeBlend00");
    GLint texCubeBlend01_LocID = glGetUniformLocation(curShaderID, "texCubeBlend01");
    GLint texCubeBlend02_LocID = glGetUniformLocation(curShaderID, "texCubeBlend02");
    GLint texCubeBlend03_LocID = glGetUniformLocation(curShaderID, "texCubeBlend03");

    glUniform1i(texSampCube00_LocID, 31);
    glUniform1i(texSampCube01_LocID, 31);
    glUniform1i(texSampCube02_LocID, 31);
    glUniform1i(texSampCube03_LocID, 31);

    // This connects the texture sampler to the texture units... 
    glUniform1i(textSampler00_ID, 0);
    glUniform1i(textSampler01_ID, 1);
    glUniform1i(textSampler02_ID, 2);
    // .. and so on

    // And the blending values
    glUniform1f(textBlend00_ID, pTheGO->textureBlend[0]);
    glUniform1f(textBlend01_ID, pTheGO->textureBlend[1]);
    glUniform1f(textBlend02_ID, pTheGO->textureBlend[2]);
    // And so on...

    //			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    //			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    if (pTheGO->bIsWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }
    else if (pTheGO->cullFace)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    glCullFace(GL_BACK);

    glBindVertexArray(VAODrawInfo.VAO_ID);

    glDrawElements(GL_TRIANGLES, VAODrawInfo.numberOfIndices, GL_UNSIGNED_INT, 0);

    // Unbind that VAO
    glBindVertexArray(0);

    return;
}

void RenderScene(std::vector<cGameObject*>& vec_pGOs, GLFWwindow* pGLFWWindow, cCameraObject* pCamera, double deltaTime)
{
    float ratio;
    int width, height;
    glfwGetFramebufferSize(pGLFWWindow, &width, &height);

    // Prevent division by zero!
    if (height == 0)
        height = 1;

    ratio = width / (float)height;
    glViewport(0, 0, width, height);

    // Clear colour AND depth buffer
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ::g_pShaderManager->useShaderProgram("GE101_Shader");
    GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    // Update all the light uniforms...
    // (for the whole scene)
    ::g_pLightManager->CopyLightInformationToCurrentShader();

    //---------------------------------------------------------------------
    // Camera block

    glm::mat4x4 matProjection;

    // Projection and view don't change per scene (maybe)
    matProjection = glm::perspective(pCamera->mFOV,   // FOV
                                     ratio,		      // Aspect ratio
                                     1.0f,		   	  // Near (as big as possible)
                                     200000.0f);      // Far (as small as possible)

    pCamera->update();

    ::g_pSkyBoxObject->position = pCamera->getCameraPosition();

    // View or "camera" matrix
    glm::mat4 matView = glm::mat4(1.0f);

    matView = glm::lookAt(pCamera->getCameraPosition(),		// "eye" or "camera" position
                          pCamera->getLookAtPosition(),				// "At" or "target" 
                          pCamera->getCameraUpVector());	// "up" vector

    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matView));
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matProjection));

    //---------------------------------------------------------------------
    // "Draw scene" loop

    unsigned int sizeOfVector = (unsigned int)vec_pGOs.size();
    for (int index = 0; index != sizeOfVector; index++)
    {
        cGameObject* pTheGO = vec_pGOs[index];

        if (pTheGO->typeOfObject == CLOTH)
        {
            ClothDraw(pTheGO);
        }
        else
        {
            DrawObject(pTheGO);
        }

        // For the AABBs
        //if(pTheGO->isDebugAABBActive)
        //{
        //    if(pTheGO->typeOfObject == SPHERE)
        //    {
        //        // Calculate all AABBs for the sphere
        //        // Put the sphere inside an axis-aligned box

        //        // Vertices
        //        float diameter = pTheGO->radius * 2;
        //        std::vector<glm::vec3> vertices;
        //        glm::vec3 vertex0 = glm::vec3(pTheGO->position - pTheGO->radius);
        //        vertices.push_back(vertex0);
        //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y, vertex0.z + diameter));
        //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y, vertex0.z + diameter));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + diameter, vertex0.z + diameter));
        //        vertices.push_back(glm::vec3(vertex0.x + diameter, vertex0.y + diameter, vertex0.z + diameter));

        //        DrawAABBforPoints(vertices, g_AABBSize);
        //    } 
        //    else if(pTheGO->typeOfObject == MESH)
        //    {
        //        // Calculate all AABBs for the mesh
        //        // Put the mesh inside an axis-aligned box

        //        // Take the mesh extents
        //        cMesh theMesh;
        //        if (!g_pVAOManager->lookupMeshFromName(pTheGO->meshName, theMesh))
        //        {
        //            // Can't find the mesh
        //            continue;
        //        }

        //        float extent = theMesh.maxExtent;

        //        // Vertices
        //        std::vector<glm::vec3> vertices;
        //        glm::vec3 vertex0(0.0f);
        //        vertex0 = pTheGO->position - (extent / 2.0f);
        //        vertices.push_back(vertex0);
        //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + extent, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y + extent, vertex0.z));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y, vertex0.z + extent));
        //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y, vertex0.z + extent));
        //        vertices.push_back(glm::vec3(vertex0.x, vertex0.y + extent, vertex0.z + extent));
        //        vertices.push_back(glm::vec3(vertex0.x + extent, vertex0.y + extent, vertex0.z + extent));

        //        DrawAABBforPoints(vertices, g_AABBSize);
        //    }
        //    else
        //    {
        //        DrawAABB(pTheGO, g_AABBSize);
        //    }
        //}

    }

    // Now Draw the transparent objects
    ::g_pTranspManager->sortObjects();
    int numTransObjects = ::g_pTranspManager->transpObjects.size();
    for(int i = 0; i < numTransObjects; i++)
    {
        if(::g_pTranspManager->transpObjects[i]->rotateToCamera)
        {
            // Orient the cloud to the camera
            turnGameObjectToCamera(::g_pTranspManager->transpObjects[i], g_pCamera->getCameraPosition());
        }
        
        DrawObject(::g_pTranspManager->transpObjects[i]);

        //// For the AABBs
        //if(::g_pTranspManager->transpObjects[i]->isDebugAABBActive)
        //{
        //    DrawAABB(::g_pTranspManager->transpObjects[i], g_AABBSize);
        //}
    }

    ////// Draw localization text ---------------------------------------------
    //g_lococalization.draw(width, height);
    //g_textManager.draw(width, height);

    //::g_pDebugRenderer->RenderDebugObjects(matView, matProjection, deltaTime);

    // "Draw scene" loop end
    //---------------------------------------------------------------------
}
