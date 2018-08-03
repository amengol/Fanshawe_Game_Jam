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

// Skinned Mesh                                                             
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
        // Do this movements only once:
        if (pTheGO->characterAnim == eCharacterAnim::JUMP)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }
        if (pTheGO->characterAnim == eCharacterAnim::JUMP_FORWARD_WALKING)
        {
            pAniState->activeAnimation.name = pTheGO->animations.walking;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.walking);
            pTheGO->characterAnim = eCharacterAnim::WALKING;
        }
        if (pTheGO->characterAnim == eCharacterAnim::JUMP_FORWARD)
        {
            pAniState->activeAnimation.name = pTheGO->animations.running;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.running);
            pTheGO->characterAnim = eCharacterAnim::RUN_FORWARD;
        }
        if (pTheGO->characterAnim == eCharacterAnim::ATTACK_01)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }
        if (pTheGO->characterAnim == eCharacterAnim::ATTACK_02)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }
        if (pTheGO->characterAnim == eCharacterAnim::ATTACK_03)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }
        if (pTheGO->characterAnim == eCharacterAnim::TURN_LEFT_180)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
        }
        if (pTheGO->characterAnim == eCharacterAnim::TURN_RIGHT_180)
        {
            pAniState->activeAnimation.name = pTheGO->animations.idle;
            pAniState->activeAnimation.totalTime =
                pTheGO->pSimpleSkinnedMesh->GetAnimationDuration(pTheGO->animations.idle);
            pTheGO->characterAnim = eCharacterAnim::IDLE;
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
                                              animationToPlay,		        //**NEW**
                                              vecFinalTransformation,		// Final bone transforms for mesh
                                              vecObjectBoneTransformation,  // final location of bones
                                              vecOffsets);                  // local offset for each bone

    //=========================================================================
    // Props update
    std::map<std::string, cGameObject*>::iterator itGO = pTheGO->mMapBoneNameTOProp.begin();

    for (; itGO != pTheGO->mMapBoneNameTOProp.end(); itGO++)
    {
        std::string boneName = itGO->first;
        cGameObject* prop = itGO->second;

        if (g_propsEnabled)
        {
            std::map<std::string, glm::mat4>::iterator itTrans =
                pTheGO->pSimpleSkinnedMesh->mMapBoneToLastLocalTransformation.find(boneName);

            if (itTrans != pTheGO->pSimpleSkinnedMesh->mMapBoneToLastLocalTransformation.end())
            {

                prop->rigidBody->SetMatOrientation(pTheGO->orientation * itTrans->second);
                glm::vec3 offset = pTheGO->orientation * itTrans->second * glm::vec4(1.0f);
                prop->rigidBody->SetPosition(offset * pTheGO->scale + pTheGO->position);
            }

            if (prop->rigidBody->IsInWorld())
            {
                g_pPhysicsWorld->RemoveRigidBody(prop->rigidBody);
            }
        }
        else
        {
            if (!prop->rigidBody->IsInWorld())
            {
                g_pPhysicsWorld->AddRigidBody(prop->rigidBody);
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
    if (::g_pVaoManager->lookupStaticVAOsFromName(meshToDraw, vecVAODrawInfo) == false)
    {	// Didn't find mesh
        return;
    }

    // Skinned Mesh                                                    
    if (pTheGO->pSimpleSkinnedMesh)
    {
        // Calculate the pose and load the skinned mesh stuff into the shader, too
        CalculateSkinnedMeshBonesAndLoad(pTheGO, g_uniLocHandler.numBonesUsed, g_uniLocHandler.bones);

        glUniform1f(g_uniLocHandler.bIsASkinnedMesh, GL_TRUE);
    }
    else
    {
        glUniform1f(g_uniLocHandler.bIsASkinnedMesh, GL_FALSE);
    }

    // Model View
    glm::mat4x4 mModel = glm::mat4x4(1.0f);
    glm::mat4 trans = glm::mat4x4(1.0f);
    // Position by nPhysics?
    if (pTheGO->rigidBody != NULL)
    {
        pTheGO->rigidBody->GetPostion(pTheGO->position);

        // Make an offset value
        if (pTheGO->pSimpleSkinnedMesh)
        {
            pTheGO->position.y -= pTheGO->rigidBody->GetShape()->GetCapsuleHalfHeight() +
                pTheGO->rigidBody->GetShape()->GetCapsuleRadius();
        }

        trans = glm::translate(trans, pTheGO->position);
    }
    else
    {
        trans = glm::translate(trans, pTheGO->position);
    }
    mModel = mModel * trans;
    // Orientation by nPhysics?
    if (pTheGO->rigidBody != NULL)
    {
        pTheGO->rigidBody->GetMatOrientation(pTheGO->orientation);
        mModel = mModel * pTheGO->orientation;

        // Update this again here to avoid camera jittrring
        if (pTheGO->pSimpleSkinnedMesh)
        {
            g_camera.updateCameraVectors();
        }
    }
    else
    {
        mModel = mModel * pTheGO->orientation;
    }
    float finalScale = pTheGO->scale;
    glm::mat4 matScale = glm::mat4x4(1.0f);
    matScale = glm::scale(matScale, glm::vec3(finalScale, finalScale, finalScale));
    mModel = mModel * matScale;
    glUniformMatrix4fv(g_uniLocHandler.mModel, 1, GL_FALSE, &mModel[0][0]);
    glm::mat4 mWorldInvTranpose = glm::inverse(glm::transpose(mModel));
    glUniformMatrix4fv(g_uniLocHandler.mWorldInvTrans, 1, GL_FALSE, &mWorldInvTranpose[0][0]);

    // Diffuse Ratio
    glUniform1f(g_uniLocHandler.ambientToDiffuseRatio, 0.3f);

    // Specular
    glUniform4f(g_uniLocHandler.materialSpecular,
                pTheGO->specular.x,
                pTheGO->specular.y,
                pTheGO->specular.z,
                pTheGO->specular.w);

    // Material Diffuse
    glUniform4f(g_uniLocHandler.materialDiffuse,
                pTheGO->diffuseColour.r,
                pTheGO->diffuseColour.g,
                pTheGO->diffuseColour.b,
                pTheGO->diffuseColour.a);

    // Fog parameters
    if (g_environment.m_fogActive)
    {
        glUniform1f(g_uniLocHandler.fogActive, 1.0f);

        // Fog colour
        glUniform3f(g_uniLocHandler.fogColour,
                    g_environment.m_fogColour.r,
                    g_environment.m_fogColour.g,
                    g_environment.m_fogColour.b);

        // Fog percent
        glUniform1f(g_uniLocHandler.fogPercent, g_environment.m_fogPercent);
    }
    else
    {
        glUniform1f(g_uniLocHandler.fogActive, 0.0f);
    }

    // Multi layer texture object?
    if (pTheGO->hasMultiLayerTextures)
    {
        glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_TRUE);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasMultiLayerTextures, GL_FALSE);
    }

    // Other uniforms
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
    if (pTheGO->bIsWireFrame)
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 0.0f);
    }
    if (pTheGO->hasReflection)
    {
        glUniform1f(g_uniLocHandler.hasReflection, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasReflection, 0.0f);
    }
    if (pTheGO->receiveShadow)
    {
        glUniform1f(g_uniLocHandler.receiveShadow, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.receiveShadow, 0.0f);
    }
    if (pTheGO->selfLight)
    {
        glUniform1f(g_uniLocHandler.selfLight, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.selfLight, 0.0f);
    }
    if (pTheGO->hasNormalMap)
    {
        glUniform1f(g_uniLocHandler.hasNormalMap, 1.0f);
    }
    else
    {
        glUniform1f(g_uniLocHandler.hasNormalMap, 0.0f);
    }

    // Set up cube map...
    GLuint cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("morning");
    glActiveTexture(GL_TEXTURE27);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 27);
    cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("day");
    glActiveTexture(GL_TEXTURE28);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 28);
    cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("sunset");
    glActiveTexture(GL_TEXTURE29);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 29);
    cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("night");
    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 20);
    cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("deep_night");
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
    glUniform1i(g_uniLocHandler.texSampCube00_LocID, 21);

    // Skyboxes blend values
    glUniform1f(g_uniLocHandler.texCubeBlend00_LocID, g_environment.m_dawn);
    glUniform1f(g_uniLocHandler.texCubeBlend01_LocID, g_environment.m_noon);
    glUniform1f(g_uniLocHandler.texCubeBlend02_LocID, g_environment.m_sunset);
    glUniform1f(g_uniLocHandler.texCubeBlend03_LocID, g_environment.m_night);
    glUniform1f(g_uniLocHandler.texCubeBlend04_LocID, g_environment.m_midNight);

    // Set up regular textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[0]));
    glUniform1i(g_uniLocHandler.textSampler00_ID, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[1]));
    glUniform1i(g_uniLocHandler.textSampler01_ID, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[2]));
    glUniform1i(g_uniLocHandler.textSampler02_ID, 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[3]));
    glUniform1i(g_uniLocHandler.textSampler03_ID, 3);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[4]));
    glUniform1i(g_uniLocHandler.textSampler04_ID, 4);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[5]));
    glUniform1i(g_uniLocHandler.textSampler05_ID, 5);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[6]));
    glUniform1i(g_uniLocHandler.textSampler06_ID, 6);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D,
                  g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[7]));
    glUniform1i(g_uniLocHandler.textSampler07_ID, 7);

    // And their blending values
    glUniform1f(g_uniLocHandler.textBlend00_ID, pTheGO->textureBlend[0]);
    glUniform1f(g_uniLocHandler.textBlend01_ID, pTheGO->textureBlend[1]);
    glUniform1f(g_uniLocHandler.textBlend02_ID, pTheGO->textureBlend[2]);
    glUniform1f(g_uniLocHandler.textBlend03_ID, pTheGO->textureBlend[3]);
    glUniform1f(g_uniLocHandler.textBlend04_ID, pTheGO->textureBlend[4]);
    glUniform1f(g_uniLocHandler.textBlend05_ID, pTheGO->textureBlend[5]);
    glUniform1f(g_uniLocHandler.textBlend06_ID, pTheGO->textureBlend[6]);
    glUniform1f(g_uniLocHandler.textBlend07_ID, pTheGO->textureBlend[7]);

    // Draw mode
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

    // Draw it (or them)
    for (size_t i = 0; i < vecVAODrawInfo.size(); i++)
    {
        glBindVertexArray(vecVAODrawInfo[i].VAO_ID);
        glDrawElements(GL_TRIANGLES, vecVAODrawInfo[i].numberOfIndices, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);

    return;
}

void RenderScene(std::vector<cGameObject*>& vec_pGOs, 
                 GLFWwindow* pGLFWWindow, 
                 cCamera& camera, 
                 double deltaTime)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Pass the GamaCorrection
    glUniform1f(g_uniLocHandler.gamaCorrection, g_gamaCorrection);

    // Update all the light uniforms...
    g_pLightManager->CopyLightInformationToCurrentShader();

    // Projection
    glm::mat4x4 matProjection;
    matProjection = glm::perspective(glm::radians(g_camera.m_zoom),             // FOV
                                     (float)g_scrWidth / (float)g_scrHeight,    // Aspect ratio
                                     0.1f,		   	                            // Near (as big as possible)
                                     2000.0f);                                  // Far (as small as possible)
    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, &matProjection[0][0]);

    // "Draw scene" loop
    unsigned int sizeOfVector = (unsigned int)vec_pGOs.size();
    for (int index = 0; index != sizeOfVector; index++)
    {
        cGameObject* pTheGO = vec_pGOs[index];

        DrawObject(pTheGO);

    }

    // Now Draw the transparent objects
    g_pTranspManager->sortObjects();
    int numTransObjects = g_pTranspManager->transpObjects.size();
    for(int i = 0; i < numTransObjects; i++)
    {
        if(::g_pTranspManager->transpObjects[i]->rotateToCamera)
        {
            // Orient the cloud to the camera
            turnGameObjectToCamera(::g_pTranspManager->transpObjects[i], camera.m_position, false);
        }
        DrawObject(::g_pTranspManager->transpObjects[i]);
    }
}

void RenderScene(cGameObject* pTheGO, 
                 GLFWwindow* pGLFWWindow, 
                 cCamera& camera, 
                 double deltaTime)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Pass the GamaCorrection
    glUniform1f(g_uniLocHandler.gamaCorrection, g_gamaCorrection);

    // Update all the light uniforms...
    ::g_pLightManager->CopyLightInformationToCurrentShader();

    // Projection
    glm::mat4x4 matProjection;
    matProjection = glm::perspective(glm::radians(g_camera.m_zoom),             // FOV
                                    (float)g_scrWidth / (float)g_scrHeight,	    // Aspect ratio
                                     0.1f,		   	                            // Near (as big as possible)
                                     2000.0f);                                  // Far (as small as possible)
    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, &matProjection[0][0]);

    DrawObject(pTheGO);
}

void RenderShadow(std::vector<cGameObject*>& vec_pGOs, unsigned int shaderID)
{
    // Projection
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 200.0f;
    lightProjection = glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, near_plane, far_plane);
    float timeOfDay = g_environment.getTimeOfDay();
    if (timeOfDay >= 6.0f && timeOfDay <= 18.0f)
    {
        lightView = glm::lookAt(g_environment.getSunPosition() * 0.25f, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    }
    else
    {
        lightView = glm::lookAt(g_environment.getMoonPosition() * 0.25f, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    }
    lightSpaceMatrix = lightProjection * lightView;
    glUniformMatrix4fv(g_uniLocHandler.mView, 1, GL_FALSE, &lightView[0][0]);
    glUniformMatrix4fv(g_uniLocHandler.mProjection, 1, GL_FALSE, &lightProjection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

    // Draw the Game Objects
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

        std::string meshToDraw = pTheGO->meshName;
        std::vector<sVAOInfo> vecVAODrawInfo;
        if (::g_pVaoManager->lookupStaticVAOsFromName(meshToDraw, vecVAODrawInfo) == false)
        {	// Didn't find mesh
            return;
        }

        // Skinned Mesh                                                            
        if (pTheGO->pSimpleSkinnedMesh)
        {
            // Calculate the pose and load the skinned mesh stuff into the shader, too
            CalculateSkinnedMeshBonesAndLoad(pTheGO, g_uniLocHandler.numBonesUsed, g_uniLocHandler.bones);
            glUniform1f(g_uniLocHandler.bIsASkinnedMesh, GL_TRUE);
        }
        else
        {
            glUniform1f(g_uniLocHandler.bIsASkinnedMesh, GL_FALSE);
        }

        // Model View
        glm::mat4x4 mModel = glm::mat4x4(1.0f);
        glm::mat4 trans = glm::mat4x4(1.0f);
        // Position by nPhysics?
        if (pTheGO->rigidBody != NULL)
        {
            pTheGO->rigidBody->GetPostion(pTheGO->position);

            // Make an offset value
            if (pTheGO->pSimpleSkinnedMesh)
            {
                pTheGO->position.y -= pTheGO->rigidBody->GetShape()->GetCapsuleHalfHeight() +
                    pTheGO->rigidBody->GetShape()->GetCapsuleRadius();
            }

            trans = glm::translate(trans, pTheGO->position);
        }
        else
        {
            trans = glm::translate(trans, pTheGO->position);
        }
        mModel = mModel * trans;
        // Orientation by nPhysics?
        if (pTheGO->rigidBody != NULL)
        {
            pTheGO->rigidBody->GetMatOrientation(pTheGO->orientation);
            mModel = mModel * pTheGO->orientation;
           
            // Update this again here to avoid camera jittrring
            if (pTheGO->pSimpleSkinnedMesh)
            {
                g_camera.updateCameraVectors();
            }
        }
        else
        {
            mModel = mModel * pTheGO->orientation;
        }
        float finalScale = pTheGO->scale;
        glm::mat4 matScale = glm::mat4x4(1.0f);
        matScale = glm::scale(matScale, glm::vec3(finalScale, finalScale, finalScale));
        mModel = mModel * matScale;
        glUniformMatrix4fv(g_uniLocHandler.mModel, 1, GL_FALSE, &mModel[0][0]);

        glUniform1f(g_uniLocHandler.hasColour, 0.0f);
        if (pTheGO->hasAlpha)
        {
            glUniform1f(g_uniLocHandler.hasAlpha, 1.0f);
        }
        else
        {
            glUniform1f(g_uniLocHandler.hasAlpha, 0.0f);
        }
        glUniform1f(g_uniLocHandler.useDiscardAlpha, 0.0f);
        glUniform1f(g_uniLocHandler.bIsDebugWireFrameObject, 0.0f);
        glUniform1f(g_uniLocHandler.hasReflection, 0.0f);
        glUniform1f(g_uniLocHandler.hasMultiLayerTextures, 0.0f);

        // Set up cube map...
        GLuint cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("morning");
        glActiveTexture(GL_TEXTURE27);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
        glUniform1i(g_uniLocHandler.texSampCube00_LocID, 27);
        cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("day");
        glActiveTexture(GL_TEXTURE28);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
        glUniform1i(g_uniLocHandler.texSampCube01_LocID, 28);
        cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("sunset");
        glActiveTexture(GL_TEXTURE29);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
        glUniform1i(g_uniLocHandler.texSampCube02_LocID, 29);
        cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("night");
        glActiveTexture(GL_TEXTURE30);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
        glUniform1i(g_uniLocHandler.texSampCube03_LocID, 30);
        cubeMapNumber = g_pTextureManager->getTextureIDFromTextureName("deep_night");
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNumber);
        glUniform1i(g_uniLocHandler.texSampCube04_LocID, 31);

        glUniform1f(g_uniLocHandler.texCubeBlend00_LocID, g_environment.m_dawn);
        glUniform1f(g_uniLocHandler.texCubeBlend01_LocID, g_environment.m_noon);
        glUniform1f(g_uniLocHandler.texCubeBlend02_LocID, g_environment.m_sunset);
        glUniform1f(g_uniLocHandler.texCubeBlend03_LocID, g_environment.m_night);
        glUniform1f(g_uniLocHandler.texCubeBlend04_LocID, g_environment.m_midNight);

        // Regulat textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[0]));
        glUniform1i(g_uniLocHandler.textSampler00_ID, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[1]));
        glUniform1i(g_uniLocHandler.textSampler01_ID, 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[2]));
        glUniform1i(g_uniLocHandler.textSampler02_ID, 2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[3]));
        glUniform1i(g_uniLocHandler.textSampler03_ID, 3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[4]));
        glUniform1i(g_uniLocHandler.textSampler04_ID, 4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[5]));
        glUniform1i(g_uniLocHandler.textSampler05_ID, 5);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[6]));
        glUniform1i(g_uniLocHandler.textSampler06_ID, 6);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D,
                      g_pTextureManager->getTextureIDFromTextureName(pTheGO->textureNames[7]));
        glUniform1i(g_uniLocHandler.textSampler07_ID, 7);
        
        // And their blending values
        glUniform1f(g_uniLocHandler.textBlend00_ID, pTheGO->textureBlend[0]);
        glUniform1f(g_uniLocHandler.textBlend01_ID, pTheGO->textureBlend[1]);
        glUniform1f(g_uniLocHandler.textBlend02_ID, pTheGO->textureBlend[2]);
        glUniform1f(g_uniLocHandler.textBlend03_ID, pTheGO->textureBlend[3]);
        glUniform1f(g_uniLocHandler.textBlend04_ID, pTheGO->textureBlend[4]);
        glUniform1f(g_uniLocHandler.textBlend05_ID, pTheGO->textureBlend[5]);
        glUniform1f(g_uniLocHandler.textBlend06_ID, pTheGO->textureBlend[6]);
        glUniform1f(g_uniLocHandler.textBlend07_ID, pTheGO->textureBlend[7]);

        // Draw mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        for (size_t i = 0; i < vecVAODrawInfo.size(); i++)
        {
            glBindVertexArray(vecVAODrawInfo[i].VAO_ID);
            glDrawElements(GL_TRIANGLES, vecVAODrawInfo[i].numberOfIndices, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }
}
