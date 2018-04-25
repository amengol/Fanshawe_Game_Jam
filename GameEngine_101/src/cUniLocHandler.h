#pragma once

#include <string>
typedef int GLint;

class cUniLocHandler
{
public:
    void InitShaderUniformLocations(std::string shaderName);

    GLint currentProgID;

    GLint materialDiffuse;
    GLint ambientToDiffuseRatio;
    GLint materialSpecular;
    GLint bIsDebugWireFrameObject;
    GLint hasColour;
    GLint hasAlpha;
    GLint hasMultiLayerTextures;
    GLint hasReflection;
    GLint isReflectRefract;
    GLint useDiscardAlpha;
    GLint isASkyBox;
    GLint eyePosition;
    GLint mModel;
    GLint mView;
    GLint mProjection;
    GLint mWorldInvTrans;
    GLint gamaCorrection;
    GLint receiveShadow;
    GLint selfLight;
    GLint hasNormalMap;
    GLint fogActive;
    GLint fogColour;
    GLint fogPercent;
    GLint renderPassNumber;
    GLint shadowMap;
    GLint texFBOColour2D;
    GLint texFBONormal2D;
    GLint texFBOVertexWorldPos2D;
    GLint screenWidth;
    GLint screenHeight;
    GLint fullRenderedImage2D;
    GLint fullRenderedImage2D_Alpha;
    GLint fullRenderedImage2D_Overlay;
    GLint bIsASkinnedMesh;
    GLint numBonesUsed;
    GLint bones;
    GLint fade;
    GLint sysTime;
    GLint noise;
    GLint noiseEffectOn;

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