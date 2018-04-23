#include "cUniLocHandler.h"
#include "globalGameStuff.h"
#include "globalOpenGL_GLFW.h"

void cUniLocHandler::InitShaderUniformLocations(std::string shaderName)
{
    if (shaderName == "GE101_Shader")
    {
        currentProgID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");

        materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");
        ambientToDiffuseRatio = glGetUniformLocation(currentProgID, "ambientToDiffuseRatio");
        materialSpecular = glGetUniformLocation(currentProgID, "materialSpecular");
        bIsDebugWireFrameObject = glGetUniformLocation(currentProgID, "bIsDebugWireFrameObject");
        hasColour = glGetUniformLocation(currentProgID, "hasColour");
        hasAlpha = glGetUniformLocation(currentProgID, "hasAlpha");
        hasMultiLayerTextures = glGetUniformLocation(currentProgID, "hasMultiLayerTextures");
        hasReflection = glGetUniformLocation(currentProgID, "hasReflection");
        isReflectRefract = glGetUniformLocation(currentProgID, "isReflectRefract");
        useDiscardAlpha = glGetUniformLocation(currentProgID, "useDiscardAlpha");
        isASkyBox = glGetUniformLocation(currentProgID, "isASkyBox");
        eyePosition = glGetUniformLocation(currentProgID, "eyePosition");
        mModel = glGetUniformLocation(currentProgID, "mModel");
        mView = glGetUniformLocation(currentProgID, "mView");
        mProjection = glGetUniformLocation(currentProgID, "mProjection");
        mWorldInvTrans = glGetUniformLocation(currentProgID, "mWorldInvTranspose");
        gamaCorrection = glGetUniformLocation(currentProgID, "gamaCorrection");
        receiveShadow = glGetUniformLocation(currentProgID, "receiveShadow");
        selfLight = glGetUniformLocation(currentProgID, "selfLight");
        hasNormalMap = glGetUniformLocation(currentProgID, "hasNormalMap");
        fogActive = glGetUniformLocation(currentProgID, "fogActive");
        fogColour = glGetUniformLocation(currentProgID, "fogColour");
        fogPercent = glGetUniformLocation(currentProgID, "fogPercent");
        renderPassNumber = glGetUniformLocation(currentProgID, "renderPassNumber");
        shadowMap = glGetUniformLocation(currentProgID, "shadowMap");
        shadowAlphaMap = glGetUniformLocation(currentProgID, "shadowAlphaMap");
        texFBOColour2D = glGetUniformLocation(currentProgID, "texFBOColour2D");
        texFBONormal2D = glGetUniformLocation(currentProgID, "texFBONormal2D");
        texFBOVertexWorldPos2D = glGetUniformLocation(currentProgID, "texFBOVertexWorldPos2D");
        screenWidth = glGetUniformLocation(currentProgID, "screenWidth");
        screenHeight = glGetUniformLocation(currentProgID, "screenHeight");
        fullRenderedImage2D = glGetUniformLocation(currentProgID, "fullRenderedImage2D");
        fullRenderedImage2D_Overlay = glGetUniformLocation(currentProgID, "fullRenderedImage2D_Overlay");
        bIsASkinnedMesh = glGetUniformLocation(currentProgID, "bIsASkinnedMesh");
        numBonesUsed = glGetUniformLocation(currentProgID, "numBonesUsed");
        bones = glGetUniformLocation(currentProgID, "bones");

        // Textures
        textSampler00_ID = glGetUniformLocation(currentProgID, "texSamp2D00");
        textSampler01_ID = glGetUniformLocation(currentProgID, "texSamp2D01");
        textSampler02_ID = glGetUniformLocation(currentProgID, "texSamp2D02");
        textSampler03_ID = glGetUniformLocation(currentProgID, "texSamp2D03");
        textSampler04_ID = glGetUniformLocation(currentProgID, "texSamp2D04");
        textSampler05_ID = glGetUniformLocation(currentProgID, "texSamp2D05");
        textSampler06_ID = glGetUniformLocation(currentProgID, "texSamp2D06");
        textSampler07_ID = glGetUniformLocation(currentProgID, "texSamp2D07");

        textBlend00_ID = glGetUniformLocation(currentProgID, "texBlend00");
        textBlend01_ID = glGetUniformLocation(currentProgID, "texBlend01");
        textBlend02_ID = glGetUniformLocation(currentProgID, "texBlend02");
        textBlend03_ID = glGetUniformLocation(currentProgID, "texBlend03");
        textBlend04_ID = glGetUniformLocation(currentProgID, "texBlend04");
        textBlend05_ID = glGetUniformLocation(currentProgID, "texBlend05");
        textBlend06_ID = glGetUniformLocation(currentProgID, "texBlend06");
        textBlend07_ID = glGetUniformLocation(currentProgID, "texBlend07");

        texSampCube00_LocID = glGetUniformLocation(currentProgID, "texSampCube00");
        texSampCube01_LocID = glGetUniformLocation(currentProgID, "texSampCube01");
        texSampCube02_LocID = glGetUniformLocation(currentProgID, "texSampCube02");
        texSampCube03_LocID = glGetUniformLocation(currentProgID, "texSampCube03");
        texSampCube04_LocID = glGetUniformLocation(currentProgID, "texSampCube04");

        texCubeBlend00_LocID = glGetUniformLocation(currentProgID, "texCubeBlend00");
        texCubeBlend01_LocID = glGetUniformLocation(currentProgID, "texCubeBlend01");
        texCubeBlend02_LocID = glGetUniformLocation(currentProgID, "texCubeBlend02");
        texCubeBlend03_LocID = glGetUniformLocation(currentProgID, "texCubeBlend03");
        texCubeBlend04_LocID = glGetUniformLocation(currentProgID, "texCubeBlend04");
    }
}
