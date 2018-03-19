#pragma once

struct sFade
{
    sFade() :
        mElapseTime(0.0f),
        mFade(0.0f),
        mIsEnable(false)
    { }
    
    // Starts the fade effect
    void startFade(float timeToFade)
    {
        mElapseTime = 0.0f;
        mFade = 1.0f;
        mTimeToFade = timeToFade;
        mIsEnable = true;
    }
    
    // Updates the elapse time
    void Update(float deltaTime)
    {
        if (mIsEnable)
        {
            mElapseTime += deltaTime;
            if (mElapseTime > mTimeToFade)
            {
                mElapseTime = 0.0f;
                mFade = 0.0f;
                mTimeToFade = 0.0f;
                mIsEnable = false;
            }
            else
            {
                mFade = 1.0f - (mElapseTime / mTimeToFade);
            }
        }
    }

    float mIsEnable;
    float mElapseTime;
    float mFade;
    float mTimeToFade;
};