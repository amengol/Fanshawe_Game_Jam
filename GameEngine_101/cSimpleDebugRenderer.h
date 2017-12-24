#ifndef _cSimpleDebugRenderer_HG_
#define _cSimpleDebugRenderer_HG_
#include "globalOpenGL_GLFW.h"

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
    void drawDebugGeometry(glm::vec3 position, long long geometryID);

private:
    std::map<long long, miniVAOInfo> mapGeometryID_VAOInfo;
    long long theGeoID;
};

#endif // !_cSimpleDebugRenderer_HG_

