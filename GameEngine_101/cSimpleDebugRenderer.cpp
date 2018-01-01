#include "cSimpleDebugRenderer.h"
#include "globalGameStuff.h"
#include <glm/gtc/type_ptr.hpp>
#include "sAABB_Triangle.h"

cSimpleDebugRenderer::cSimpleDebugRenderer()
{
    this->theGeoID = 100; // As a reference for the first ID

    // Initialize the Dynamic Buffer
    glGenVertexArrays(1, &this->dynamicBuffer.VAO_ID);
    glBindVertexArray(this->dynamicBuffer.VAO_ID);
    glGenBuffers(1, &this->dynamicBuffer.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, this->dynamicBuffer.bufferID);

    // Put a data there just in case OpenGL wants
    // to wipe it out before we even use it
    glm::vec3 vGarbage(0.0f);
    // Allocate the global vertex array
    sVertex* pVertices = new sVertex[1];

    // Copy the local vertex array into the GPUs memory
    int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * 1;
    glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

    delete[] pVertices;

    // Unbind that VAO
    glBindVertexArray(0);

    // Unbind (release) everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

cSimpleDebugRenderer::~cSimpleDebugRenderer()
{
}

bool cSimpleDebugRenderer::genDebugGeometry(DebugType type, float size, long long& geometryID)
{
    switch(type)
    {
    case DEBUG_LINE:
    {
        unsigned int VAO_ID;

        glGenVertexArrays(1, &VAO_ID);
        glBindVertexArray(VAO_ID);

        GLuint bufferID;

        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);

        // Line vertices
        glm::vec3 vert0, vert1;

        // The vertice 0
        vert0 = glm::vec3(0.0f, 0.0f, 0.0f);

        // The vertice 1
        vert1.x = vert0.x;
        vert1.y = vert0.y + size;
        vert1.z = vert0.z;

        // Allocate the global vertex array
        sVertex* pVertices = new sVertex[2];

        //Line 0-1
        pVertices[0].x = vert0.x;
        pVertices[0].y = vert0.y;
        pVertices[0].z = vert0.z;
        pVertices[1].x = vert1.x;
        pVertices[1].y = vert1.y;
        pVertices[1].z = vert1.z;
        
        // Copy the local vertex array into the GPUs memory
        int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * 2;
        glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

        delete[] pVertices;

        // Unbind that VAO
        glBindVertexArray(0);

        // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        miniVAOInfo theVAO;
        theVAO.VAO_ID = VAO_ID;
        theVAO.bufferID = bufferID;


        this->mapGeometryID_VAOInfo[this->theGeoID] = theVAO;
        geometryID = this->theGeoID;
        this->theGeoID++;

        return true;
    }
    case DEBUG_CUBE:
    {
        unsigned int VAO_ID;

        glGenVertexArrays(1, &VAO_ID);
        glBindVertexArray(VAO_ID);

        GLuint bufferID;

        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);

        // All cube vertices
        glm::vec3 vert0, vert1, vert2, vert3, vert4, vert5, vert6, vert7;

        // The vertice 0
        vert0 = glm::vec3(0.0f, 0.0f, 0.0f);

        // The vertice 1
        vert1.x = vert0.x + size;
        vert1.y = vert0.y;
        vert1.z = vert0.z;

        // The vertice 2
        vert2.x = vert0.x;
        vert2.y = vert0.y + size;
        vert2.z = vert0.z;

        // The vertice 3
        vert3.x = vert0.x + size;
        vert3.y = vert0.y + size;
        vert3.z = vert0.z;

        // The vertice 4
        vert4.x = vert0.x;
        vert4.y = vert0.y;
        vert4.z = vert0.z + size;

        // The vertice 5
        vert5.x = vert0.x + size;
        vert5.y = vert0.y;
        vert5.z = vert0.z + size;

        // The vertice 6
        vert6.x = vert0.x;
        vert6.y = vert0.y + size;
        vert6.z = vert0.z + size;

        // The vertice 7
        vert7.x = vert0.x + size;
        vert7.y = vert0.y + size;
        vert7.z = vert0.z + size;

        // Allocate the global vertex array
        sVertex* pVertices = new sVertex[24];

        //Line 0-1
        pVertices[0].x = vert0.x;
        pVertices[0].y = vert0.y;
        pVertices[0].z = vert0.z;
        pVertices[1].x = vert1.x;
        pVertices[1].y = vert1.y;
        pVertices[1].z = vert1.z;

        //Line 0-2
        pVertices[2].x = vert0.x;
        pVertices[2].y = vert0.y;
        pVertices[2].z = vert0.z;
        pVertices[3].x = vert2.x;
        pVertices[3].y = vert2.y;
        pVertices[3].z = vert2.z;

        //Line 1-3
        pVertices[4].x = vert3.x;
        pVertices[4].y = vert3.y;
        pVertices[4].z = vert3.z;
        pVertices[5].x = vert1.x;
        pVertices[5].y = vert1.y;
        pVertices[5].z = vert1.z;

        //Line 2-3
        pVertices[6].x = vert2.x;
        pVertices[6].y = vert2.y;
        pVertices[6].z = vert2.z;
        pVertices[7].x = vert3.x;
        pVertices[7].y = vert3.y;
        pVertices[7].z = vert3.z;

        //Line 0-4
        pVertices[8].x = vert0.x;
        pVertices[8].y = vert0.y;
        pVertices[8].z = vert0.z;
        pVertices[9].x = vert4.x;
        pVertices[9].y = vert4.y;
        pVertices[9].z = vert4.z;

        //Line 2-6
        pVertices[10].x = vert2.x;
        pVertices[10].y = vert2.y;
        pVertices[10].z = vert2.z;
        pVertices[11].x = vert6.x;
        pVertices[11].y = vert6.y;
        pVertices[11].z = vert6.z;

        //Line 1-5
        pVertices[12].x = vert1.x;
        pVertices[12].y = vert1.y;
        pVertices[12].z = vert1.z;
        pVertices[13].x = vert5.x;
        pVertices[13].y = vert5.y;
        pVertices[13].z = vert5.z;

        //Line 3-7
        pVertices[14].x = vert3.x;
        pVertices[14].y = vert3.y;
        pVertices[14].z = vert3.z;
        pVertices[15].x = vert7.x;
        pVertices[15].y = vert7.y;
        pVertices[15].z = vert7.z;

        //Line 4-6
        pVertices[16].x = vert4.x;
        pVertices[16].y = vert4.y;
        pVertices[16].z = vert4.z;
        pVertices[17].x = vert6.x;
        pVertices[17].y = vert6.y;
        pVertices[17].z = vert6.z;

        //Line 4-5
        pVertices[18].x = vert4.x;
        pVertices[18].y = vert4.y;
        pVertices[18].z = vert4.z;
        pVertices[19].x = vert5.x;
        pVertices[19].y = vert5.y;
        pVertices[19].z = vert5.z;

        //Line 5-7
        pVertices[20].x = vert5.x;
        pVertices[20].y = vert5.y;
        pVertices[20].z = vert5.z;
        pVertices[21].x = vert7.x;
        pVertices[21].y = vert7.y;
        pVertices[21].z = vert7.z;

        //Line 6-7
        pVertices[22].x = vert6.x;
        pVertices[22].y = vert6.y;
        pVertices[22].z = vert6.z;
        pVertices[23].x = vert7.x;
        pVertices[23].y = vert7.y;
        pVertices[23].z = vert7.z;

        // Copy the local vertex array into the GPUs memory
        int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * 24;
        glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

        delete[] pVertices;

        // Unbind that VAO
        glBindVertexArray(0);

        // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        miniVAOInfo theVAO;
        theVAO.VAO_ID = VAO_ID;
        theVAO.bufferID = bufferID;

        
        this->mapGeometryID_VAOInfo[this->theGeoID] = theVAO;
        geometryID = this->theGeoID;
        this->theGeoID++;        

        return true;
    }
    default:
        return false;
    }
}

bool cSimpleDebugRenderer::genDebugTriangle(sAABB_Triangle theTri, long long& geometryID)
{
    unsigned int VAO_ID;

    glGenVertexArrays(1, &VAO_ID);
    glBindVertexArray(VAO_ID);

    GLuint bufferID;

    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    // All triangle vertices
    glm::vec3 vertA = theTri.verticeA;
    glm::vec3 vertB = theTri.verticeB;
    glm::vec3 vertC = theTri.verticeC;

    // Allocate the global vertex array
    sVertex* pVertices = new sVertex[6];

    //Line A-B
    pVertices[0].x = vertA.x;
    pVertices[0].y = vertA.y;
    pVertices[0].z = vertA.z;
    pVertices[1].x = vertB.x;
    pVertices[1].y = vertB.y;
    pVertices[1].z = vertB.z;

    //Line B-C
    pVertices[2].x = vertB.x;
    pVertices[2].y = vertB.y;
    pVertices[2].z = vertB.z;
    pVertices[3].x = vertC.x;
    pVertices[3].y = vertC.y;
    pVertices[3].z = vertC.z;

    //Line C-A
    pVertices[4].x = vertC.x;
    pVertices[4].y = vertC.y;
    pVertices[4].z = vertC.z;
    pVertices[5].x = vertA.x;
    pVertices[5].y = vertA.y;
    pVertices[5].z = vertA.z;
   
    // Copy the local vertex array into the GPUs memory
    int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * 6;
    glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

    delete[] pVertices;

    // Unbind that VAO
    glBindVertexArray(0);

    // Unbind (release) everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    miniVAOInfo theVAO;
    theVAO.VAO_ID = VAO_ID;
    theVAO.bufferID = bufferID;


    this->mapGeometryID_VAOInfo[this->theGeoID] = theVAO;
    geometryID = this->theGeoID;
    this->theGeoID++;

    return true;
}

void cSimpleDebugRenderer::drawDebugGeometry(glm::vec3 position, long long geometryID, glm::vec3 color, glm::mat4x4 orientation)
{
    std::map<long long, miniVAOInfo>::iterator itIDVao = mapGeometryID_VAOInfo.find(geometryID);
    if(itIDVao == mapGeometryID_VAOInfo.end())
    {
        return;
    }
    else
    {
        miniVAOInfo theVAO = itIDVao->second;

        glBindVertexArray(theVAO.VAO_ID);
        glBindBuffer(GL_ARRAY_BUFFER, theVAO.bufferID);

        // Now set up the vertex layout (for this shader)
        GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
        GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
        GLuint uniLoc_materialDiffuse = glGetUniformLocation(shaderID, "materialDiffuse");
        GLuint uniLoc_mModel = glGetUniformLocation(shaderID, "mModel");
        GLuint uniLoc_HasColour = glGetUniformLocation(shaderID, "hasColour");
        GLuint uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(shaderID, "bIsDebugWireFrameObject");

        // Size of the vertex we are using in the array.
        // This is called the "stride" of the vertices in the vertex buffer
        const unsigned int VERTEX_STRIDE = sizeof(sVertex);

        glEnableVertexAttribArray(vpos_location);
        const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
        glVertexAttribPointer(vpos_location, 3,
                              GL_FLOAT, GL_FALSE,
                              VERTEX_STRIDE,
                              reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));


        glm::mat4x4 mModel = glm::mat4x4(1.0f);

        // Translate
        mModel = glm::translate(mModel, position);

        // Rotate
        mModel = mModel * orientation;



        glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mModel));

        glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

        glUniform1f(uniLoc_HasColour, 0.0f);
        glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);
        glUniform4f(uniLoc_materialDiffuse, color.r, color.g, color.b, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);


        glCullFace(GL_BACK);

        glDrawArrays(GL_LINES, 0, 24);


        // Unbind that VAO
        glBindVertexArray(0);

        // Unbind (release) everything
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vpos_location);
    }
}

void cSimpleDebugRenderer::drawCustomGeometry(std::vector<sVertex>& theMesh, glm::vec3 color)
{
    glBindVertexArray(this->dynamicBuffer.VAO_ID);
    glBindBuffer(GL_ARRAY_BUFFER, this->dynamicBuffer.bufferID);

    sVertex* pVertices = new sVertex[theMesh.size()];

    for(int i = 0; i < theMesh.size(); i++)
    {
        pVertices[i] = theMesh[i];
    }

    // Copy the local vertex array into the GPUs memory
    int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex) * theMesh.size();
    glBufferData(GL_ARRAY_BUFFER, sizeOfGlobalVertexArrayInBytes, pVertices, GL_STATIC_DRAW);

    delete[] pVertices;

    // Now set up the vertex layout (for this shader)
    GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("GE101_Shader");
    GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");
    GLuint uniLoc_materialDiffuse = glGetUniformLocation(shaderID, "materialDiffuse");
    GLuint uniLoc_mModel = glGetUniformLocation(shaderID, "mModel");
    GLuint uniLoc_HasColour = glGetUniformLocation(shaderID, "hasColour");
    GLuint uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(shaderID, "bIsDebugWireFrameObject");

    // Size of the vertex we are using in the array.
    // This is called the "stride" of the vertices in the vertex buffer
    const unsigned int VERTEX_STRIDE = sizeof(sVertex);

    glEnableVertexAttribArray(vpos_location);
    const unsigned int OFFSET_TO_X = offsetof(sVertex, x);
    glVertexAttribPointer(vpos_location, 3,
                          GL_FLOAT, GL_FALSE,
                          VERTEX_STRIDE,
                          reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X)));


    glm::mat4x4 mModel = glm::mat4x4(1.0f);


    glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mModel));

    glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

    glUniform1f(uniLoc_HasColour, 0.0f);
    glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);
    glUniform4f(uniLoc_materialDiffuse, color.r, color.g, color.b, 1.0f);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);


    glCullFace(GL_BACK);

    glDrawArrays(GL_LINES, 0, theMesh.size() * 3);


    // Unbind that VAO
    glBindVertexArray(0);

    // Unbind (release) everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vpos_location);
}
