#include "stdafx.h"
#include "cUniversalConstraint.h"
#include <btBulletDynamicsCommon.h>
#include "cRigidBody.h"

nPhysics::cUniversalConstraint::cUniversalConstraint(iRigidBody* rbA, 
                                                           iRigidBody* rbB, 
                                                           const glm::vec3& anchor, 
                                                           const glm::vec3& axis1, 
                                                           const glm::vec3& axis2)
{
    btRigidBody* bulletRB_A = (dynamic_cast<cRigidBody*>(rbA))->getBulletRigidBody();
    btRigidBody* bulletRB_B = (dynamic_cast<cRigidBody*>(rbB))->getBulletRigidBody();

    btVector3 btAnchor;
    btAnchor.setX(anchor.x);
    btAnchor.setY(anchor.y);
    btAnchor.setZ(anchor.z);

    btVector3 btAxis1;
    btAxis1.setX(axis1.x);
    btAxis1.setY(axis1.y);
    btAxis1.setZ(axis1.z);

    btVector3 btAxis2;
    btAxis2.setX(axis2.x);
    btAxis2.setY(axis2.y);
    btAxis2.setZ(axis2.z);

    mBulletConstraint = new btUniversalConstraint(*bulletRB_A, *bulletRB_B, btAnchor, btAxis1, btAxis2);
}
