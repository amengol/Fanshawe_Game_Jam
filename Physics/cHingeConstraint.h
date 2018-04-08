#pragma once
#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <iRigidBody.h>

class btHingeConstraint;

namespace nPhysics
{
    class cHingeConstraint : public iConstraint
    {
    public:
        cHingeConstraint(iRigidBody* rbA,
                         iRigidBody* rbB,
                         const glm::vec3& pivotInA,
                         const glm::vec3& pivotInB,
                         glm::vec3& axisInA,
                         glm::vec3& axisInB);

        virtual ~cHingeConstraint() { return; }
        virtual void SetDamping(float damping) { return; }
        virtual void SetImpulseClamp(float ImpulseClamp) { return; }

        btHingeConstraint* GetBulletConstraint() { return mBulletConstraint; }

    private:
        btHingeConstraint* mBulletConstraint;
    };
}
