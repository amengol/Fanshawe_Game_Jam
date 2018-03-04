#include "cAnimationState.h"

// Returns true if time had to be reset
// (for checking to see if the animation has finished or not)
bool cAnimationState::sStateDetails::IncrementTime(bool bResetToZero /*=true*/)
{
	bool bDidWeReset = false;

	this->currentTime += this->frameStepTime;
	if ( this->currentTime >= this->totalTime )
	{
        this->currentTime = fmod(this->currentTime - this->totalTime, this->totalTime);
		bDidWeReset = true;
	}

	return bDidWeReset;
}

bool cAnimationState::sStateDetails::IncrementTime(float deltaTime, bool bResetToZero)
{
    bool bDidWeReset = false;

    this->currentTime += deltaTime;
    if (this->currentTime >= this->totalTime)
    {
        this->currentTime = fmod(this->currentTime - this->totalTime, this->totalTime);
        bDidWeReset = true;
    }

    return bDidWeReset;
}
