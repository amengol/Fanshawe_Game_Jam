#pragma once
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>
// Helper class; draws the world as seen by Bullet.
// This is very handy to see it Bullet's world matches yours
// How to use this class :
// Declare an instance of the class :
// 
// dynamicsWorld->setDebugDrawer(&mydebugdrawer);
// Each frame, call it :
// mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
// dynamicsWorld->debugDrawWorld();
class cBulletDebugDrawer : public btIDebugDraw
{
public:
    cBulletDebugDrawer();
    ~cBulletDebugDrawer();

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) { }
    virtual void reportErrorWarning(const char *) { }
    virtual void draw3dText(const btVector3 &, const char *) { }
    virtual void setDebugMode(int p) { }
    int getDebugMode(void) const { return 1; }

    void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix, int shaderID);

private:
    unsigned int m_VAO;
    unsigned int m_VBO;
};