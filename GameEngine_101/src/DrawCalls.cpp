#include "DrawCalls.h"
#include "cGameObject.h"
#include "globalOpenGL_GLFW.h"
#include <glm/gtc/type_ptr.hpp>
#include "globalGameStuff.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Assimp\cAnimationState.h"
#include "AI\cCharacterControl.h"
#include "Utilities.h"

bool g_clothDebug = false;
float g_gamaCorrection = 1.5f;

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

    // Skybox update
    if (pTheGO->typeOfObject == SKYBOX)
    {
        pTheGO->position = g_camera.m_position;
        glUniform1f(g_uniLocHandler.isASkyBox, GL_TRUE);
    }
    else
    {
        glUniform1f(g_uniLocHandler.isASkyBox, GL_FALSE);
    }

    // Eye Position
    glm::vec3 eyePos = g_camera.m_position;
    glUniform3f(g_uniLocHandler.eyePosition, eyePos.x, eyePos.y, eyePos.z);

    // Update lights
    if (pTheGO->hasLights)
    {
        for (int i = 0; i < pTheGO->vecLightsInfo.size(); i++)
        {
            lightInfo lightInfo = pTheGO->vecLightsInfo[i];
            glm::vec3 rotatedOffset = pTheGO->orientation * glm::vec4(lightInfo.offset, 0.0f);
            g_pLightManager->vecLights[lightInfo.index]->position = pTheGO->position + rotatedOffset;
            // Update directly to not get a delay
            glUniform4f(g_pLightManager->vecLights[lightInfo.index]->shaderlocID_position,
                        g_pLightManager->vecLights[lightInfo.index]->position.x,
                        g_pLightManager->vecLights[lightInfo.index]->position.y,
                        g_pLightManager->vecLights[lightInfo.index]->position.z,
                        1.0f);
            if (lightInfo.type == SPOT)
            {
                glm::vec3 rotatedFocus = pTheGO->orientation * glm::vec4(lightInfo.focusDirection, 0.0f);
                g_pLightManager->vecLights[lightInfo.index]->direction = rotatedFocus;
                // Update directly to not get a delay
                glUniform4f(g_pLightManager->vecLights[lightInfo.index]->shaderlocID_direction,
                            g_pLightManager->vecLights[lightInfo.index]->direction.x,
                            g_pLightManager->vecLights[lightInfo.index]->direction.y,
                            g_pLightManager->vecLights[lightInfo.index]->direction.z,
                            1.0f);
            }
        }
    }

    // Was near the draw call, but we need the mesh name
    std::string meshToDraw = pTheGO->meshName;


    std::vector<sVAOInfo> vecVAODrawInfo;
    if (::g_pVAOManager->lookupStaticVAOsFromName(meshToDraw, vecVAODrawInfo) == false)
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

    if (pTheGO->hasReflection)
    {
        glUniform1f(g_uniLocHandler.hasReflection, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasReflection, 0.0f);
    }

    // Set up cube map...
    GLuint cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("morning");
    glActiveTexture(GL_TEXTURE27);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

    cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("day");
    glActiveTexture(GL_TEXTURE28);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

    cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("sunset");
    glActiveTexture(GL_TEXTURE29);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

    cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("night");
    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

    cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("deep_night");
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

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

    // 3
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[3]));

    // 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[4]));

    // 5
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[5]));

    // 6
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[6]));

    // 7
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D,
                  ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[7]));

    // Multi layer texture object?
    if (pTheGO->hasMultiLayerTextures)
    {
        glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_TRUE);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_FALSE);
    }

    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 27);
    glUniform1i(g_uniLocHandler.texSampCube01_LocID, 28);
    glUniform1i(g_uniLocHandler.texSampCube02_LocID, 29);
    glUniform1i(g_uniLocHandler.texSampCube03_LocID, 30);
    glUniform1i(g_uniLocHandler.texSampCube04_LocID, 31);

    glUniform1f(g_uniLocHandler.texCubeBlend00_LocID, g_environment.m_dawn->typeParams2.x);
    glUniform1f(g_uniLocHandler.texCubeBlend01_LocID, g_environment.m_noon->typeParams2.x);
    glUniform1f(g_uniLocHandler.texCubeBlend02_LocID, g_environment.m_sunset->typeParams2.x);
    glUniform1f(g_uniLocHandler.texCubeBlend03_LocID, g_environment.m_night->typeParams2.x);
    glUniform1f(g_uniLocHandler.texCubeBlend04_LocID, g_environment.m_midNight->typeParams2.x);

    // This connects the texture sampler to the texture units... 
    glUniform1i(g_uniLocHandler.textSampler00_ID, 0);
    glUniform1i(g_uniLocHandler.textSampler01_ID, 1);
    glUniform1i(g_uniLocHandler.textSampler02_ID, 2);
    glUniform1i(g_uniLocHandler.textSampler03_ID, 3);
    glUniform1i(g_uniLocHandler.textSampler04_ID, 4);
    glUniform1i(g_uniLocHandler.textSampler05_ID, 5);
    glUniform1i(g_uniLocHandler.textSampler06_ID, 6);
    glUniform1i(g_uniLocHandler.textSampler07_ID, 7);
    // .. and so on

    // And the blending values
    glUniform1f(g_uniLocHandler.textBlend00_ID, pTheGO->textureBlend[0]);
    glUniform1f(g_uniLocHandler.textBlend01_ID, pTheGO->textureBlend[1]);
    glUniform1f(g_uniLocHandler.textBlend02_ID, pTheGO->textureBlend[2]);
    glUniform1f(g_uniLocHandler.textBlend03_ID, pTheGO->textureBlend[3]);
    glUniform1f(g_uniLocHandler.textBlend04_ID, pTheGO->textureBlend[4]);
    glUniform1f(g_uniLocHandler.textBlend05_ID, pTheGO->textureBlend[5]);
    glUniform1f(g_uniLocHandler.textBlend06_ID, pTheGO->textureBlend[6]);
    glUniform1f(g_uniLocHandler.textBlend07_ID, pTheGO->textureBlend[7]);
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
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
    }

    glCullFace(GL_BACK);


    for (size_t i = 0; i < vecVAODrawInfo.size(); i++)
    {
        glBindVertexArray(vecVAODrawInfo[i].VAO_ID);
        glDrawElements(GL_TRIANGLES, vecVAODrawInfo[i].numberOfIndices, GL_UNSIGNED_INT, 0);
    }
    
    // Unbind that VAO
    glBindVertexArray(0);

    return;
}

void RenderScene(std::vector<cGameObject*>& vec_pGOs, 
                 GLFWwindow* pGLFWWindow, 
                 cCamera& camera, 
                 double deltaTime,
                 cGameObject* skyBox)
{

    // Clear colour AND depth buffer
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Pass the GamaCorrection
    glUniform1f(g_uniLocHandler.gamaCorrection, g_gamaCorrection);

    ::g_pShaderManager->useShaderProgram("GE101_Shader");
    GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

    // Update all the light uniforms...
    // (for the whole scene)
    ::g_pLightManager->CopyLightInformationToCurrentShader();

    //---------------------------------------------------------------------
    // Camera block

    glm::mat4x4 matProjection;

    // Projection and view don't change per scene (maybe)
    matProjection = glm::perspective(glm::radians(g_camera.m_zoom),             // FOV
                                     (float)g_scrWidth / (float)g_scrHeight,	// Aspect ratio
                                     0.1f,		   	                            // Near (as big as possible)
                                     2000.0f);                                // Far (as small as possible)

    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(matProjection));

    //---------------------------------------------------------------------
    // "Draw scene" loop

    unsigned int sizeOfVector = (unsigned int)vec_pGOs.size();
    for (int index = 0; index != sizeOfVector; index++)
    {
        cGameObject* pTheGO = vec_pGOs[index];

        DrawObject(pTheGO);

    }

    // Lastly, draw the SkyBox
    if (skyBox != NULL)
    {
        DrawObject(skyBox);
    }

    // Now Draw the transparent objects
    ::g_pTranspManager->sortObjects();
    int numTransObjects = ::g_pTranspManager->transpObjects.size();
    for(int i = 0; i < numTransObjects; i++)
    {
        if(::g_pTranspManager->transpObjects[i]->rotateToCamera)
        {
            // Orient the cloud to the camera
            turnGameObjectToCamera(::g_pTranspManager->transpObjects[i], g_camera.m_position);
        }
        
        DrawObject(::g_pTranspManager->transpObjects[i]);

    }
}

void RenderScene(std::vector<cGameObject*>& vec_pGOs, unsigned int shaderID)
{

    //---------------------------------------------------------------------
    // Camera block

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 2000.0f;
    lightProjection = glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, near_plane, far_plane);
    lightView = glm::lookAt(glm::vec3(-50.0f, 50.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, &lightView[0][0]);
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, &lightProjection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);


    for (size_t i = 0; i < vec_pGOs.size(); i++)
    {
        cGameObject* pTheGO = vec_pGOs[i];


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

        glUniform1f(g_uniLocHandler.isASkyBox, GL_FALSE);

        // Eye Position
        glm::vec3 eyePos = g_camera.m_position;
        glUniform3f(g_uniLocHandler.eyePosition, eyePos.x, eyePos.y, eyePos.z);

        // Was near the draw call, but we need the mesh name
        std::string meshToDraw = pTheGO->meshName;


        std::vector<sVAOInfo> vecVAODrawInfo;
        if (::g_pVAOManager->lookupStaticVAOsFromName(meshToDraw, vecVAODrawInfo) == false)
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

        glUniform1f(g_uniLocHandler.hasColour, 0.0f);
        glUniform1f(g_uniLocHandler.hasAlpha, 0.0f);
        glUniform1f(g_uniLocHandler.useDiscardAlpha, 0.0f);
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 0.0f);
        glUniform1f(g_uniLocHandler.hasReflection, 0.0f);

        // Set up cube map...
        GLuint cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("morning");
        glActiveTexture(GL_TEXTURE27);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

        cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("day");
        glActiveTexture(GL_TEXTURE28);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

        cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("sunset");
        glActiveTexture(GL_TEXTURE29);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

        cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("night");
        glActiveTexture(GL_TEXTURE30);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

        cubeMapNumber = ::g_pTextureManager->getTextureIDFromTextureName("deep_night");
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);

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

        // 3
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,
                      ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[3]));

        // 4
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,
                      ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[4]));

        // 5
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D,
                      ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[5]));

        // 6
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D,
                      ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[6]));

        // 7
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D,
                      ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[7]));

        // Multi layer texture object?
        if (pTheGO->hasMultiLayerTextures)
        {
            glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_TRUE);
        }
        else
        {
            glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_FALSE);
        }

        glUniform1i(g_uniLocHandler.texSampCube00_LocID, 27);
        glUniform1i(g_uniLocHandler.texSampCube01_LocID, 28);
        glUniform1i(g_uniLocHandler.texSampCube02_LocID, 29);
        glUniform1i(g_uniLocHandler.texSampCube03_LocID, 30);
        glUniform1i(g_uniLocHandler.texSampCube04_LocID, 31);

        glUniform1f(g_uniLocHandler.texCubeBlend00_LocID, g_environment.m_dawn->typeParams2.x);
        glUniform1f(g_uniLocHandler.texCubeBlend01_LocID, g_environment.m_noon->typeParams2.x);
        glUniform1f(g_uniLocHandler.texCubeBlend02_LocID, g_environment.m_sunset->typeParams2.x);
        glUniform1f(g_uniLocHandler.texCubeBlend03_LocID, g_environment.m_night->typeParams2.x);
        glUniform1f(g_uniLocHandler.texCubeBlend04_LocID, g_environment.m_midNight->typeParams2.x);

        // This connects the texture sampler to the texture units... 
        glUniform1i(g_uniLocHandler.textSampler00_ID, 0);
        glUniform1i(g_uniLocHandler.textSampler01_ID, 1);
        glUniform1i(g_uniLocHandler.textSampler02_ID, 2);
        glUniform1i(g_uniLocHandler.textSampler03_ID, 3);
        glUniform1i(g_uniLocHandler.textSampler04_ID, 4);
        glUniform1i(g_uniLocHandler.textSampler05_ID, 5);
        glUniform1i(g_uniLocHandler.textSampler06_ID, 6);
        glUniform1i(g_uniLocHandler.textSampler07_ID, 7);
        // .. and so on

        // And the blending values
        glUniform1f(g_uniLocHandler.textBlend00_ID, pTheGO->textureBlend[0]);
        glUniform1f(g_uniLocHandler.textBlend01_ID, pTheGO->textureBlend[1]);
        glUniform1f(g_uniLocHandler.textBlend02_ID, pTheGO->textureBlend[2]);
        glUniform1f(g_uniLocHandler.textBlend03_ID, pTheGO->textureBlend[3]);
        glUniform1f(g_uniLocHandler.textBlend04_ID, pTheGO->textureBlend[4]);
        glUniform1f(g_uniLocHandler.textBlend05_ID, pTheGO->textureBlend[5]);
        glUniform1f(g_uniLocHandler.textBlend06_ID, pTheGO->textureBlend[6]);
        glUniform1f(g_uniLocHandler.textBlend07_ID, pTheGO->textureBlend[7]);
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
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_CULL_FACE);
        }

        glCullFace(GL_BACK);


        for (size_t i = 0; i < vecVAODrawInfo.size(); i++)
        {
            glBindVertexArray(vecVAODrawInfo[i].VAO_ID);
            glDrawElements(GL_TRIANGLES, vecVAODrawInfo[i].numberOfIndices, GL_UNSIGNED_INT, 0);
        }

        // Unbind that VAO
        glBindVertexArray(0);
    }
}
