#pragma once
#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <iRigidBody.h>

class btPoint2PointConstraint;

namespace nPhysics
{
    class cPoint2PointConstraint : public iConstraint
    {
    public:
        cPoint2PointConstraint(iRigidBody* rbA,
                               iRigidBody* rbB,
                               const glm::vec3& pivotInA,
                               const glm::vec3& pivotInB);

        virtual ~cPoint2PointConstraint();
        virtual void SetDamping(float damping);
        virtual void SetImpulseClamp(float ImpulseClamp);

        btPoint2PointConstraint* GetBulletConstraint() { return mBulletConstraint; }

    private:
        btPoint2PointConstraint* mBulletConstraint;
    };
}
