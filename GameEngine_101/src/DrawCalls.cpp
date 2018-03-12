#include "DrawCalls.h"
#include "cGameObject.h"
#include <cSoftBody.h>
#include <cCloth.h>
#include "globalOpenGL_GLFW.h"
#include <glm/gtc/type_ptr.hpp>
#include "globalGameStuff.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Assimp\cAnimationState.h"

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

    // Are there any animations in the queue of animations
    if (!pAniState->vecAnimationQueue.empty())
    {
        // Play the "1st" animation in the queue 
        animationToPlay = pAniState->vecAnimationQueue[0].name;
        curFrameTime = pAniState->vecAnimationQueue[0].currentTime;
        float deltaTime = glfwGetTime() - pAniState->vecAnimationQueue[0].currentClockTime;

        // Increment the top animation in the queue
        if (pAniState->vecAnimationQueue[0].IncrementTime(deltaTime))
        {
            // The animation reset to zero on increment...
            // ...meaning that the 1st animation is done
            // (WHAT!? Should you use a vector for this???)
            pAniState->vecAnimationQueue.erase(pAniState->vecAnimationQueue.begin());

        }//vecAnimationQueue[0].IncrementTime()

        pAniState->vecAnimationQueue[0].currentClockTime = glfwGetTime();
    }
    else
    {	// Use the default animation.
        float deltaTime = glfwGetTime() - pAniState->defaultAnimation.currentClockTime;

        // Update the next position
        if (pAniState->defaultAnimation.IncrementTime(deltaTime))
        {
            // If you had to loop the animation, the new start position is goind
            // to be the last hip position
            pTheGO->pSimpleSkinnedMesh->mStartHipPosition = 
                pTheGO->pSimpleSkinnedMesh->mLastHipPosition;
        }
        // Rotate the start position
        glm::vec3 rotatedStartPos = pTheGO->orientation * glm::vec4(pTheGO->pSimpleSkinnedMesh->mStartHipPosition, 0.0f);
        pTheGO->position = rotatedStartPos * pTheGO->scale;

        // Project the root to the ground level
        pTheGO->position.y = 0.0f;
        

        animationToPlay = pAniState->defaultAnimation.name;
        curFrameTime = pAniState->defaultAnimation.currentTime;
        pAniState->defaultAnimation.currentClockTime = glfwGetTime();

    }//if ( pAniState->vecAnimationQueue.empty()

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

    unsigned int numberOfBonesUsed = static_cast< unsigned int >(vecFinalTransformation.size());
    glUniform1i(UniformLoc_numBonesUsed, numberOfBonesUsed);

    glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);
    // UniformLoc_bonesArray is the getUniformLoc of "bones[0]" from
    //	uniform mat4 bones[MAXNUMBEROFBONES] 
    // in the shader
    glUniformMatrix4fv(UniformLoc_bonesArray, numberOfBonesUsed, GL_FALSE,
        (const GLfloat*)glm::value_ptr(*pBoneMatrixArray));


    //	//glUniform1i( UniformLoc_bIsASkinnedMesh, FALSE );
    //	// Draw all the bones
    //	for ( unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; boneIndex++ )
    //	{
    //		glm::mat4 boneLocal = vecObjectBoneTransformation[boneIndex];
    //
    //		cPhysicalProperties phyProps;
    //		pTheGO->GetPhysState( phyProps );
    //
    //		float scale = pTheGO->vecMeshes;
    //		boneLocal = glm::scale( boneLocal, glm::vec3(scale, scale, scale) );
    //
    //		cPhysicalProperties phyProps;
    //		pTheGO->GetPhysState( phyProps );
    //		glm::vec4 GameObjectlocation = glm::vec4( phyProps.position, 1.0f );
    //
    //		glm::vec4 boneBallLocation = boneLocal * GameObjectlocation;
    //		//glm::vec4 boneBallLocation = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f );
    //		boneBallLocation *= scale;
    //		//boneBallLocation += glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    //			
    //		DrawDebugBall( glm::vec3(boneBallLocation), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.2f );
    //
    //		if ( boneIndex == 35 )
    //		{
    //			DrawDebugBall( glm::vec3(boneBallLocation), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.5f );
    //		}
    //	}


    //****************************************************************************************
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