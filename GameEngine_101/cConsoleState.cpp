#include "cConsoleState.h"
#include <string>

// Internal function signatures
extern void errorcheck(FMOD_RESULT result);

bool mkeydown = false;
int sleep_ms = 200;

// FMOD variables
int numChannels = 0;
FMOD::Channel* chann;
FMOD::Sound* theSound;
char soundName[64];
float theVolume;
bool isMute = false;
float thePan = 0.0f;
float theFrequency;
float thePitch;
FMOD::DSP* theDSP;
bool bypass = false;

cConsoleState::cConsoleState()
{
    this->mFirstLevel = true;
    this->mSecondLevel = false;
    this->mSelectedGroup = 0;
    this->mAspects = false;
    this->mEffects = false;
    this->mDSPG1_1 = false;
    this->mDSPG1_2 = false;
    this->mDSPG1_3 = false;
    this->mDSPG2_1 = false;
    this->mDSPG2_2 = false;
    this->mDSPG2_3 = false;
    this->mDSPG3_1 = false;
    this->mDSPG3_2 = false;
    this->mDSPG3_3 = false;
    this->mSound1 = false;
    this->mSound2 = false;
    this->mSound3 = false;
    this->mVolume = false;
    this->mMute = false;
    this->mPlaySpeed = false;
    this->mPitch = false;
    this->mBalance = false;
}

cConsoleState::~cConsoleState()
{
    return;
}

void cConsoleState::printScreen(FMOD::ChannelGroup* groupA, FMOD::ChannelGroup* groupB, FMOD::ChannelGroup* groupC)
{

    //Needed for print_text
    start_text();

    this->handle_keyboard();

    if (this->mFirstLevel)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        print_text("               GroupA[ ]     GroupB[ ]     GroupC[ ]                           ");
        print_text("                      1             2             3                            ");
        print_text("===============================================================================");
        for (int i = 0; i < 19; i++)
            print_text("                                                                               ");
    }

    if (this->mSecondLevel)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
        else if (this->mSelectedGroup == 2)
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
        else
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
        print_text("                                                         ESCAPE to go back!    ");
        print_text("===============================================================================");
        print_text("1 [ ] Change aspects of a sound (volume, balance, etc)                         ");
        print_text("2 [ ] Apply DSP effects to the selected group                                  ");
        print_text("-------------------------------------------------------------------------------");
        for (int i = 0; i < 17; i++)
            print_text("                                                                               ");
    }

    if (this->mEffects)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
        {
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [ ] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [X] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            print_text("1 [%c] Echo                                                                    ",
                this->mDSPG1_1 ? 'X' : ' ');
            groupA->getDSP(1, &theDSP);
            theDSP->getBypass(&bypass);
            if (mDSPG1_1 == bypass)
            {
                bypass = !bypass;
                theDSP->setBypass(bypass);
            }
            print_text("2 [%c] Chorus                                                                   ",
                this->mDSPG1_2 ? 'X' : ' ');
            groupA->getDSP(2, &theDSP);
            theDSP->getBypass(&bypass);
            if (mDSPG1_2 == bypass)
            {
                bypass = !bypass;
                theDSP->setBypass(bypass);
            }
            print_text("3 [%c] Distortion                                                               ",
                this->mDSPG1_3 ? 'X' : ' ');
            groupA->getDSP(3, &theDSP);
            theDSP->getBypass(&bypass);
            if (mDSPG1_3 == bypass)
            {
                bypass = !bypass;
                theDSP->setBypass(bypass);
            }
            print_text("-------------------------------------------------------------------------------");
            groupA->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++)
            {
                groupA->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("   %s                                                                          "
                    , soundName);
            }
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < 9; i++)
                print_text("                                                                               ");
        }
        else if (this->mSelectedGroup == 2)
        {
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [ ] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [X] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            print_text("1 [%c] Lowpass                                                                  ",
                this->mDSPG2_1 ? 'X' : ' ');
            groupB->getDSP(1, &theDSP);
            theDSP->getBypass(&bypass);
            if (mDSPG2_1 == bypass)
            {
                bypass = !bypass;
                theDSP->setBypass(bypass);
            }
            print_text("2 [%c] DSP Effect                                                               ",
                this->mDSPG2_2 ? 'X' : ' ');
            print_text("3 [%c] DSP Effect                                                               ",
                this->mDSPG2_3 ? 'X' : ' ');
            print_text("-------------------------------------------------------------------------------");
            groupB->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++)
            {
                groupB->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("   %s                                                                          "
                    , soundName);
            }
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < 9; i++)
                print_text("                                                                               ");
        }
        else
        {
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [ ] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [X] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            print_text("1 [%c] DSP Effect                                                               ",
                this->mDSPG3_1 ? 'X' : ' ');
            print_text("2 [%c] DSP Effect                                                               ",
                this->mDSPG3_2 ? 'X' : ' ');
            print_text("3 [%c] DSP Effect                                                               ",
                this->mDSPG3_3 ? 'X' : ' ');
            print_text("-------------------------------------------------------------------------------");
            groupC->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++)
            {
                groupC->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("   %s                                                                          "
                    , soundName);
            }
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < 9; i++)
                print_text("                                                                               ");
        }
    }    

    if (this->mAspects)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
        {
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            groupA->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++) 
            {                           
                groupA->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("%d [ ] %s                                                                      "
                    , i + 1, soundName);
            }
            for (int i = 0; i < 14; i++)
                print_text("                                                                               ");
        }
        else if (this->mSelectedGroup == 2)
        {
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            groupB->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++) 
            {
                           
                groupB->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("%d [ ] %s                                                                      "
                    , i + 1, soundName);
            }
            for (int i = 0; i < 14; i++)
                print_text("                                                                               ");
        }
        else
        {
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            groupC->getNumChannels(&numChannels);
            for (int i = 0; i < numChannels; i++)
            {

                groupC->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("%d [ ] %s                                                                      "
                    , i + 1, soundName);
            }
            for (int i = 0; i < 14; i++)
                print_text("                                                                               ");
        }
    }

    if (this->mSound1)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
        {
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupA->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);                
                print_text("  [%C] %s                                                                      "
                    , (i == 0) ? 'X' : ' ' , soundName);
            }
            print_text("-------------------------------------------------------------------------------");
            
            groupA->getChannel(0, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);
            
            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else if (this->mSelectedGroup == 2)
        {
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupB->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 0) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupB->getChannel(0, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else
        {
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupC->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 0) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupC->getChannel(0, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
    }

    if (this->mSound2)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
        {
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupA->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 1) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupA->getChannel(1, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else if (this->mSelectedGroup == 2)
        {
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupB->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 1) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupB->getChannel(1, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else
        {
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupC->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 1) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupC->getChannel(1, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
    }

    if (this->mSound3)
    {
        print_text("===============================================================================");
        print_text("                    Select a group channel                                     ");
        if (this->mSelectedGroup == 1)
        {
            print_text("               GroupA[X]     GroupB[ ]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupA->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 2) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupA->getChannel(2, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else if (this->mSelectedGroup == 2)
        {
            print_text("               GroupA[ ]     GroupB[X]     GroupC[ ]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupB->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 2) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupB->getChannel(2, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
        else
        {
            print_text("               GroupA[ ]     GroupB[ ]     GroupC[X]                           ");
            print_text("                                                         ESCAPE to go back!    ");
            print_text("===============================================================================");
            print_text("  [X] Change aspects of a sound (volume, balance, etc)                         ");
            print_text("  [ ] Apply DSP effects to the selected group                                  ");
            print_text("-------------------------------------------------------------------------------");
            for (int i = 0; i < numChannels; i++)
            {
                groupC->getChannel(i, &chann);
                chann->getCurrentSound(&theSound);
                theSound->getName(soundName, 64);
                print_text("  [%C] %s                                                                      "
                    , (i == 2) ? 'X' : ' ', soundName);
            }
            print_text("-------------------------------------------------------------------------------");

            groupC->getChannel(2, &chann);
            chann->getVolume(&theVolume);
            chann->getMute(&isMute);
            chann->getFrequency(&theFrequency);
            chann->getPitch(&thePitch);

            print_text("1 [%c] Volume            %.1f%%                                                   ",
                this->mVolume ? 'X' : ' ', theVolume * 100);
            print_text("2 [%c] Mute              %s                                                     ",
                this->mMute ? 'X' : ' ', (isMute) ? "On" : "Off");
            if (thePan < -0.8f)                           // -1.0f
                print_text("3 [%c] Balance           L*---|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.8f && thePan < -0.6f)    // -0.75f
                print_text("3 [%c] Balance           L-*--|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.6f && thePan < -0.4f)    // -0.50f
                print_text("3 [%c] Balance           L--*-|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.4f && thePan < -0.2f)    // -0.25f
                print_text("3 [%c] Balance           L---*|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > -0.2f && thePan < 0.2f)    // 0.0f
                print_text("3 [%c] Balance           L----|----R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.2f && thePan < 0.4f)     // 0.25f
                print_text("3 [%c] Balance           L----|*---R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.4f && thePan < 0.6f)     // 0.50f
                print_text("3 [%c] Balance           L----|-*--R                                          ",
                    this->mBalance ? 'X' : ' ');
            else if (thePan > 0.6f && thePan < 0.8f)     // 0.75f
                print_text("3 [%c] Balance           L----|--*-R                                          ",
                    this->mBalance ? 'X' : ' ');
            else                                         // 1.00f
                print_text("3 [%c] Balance           L----|---*R                                          ",
                    this->mBalance ? 'X' : ' ');
            print_text("4 [%c] Frequency         %.2fHz                                                   ",
                this->mPlaySpeed ? 'X' : ' ', theFrequency);
            print_text("5 [%c] Pitch             %.2f                                                   ",
                this->mPitch ? 'X' : ' ', thePitch);
            print_text("-------------------------------------------------------------------------------");
        }
    }

    //Needed for print_text
    end_text();
}

void cConsoleState::setState(int state)
{
    if (this->mFirstLevel)
    {
        switch (state)
        {
        case 1:
            this->mFirstLevel = false;
            this->mSecondLevel = true;
            this->mSelectedGroup = state;
            return;
        case 2:
            this->mFirstLevel = false;
            this->mSecondLevel = true;
            this->mSelectedGroup = state;
            return;
        case 3:
            this->mFirstLevel = false;
            this->mSecondLevel = true;
            this->mSelectedGroup = state;
            return;
        default:
            return;
        }
    }

    if (this->mSecondLevel)
    {
        switch (state)
        {
        case 1:
            this->mSecondLevel = false;
            this->mAspects = true;
            return;
        case 2:
            this->mSecondLevel = false;
            this->mEffects = true;
            return;
        case 99:
            this->mAspects = false;
            this->mEffects = false;
            this->mFirstLevel = true;
            this->mSecondLevel = false;
            this->mSelectedGroup = 0;
            return;
        default:
            return;
        }
    }

    if (this->mEffects)
    {
        switch (state)
        {
        case 1:
            if (mSelectedGroup == 1)
                this->mDSPG1_1 = !this->mDSPG1_1;
            if (mSelectedGroup == 2)
                this->mDSPG2_1 = !this->mDSPG2_1;
            if (mSelectedGroup == 3)
                this->mDSPG3_1 = !this->mDSPG3_1;
            return;
        case 2:
            if (mSelectedGroup == 1)
                this->mDSPG1_2 = !this->mDSPG1_2;
            if (mSelectedGroup == 2)
                this->mDSPG2_2 = !this->mDSPG2_2;
            if (mSelectedGroup == 3)
                this->mDSPG3_2 = !this->mDSPG3_2;
            return;
        case 3:
            if (mSelectedGroup == 1)
                this->mDSPG1_3 = !this->mDSPG1_3;
            if (mSelectedGroup == 2)
                this->mDSPG2_3 = !this->mDSPG2_3;
            if (mSelectedGroup == 3)
                this->mDSPG3_3 = !this->mDSPG3_3;
            return;
        case 99:
            this->mEffects = false;
            this->mSecondLevel = true;
            return;
        default:
            return;
        }
    }

    if (this->mAspects)
    {
        switch (state)
        {
        case 1:
            if (numChannels > 0)
            {
                this->mAspects = false;
                this->mSound1 = true;
            }
            return;
        case 2:
            if (numChannels > 1)
            {
                this->mAspects = false;
                this->mSound2 = true;
            }
            return;
        case 3:
            if (numChannels > 2)
            {
                this->mAspects = false;
                this->mSound3 = true;
            }
            return;
        case 99:
            this->mSound1 = false;
            this->mSound2 = false;
            this->mSound3 = false;
            this->mAspects = false;
            this->mSecondLevel = true;
            return;
        default:
            return;
        }
    }

    if (this->mSound1)
    {
        switch (state)
        {
        case 1:
            this->mVolume = true;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 2:
            this->mVolume = false;
            this->mMute = true;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 3:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = true;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 4:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = true;
            this->mPitch = false;
            return;
        case 5:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = true;
            return;
        case 99:            
            this->mSound1 = false;
            this->mAspects = true;
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            thePan = 0.0f;
            return;
        default:
            return;
        }
    }

    if (this->mSound2)
    {
        switch (state)
        {
        case 1:
            this->mVolume = true;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 2:
            this->mVolume = false;
            this->mMute = true;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 3:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = true;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 4:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = true;
            this->mPitch = false;
            return;
        case 5:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = true;
            return;
        case 99:
            this->mSound2 = false;
            this->mAspects = true;
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            thePan = 0.0f;
            return;
        default:
            return;
        }
    }

    if (this->mSound3)
    {
        switch (state)
        {
        case 1:
            this->mVolume = true;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 2:
            this->mVolume = false;
            this->mMute = true;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 3:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = true;
            this->mPlaySpeed = false;
            this->mPitch = false;
            return;
        case 4:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = true;
            this->mPitch = false;
            return;
        case 5:
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = true;
            return;
        case 99:
            this->mSound3 = false;
            this->mAspects = true;
            this->mVolume = false;
            this->mMute = false;
            this->mBalance = false;
            this->mPlaySpeed = false;
            this->mPitch = false;
            thePan = 0.0f;
            return;
        default:
            return;
        }
    }

}

void cConsoleState::handle_keyboard()
{

    //===============================================================================================
    //Esc key pressed
    if (GetAsyncKeyState(VK_ESCAPE) && !mkeydown) {
        mkeydown = true;

        this->setState(99);
       
        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    // Space bar pressed		
    if ((GetKeyState(VK_SPACE) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;
    }


    //===============================================================================================
    //Arrow UP
    else if ((GetKeyState(VK_UP) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Arrow Down
    else if ((GetKeyState(VK_DOWN) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Arrow right
    else if ((GetKeyState(VK_RIGHT) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;        
    }

    //===============================================================================================
    //Arrow left
    else if ((GetKeyState(VK_LEFT) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;        
    }

    //===============================================================================================
    //Comma
    else if ((GetKeyState(VK_OEM_COMMA) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE
        if (this->mSound1 || this->mSound2 || this->mSound3)
        {
            if (this->mVolume)
            {
                chann->getVolume(&theVolume);
                if (theVolume < 0.006f)
                {
                    theVolume = 0.0f;
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
                else theVolume -= 0.005f;
                chann->setVolume(theVolume);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mMute)
            {
                isMute = !isMute;
                chann->setMute(isMute);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mBalance)
            {
                if (thePan < -0.8f) // -1.0f
                {
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
                else
                {
                    thePan -= 0.25;
                    chann->setPan(thePan);
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
            }
            if (this->mPlaySpeed)
            {
                chann->setFrequency(theFrequency - 100.0f);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mPitch)
            {
                thePitch -= 0.01f;
                chann->setPitch(thePitch);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
        }
    }
     
    //===============================================================================================
    //Period
    else if ((GetKeyState(VK_OEM_PERIOD) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        if (this->mSound1 || this->mSound2 || this->mSound3)
        {
            if (this->mVolume)
            {
                chann->getVolume(&theVolume);
                if (theVolume > 0.994f)
                {
                    theVolume = 1.0f;
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
                else theVolume += 0.005f;
                chann->setVolume(theVolume);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mMute)
            {
                isMute = !isMute;
                chann->setMute(isMute);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mBalance)
            {
                if (thePan > 0.8f) // 1.0f
                {
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
                else
                {
                    thePan += 0.25;
                    chann->setPan(thePan);
                    Sleep(sleep_ms);
                    mkeydown = false;
                    return;
                }
            }
            if (this->mPlaySpeed)
            {
                chann->setFrequency(theFrequency + 100.0f);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
            if (this->mPitch)
            {
                thePitch += 0.01f;
                chann->setPitch(thePitch);
                Sleep(sleep_ms);
                mkeydown = false;
                return;
            }
        }
    }

    //===============================================================================================
    //Number 0
    else if ((GetKeyState(0x30) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 1
    else if ((GetKeyState(0x31) < 0) && !mkeydown) { //Key down
        mkeydown = true;
        //YOUR CODE HERE
        this->setState(1);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 2
    else if ((GetKeyState(0x32) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE	
        this->setState(2);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 3
    else if ((GetKeyState(0x33) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(3);
       
        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 4
    else if ((GetKeyState(0x34) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(4);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 5
    else if ((GetKeyState(0x35) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(5);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 6
    else if ((GetKeyState(0x36) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(6);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 7
    else if ((GetKeyState(0x37) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(7);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 8
    else if ((GetKeyState(0x38) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(8);

        Sleep(sleep_ms);
        mkeydown = false;
    }

    //===============================================================================================
    //Number 9
    else if ((GetKeyState(0x39) < 0) && !mkeydown) {
        mkeydown = true;
        //YOUR CODE HERE		
        this->setState(9);

        Sleep(sleep_ms);
        mkeydown = false;
    }

}