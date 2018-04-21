#include "stdafx.h"
#include "cBulletDebugDrawer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void cBulletDebugDrawer::SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix, int shaderID)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "mProjection"), 1, GL_FALSE, &pProjectionMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "mView"), 1, GL_FALSE, &pViewMatrix[0][0]);
}

cBulletDebugDrawer::cBulletDebugDrawer()
{
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
}

cBulletDebugDrawer::~cBulletDebugDrawer()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void cBulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    // Vertex data
    GLfloat points[12];

    points[0] = from.x();
    points[1] = from.y();
    points[2] = from.z();
    points[3] = color.x();
    points[4] = color.y();
    points[5] = color.z();

    points[6] = to.x();
    points[7] = to.y();
    points[8] = to.z();
    points[9] = color.x();
    points[10] = color.y();
    points[11] = color.z();

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

}
