#pragma once

namespace nPhysics
{
	class iSpring
	{
	public:
		virtual ~iSpring() {}

        // Offesets the two nodes of the spring according Hooke's Law
        virtual void restoreSpring() = 0;
	};
}