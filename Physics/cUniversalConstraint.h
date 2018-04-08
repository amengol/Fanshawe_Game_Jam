#pragma once

#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <iRigidBody.h>

class btUniversalConstraint;

namespace nPhysics
{
    class cUniversalConstraint : public iConstraint
    {
    public:
        cUniversalConstraint(iRigidBody* rbA,
                             iRigidBody* rbB,
                             const glm::vec3& anchor,
                             const glm::vec3& axis1,
                             const glm::vec3& axis2);

        virtual ~cUniversalConstraint() { return; }
        virtual void SetDamping(float damping) { return; }
        virtual void SetImpulseClamp(float ImpulseClamp) { return; }

        btUniversalConstraint* GetBulletConstraint() { return mBulletConstraint; }

    private:
        btUniversalConstraint* mBulletConstraint;
    };
}