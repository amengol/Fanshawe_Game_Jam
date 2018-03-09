#pragma once

#include <string>
typedef int GLint;

class cUniLocHandler
{
public:
    cUniLocHandler();

    void InitShaderUniformLocations(std::string shaderName);

    GLint currentProgID;

    GLint materialDiffuse;
    GLint ambientToDiffuseRatio;
    GLint materialSpecular;
    GLint bIsDebugWireFrameObject;
    GLint hasColour;
    GLint hasAlpha;
    GLint hasReflection;
    GLint isReflectRefract;
    GLint useDiscardAlpha;
    GLint isASkyBox;
    GLint eyePosition;
    GLint mModel;
    GLint mView;
    GLint mProjection;
    GLint mWorldInvTrans;
private:

};