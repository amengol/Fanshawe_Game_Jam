#include "stdafx.h"
#include "cPhysicsDebugDrawer.h"

nPhysics::cPhysicsDebugDrawer::cPhysicsDebugDrawer()
{
    m_bulletDebugDrawer = new cBulletDebugDrawer();
}

void nPhysics::cPhysicsDebugDrawer::drawLine(const glm::vec3& from,
                                             const glm::vec3& to, 
                                             const glm::vec3& colour)
{
    btVector3 btFrom(from.x, from.y, from.z);
    btVector3 btTo(to.x, to.y, to.z);
    btVector3 btColour(colour.x, colour.y, colour.z);

    m_bulletDebugDrawer->drawLine(btFrom, btTo, btColour);
}
