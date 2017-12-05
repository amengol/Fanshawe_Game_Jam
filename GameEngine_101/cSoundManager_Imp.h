#ifndef _cSoundManager_Imp_HG_
#define _cSoundManager_Imp_HG_

#include <vector>
#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>
#include <glm\vec3.hpp>
#include "cSoundObject.h"

class cSoundManager_Imp
{
public:
    cSoundManager_Imp();
    ~cSoundManager_Imp();

    void loadConfigFile(std::string file);
    void loadSoundObjectsIntoScene();
    void initSoundScene();
    void updateSoundScene(glm::vec3 listener);
    bool loadSoundParameters(std::string configFile);

    //Only here functions
    void init_fmod();

private:
    
    std::vector<cSoudObject*> soundObjects;

    FMOD_RESULT mresult;
    FMOD::System* msystem;
    std::vector<FMOD::Sound*> msounds;
    std::vector<FMOD::Channel*> mchannels;
    //Master channel group
    FMOD::ChannelGroup* mastergroup;

    //DSP variables
    FMOD::DSP* dsplowpass;
    FMOD::DSP* dsphighpass;
    FMOD::DSP* dspecho;
    FMOD::DSP* dspflange;

    FMOD_VECTOR mlistenerposition;
    FMOD_VECTOR mforward;
    FMOD_VECTOR mup;
    FMOD_VECTOR mvel;

};


#endif // !_cSoundManager_Imp_HG_

