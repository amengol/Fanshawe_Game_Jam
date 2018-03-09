#include "cUniLocHandler.h"
#include "globalGameStuff.h"

cUniLocHandler::cUniLocHandler()
{
    currentProgID = -1;
    materialDiffuse = -1;
    ambientToDiffuseRatio = -1;
    materialSpecular = -1;
    bIsDebugWireFrameObject = -1;
    hasColour = -1;
    hasAlpha = -1;
    hasReflection = -1;
    isReflectRefract = -1;
    useDiscardAlpha = -1;
    isASkyBox = -1;
    eyePosition = -1;
    mModel = -1;
    mView = -1;
    mProjection = -1;
    mWorldInvTrans = -1;
}

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
        hasReflection = glGetUniformLocation(currentProgID, "hasReflection");
        isReflectRefract = glGetUniformLocation(currentProgID, "isReflectRefract");
        useDiscardAlpha = glGetUniformLocation(currentProgID, "useDiscardAlpha");
        isASkyBox = glGetUniformLocation(currentProgID, "isASkyBox");
        eyePosition = glGetUniformLocation(currentProgID, "eyePosition");
        mModel = glGetUniformLocation(currentProgID, "mModel");
        mView = glGetUniformLocation(currentProgID, "mView");
        mProjection = glGetUniformLocation(currentProgID, "mProjection");
        mWorldInvTrans = glGetUniformLocation(currentProgID, "mWorldInvTranspose");
    }
}
