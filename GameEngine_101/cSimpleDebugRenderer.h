#ifndef _cSimpleDebugRenderer_HG_
#define _cSimpleDebugRenderer_HG_
#include "globalOpenGL_GLFW.h"

class sAABB_Triangle;

enum DebugType
{
    DEBUG_POINT,
    DEBUG_LINE,
    DEBUG_TRIANGLE,
    DEBUG_CUBE
};

struct miniVAOInfo
{
    unsigned int VAO_ID;
    GLuint bufferID;
};

class cSimpleDebugRenderer
{
public:
    cSimpleDebugRenderer();
    ~cSimpleDebugRenderer();

    bool genDebugGeometry(DebugType, float size, long long& geometryID);
    bool genDebugTriangle(sAABB_Triangle, long long& geometryID);
    void drawDebugGeometry(glm::vec3 position, long long geometryID, glm::mat4x4 orientation);

private:
    std::map<long long, miniVAOInfo> mapGeometryID_VAOInfo;
    long long theGeoID;
};

#endif // !_cSimpleDebugRenderer_HG_

