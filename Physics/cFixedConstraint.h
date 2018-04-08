#pragma once

#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <iRigidBody.h>

class btFixedConstraint;

namespace nPhysics
{
    class cFixedConstraint : public iConstraint
    {
    public:
        cFixedConstraint(iRigidBody* rbA,
                         iRigidBody* rbB,
                         const glm::vec3& pivotInA,
                         const glm::vec3& pivotInB);

        virtual ~cFixedConstraint() { return; }
        virtual void SetDamping(float damping) { return; }
        virtual void SetImpulseClamp(float ImpulseClamp) { return; }

        btFixedConstraint* GetBulletConstraint() { return mBulletConstraint; }

    private:
        btFixedConstraint* mBulletConstraint;
    };
}