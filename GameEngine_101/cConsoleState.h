#ifndef _cConsoleState_HG_
#define _cConsoleState_HG_
#include <utils\utils.h>


class cConsoleState
{
public:
    cConsoleState();
    ~cConsoleState();

    void printScreen(FMOD::ChannelGroup* groupA, 
                     FMOD::ChannelGroup* groupB, 
                     FMOD::ChannelGroup* groupC);
    void setState(int state);

private:
    bool mFirstLevel;
    bool mSecondLevel;
    int mSelectedGroup;
    bool mAspects;
    bool mEffects;
    bool mDSPG1_1;
    bool mDSPG1_2;
    bool mDSPG1_3;
    bool mDSPG2_1;
    bool mDSPG2_2;
    bool mDSPG2_3;
    bool mDSPG3_1;
    bool mDSPG3_2;
    bool mDSPG3_3;
    bool mSound1;
    bool mSound2;
    bool mSound3;
    bool mVolume;
    bool mMute;
    bool mPlaySpeed;
    bool mPitch;
    bool mBalance;

    FMOD_RESULT mresult;
    void handle_keyboard();
};

#endif // !_cConsoleState_HG_

