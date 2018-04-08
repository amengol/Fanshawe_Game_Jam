#include "stdafx.h"
#include "cFixedConstraint.h"
#include <btBulletDynamicsCommon.h>
#include "cRigidBody.h"

nPhysics::cFixedConstraint::cFixedConstraint(iRigidBody* rbA, 
                                                   iRigidBody* rbB, 
                                                   const glm::vec3& pivotInA,
                                                   const glm::vec3 & pivotInB)
{
    btRigidBody* bulletRB_A = (dynamic_cast<cRigidBody*>(rbA))->getBulletRigidBody();
    btRigidBody* bulletRB_B = (dynamic_cast<cRigidBody*>(rbB))->getBulletRigidBody();

    btVector3 btPivotInA;
    btPivotInA.setX(pivotInA.x);
    btPivotInA.setY(pivotInA.y);
    btPivotInA.setZ(pivotInA.z);

    btMatrix3x3 mat;
    mat.setIdentity();
    btTransform t_frameInA = btTransform(mat, btPivotInA);

    btVector3 btPivotInB;
    btPivotInB.setX(pivotInB.x);
    btPivotInB.setY(pivotInB.y);
    btPivotInB.setZ(pivotInB.z);

    btTransform t_frameInB = btTransform(mat, btPivotInB);

    mBulletConstraint = new btFixedConstraint(*bulletRB_A, *bulletRB_B, t_frameInA, t_frameInB);
}
