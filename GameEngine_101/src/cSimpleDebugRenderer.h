#ifndef _cSimpleDebugRenderer_HG_
#define _cSimpleDebugRenderer_HG_

#include <glm\glm.hpp>
#include <map>
#include <vector>

struct sVertex;
struct sAABB_Triangle;
typedef unsigned int GLuint;

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
    void drawDebugGeometry(glm::vec3 position, long long geometryID, glm::vec3 color, glm::mat4x4 orientation);
    void drawCustomGeometry(std::vector<sVertex>&, 
                            glm::vec3 color,
                            bool repositioning = false,
                            glm::mat4 posMatrix = glm::mat4(1.0f));
    void drawCustomLines(std::vector<sVertex>&,
                         glm::vec3 color,
                         bool repositioning = false,
                         glm::mat4 posMatrix = glm::mat4(1.0f));

private:
    std::map<long long, miniVAOInfo> mapGeometryID_VAOInfo;
    long long theGeoID;
    miniVAOInfo dynamicBuffer;
};

#endif // !_cSimpleDebugRenderer_HG_

