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

    // Textures
    GLint textSampler00_ID;
    GLint textSampler01_ID;
    GLint textSampler02_ID;
    GLint textSampler03_ID;
    GLint textSampler04_ID;
    GLint textSampler05_ID;
    GLint textSampler06_ID;
    GLint textSampler07_ID;
                        
    GLint textBlend00_ID;
    GLint textBlend01_ID;
    GLint textBlend02_ID;
    GLint textBlend03_ID;
    GLint textBlend04_ID;
    GLint textBlend05_ID;
    GLint textBlend06_ID;
    GLint textBlend07_ID;
                          
    GLint texSampCube00_LocID;
    GLint texSampCube01_LocID;
    GLint texSampCube02_LocID;
    GLint texSampCube03_LocID;
    GLint texSampCube04_LocID;
                             
    GLint texCubeBlend00_LocID;
    GLint texCubeBlend01_LocID;
    GLint texCubeBlend02_LocID;
    GLint texCubeBlend03_LocID;
    GLint texCubeBlend04_LocID;
private:

};