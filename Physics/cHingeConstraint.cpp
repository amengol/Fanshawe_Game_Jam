#include "stdafx.h"
#include "cHingeConstraint.h"
#include <btBulletDynamicsCommon.h>
#include "cRigidBody.h"

nPhysics::cHingeConstraint::cHingeConstraint(iRigidBody* rbA,
                                                   iRigidBody* rbB, 
                                                   const glm::vec3& pivotInA, 
                                                   const glm::vec3& pivotInB, 
                                                   glm::vec3& axisInA, 
                                                   glm::vec3& axisInB)
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

    btVector3 btAxisInA;
    btAxisInA.setX(axisInA.x);
    btAxisInA.setY(axisInA.y);
    btAxisInA.setZ(axisInA.z);

    btVector3 btAxisInB;
    btAxisInB.setX(axisInB.x);
    btAxisInB.setY(axisInB.y);
    btAxisInB.setZ(axisInB.z);

    mBulletConstraint = new btHingeConstraint(*bulletRB_A, *bulletRB_B, btPivotInA, 
                                              btPivotInB, btAxisInA, btAxisInB);
}
