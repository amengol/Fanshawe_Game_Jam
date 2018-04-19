#pragma once
#include <iPhysicsDebugDrawer.h>
#include "cBulletDebugDrawer.h"

class cBulletDebugDrawer;
namespace nPhysics
{
    class cPhysicsDebugDrawer : public iPhysicsDebugDrawer
    {
    public:
        cPhysicsDebugDrawer();
        virtual ~cPhysicsDebugDrawer() { }

        virtual void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& colour);
        virtual void drawContactPoint(const glm::vec3& PointOnB,
                                      const glm::vec3& normalOnB,
                                      float distance,
                                      int lifeTime,
                                      const glm::vec3& color)  { }
        virtual void reportErrorWarning(const char* warningString) { }
        virtual void draw3dText(const glm::vec3& location, const char* textString) { }
        virtual void setDebugMode(int debugMode) { }
        virtual int	 getDebugMode() const { return 1; }

        cBulletDebugDrawer* getBulletDebugDrawer() { return m_bulletDebugDrawer; }

    private:
        cBulletDebugDrawer* m_bulletDebugDrawer;
    };
}
