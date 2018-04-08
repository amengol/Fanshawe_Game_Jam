#include "stdafx.h"
#include "cPoint2PointConstraint.h"
#include <btBulletDynamicsCommon.h>
#include "cRigidBody.h"

nPhysics::cPoint2PointConstraint::cPoint2PointConstraint(iRigidBody* rbA,
                                                               iRigidBody* rbB,
                                                               const glm::vec3& pivotInA,
                                                               const glm::vec3& pivotInB)
{
    btRigidBody* bulletRB_A = (dynamic_cast<cRigidBody*>(rbA))->getBulletRigidBody();
    btRigidBody* bulletRB_B = (dynamic_cast<cRigidBody*>(rbB))->getBulletRigidBody();
    
    btVector3 btPivotInA;
    btPivotInA.setX(pivotInA.x);
    btPivotInA.setY(pivotInA.y);
    btPivotInA.setZ(pivotInA.z);

    btVector3 btPivotInB;
    btPivotInB.setX(pivotInB.x);
    btPivotInB.setY(pivotInB.y);
    btPivotInB.setZ(pivotInB.z);
    
    mBulletConstraint = new btPoint2PointConstraint(*bulletRB_A,
                                                    *bulletRB_B,
                                                    btPivotInA, 
                                                    btPivotInB);
}


nPhysics::cPoint2PointConstraint::~cPoint2PointConstraint()
{
}

void nPhysics::cPoint2PointConstraint::SetDamping(float damping)
{
    mBulletConstraint->m_setting.m_damping = damping;
}

void nPhysics::cPoint2PointConstraint::SetImpulseClamp(float ImpulseClamp)
{
    mBulletConstraint->m_setting.m_impulseClamp = ImpulseClamp;
}
