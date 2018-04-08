#pragma once

namespace nPhysics
{
    class iConstraint
    {
    public:
        virtual ~iConstraint() { }

        virtual void SetDamping(float damping) = 0;
        virtual void SetImpulseClamp(float ImpulseClamp) = 0;
    };
}
