#include "cSoundManager_Imp.h"
#include <fstream>
#include <iostream>

// Function signature
void errorcheck(FMOD_RESULT result);
int get_FMOD_Type(std::string token);

cSoundManager_Imp::cSoundManager_Imp()
{
    this->msystem = NULL;
    this->mastergroup = 0;
    this->dsplowpass = 0;
    this->dsphighpass = 0;
    this->dspecho = 0;
    this->dspflange = 0;
    this->mlistenerposition = { 0.0f, 0.0f, -5.0f };
    this->mforward = { 0.0f, 0.0f, 1.0f };
    this->mup = { 0.0f, 1.0f, 0.0f };
    this->mvel = { 0.0f, 0.0f, 0.0f };
}

cSoundManager_Imp::~cSoundManager_Imp()
{
    //Shutdown
    for (int i = 0; i < this->msounds.size(); i++)
    {
        if (msounds[i])
        {
            msounds[i]->release();
            errorcheck(this->mresult);
        }
    }

    if (this->msystem) {
        this->mresult = this->msystem->close();
        errorcheck(this->mresult);
        this->mresult = this->msystem->release();
        errorcheck(this->mresult);
    }
}

void cSoundManager_Imp::loadConfigFile(std::string file)
{
    return;
}

void cSoundManager_Imp::loadSoundObjectsIntoScene()
{
    return;
}

void cSoundManager_Imp::initSoundScene()
{

    if (!loadSoundParameters("../sounds/sound.conf"))
    {
        std::cout << "Something went wrong while loading the sound configuration!\n";
    }

    // Create the sounds and channels
    const int NUM_OF_ELEMENTS = 2;

    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        FMOD::Sound* tempSd;
        this->msounds.push_back(tempSd);
        FMOD::Channel* tempCh;
        this->mchannels.push_back(tempCh);
        cSoudObject* tempSO;
        this->soundObjects.push_back(tempSO);
    }

    init_fmod();

    ////Get master channel group
    //mresult = msystem->getMasterChannelGroup(&mastergroup);
    //errorcheck(mresult);

    ////Create DSP effects
    //mresult = msystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpass);
    //errorcheck(mresult);
    //mresult = msystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpass);
    //errorcheck(mresult);
    //mresult = msystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspecho);
    //errorcheck(mresult);
    //mresult = msystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflange);
    //errorcheck(mresult);

    ////Add effects to master channel group.
    //mresult = mastergroup->addDSP(0, dsplowpass);
    //errorcheck(mresult);
    //mresult = mastergroup->addDSP(0, dsphighpass);
    //errorcheck(mresult);
    //mresult = mastergroup->addDSP(0, dspecho);
    //errorcheck(mresult);
    //mresult = mastergroup->addDSP(0, dspflange);
    //errorcheck(mresult);

    ////Bypass all effects, this plays the sound with no effects.
    //mresult = dsplowpass->setBypass(true);
    //errorcheck(mresult);
    //mresult = dsphighpass->setBypass(true);;
    //errorcheck(mresult);
    //mresult = dspecho->setBypass(true);
    //errorcheck(mresult);
    //mresult = dspflange->setBypass(true);
    //errorcheck(mresult);


    mresult = msystem->createSound(this->soundObjects[0]->getSource().c_str(), this->soundObjects[0]->getFMODType(), 0, &msounds[0]);
    errorcheck(mresult);
    mresult = msounds[0]->setMode(FMOD_LOOP_NORMAL);
    errorcheck(mresult);


    //play sound 
    mresult = msystem->playSound(msounds[0], 0, false, &mchannels[0]);
    errorcheck(mresult);

    mresult = mchannels[0]->setVolume(0.005f);
    errorcheck(mresult);


    //we need position and velocity vecors for 3d sounds
    glm::vec3 soundPos = this->soundObjects[1]->getPosition();
    FMOD_VECTOR sound_position;
    sound_position.x = soundPos.x;
    sound_position.y = soundPos.y;
    sound_position.z = soundPos.z;
    FMOD_VECTOR sound_velocity = { 0.0f, 0.0f, 0.0f };

    mresult = msystem->createSound(this->soundObjects[1]->getSource().c_str(), this->soundObjects[1]->getFMODType(), 0, &msounds[1]);
    errorcheck(mresult);
    mresult = msounds[1]->setMode(FMOD_LOOP_NORMAL);
    errorcheck(mresult);

    mresult = msounds[1]->set3DMinMaxDistance(0.3f, 5000.0f);
    errorcheck(mresult);

    //play sound as paused, we will set 3d settings after
    mresult = msystem->playSound(msounds[1], 0, true, &mchannels[1]);
    errorcheck(mresult);
    //set 3d attributes
    mresult = mchannels[1]->set3DAttributes(&sound_position, &sound_velocity);
    errorcheck(mresult);
    //unpause
    mresult = mchannels[1]->setPaused(false);
    errorcheck(mresult);


}

void cSoundManager_Imp::updateSoundScene(glm::vec3 listener)
{
    // Update the listener position
    this->mlistenerposition.x = listener.x;
    this->mlistenerposition.y = listener.y;
    this->mlistenerposition.z = listener.z;

    //Important to update msystem
    mresult = msystem->set3DListenerAttributes(0, &mlistenerposition, &mvel, &mforward, &mup);
    mresult = msystem->update();
    errorcheck(mresult);
}

bool cSoundManager_Imp::loadSoundParameters(std::string configFile)
{


    std::ifstream file(configFile.c_str());

    if (!file.is_open())
    {	
        return false;
    }

    std::string token;
    file >> token;

    while (!file.eof())
    {

        if (token.at(0) == '#')
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            file >> token;
            continue;
        }

        cSoudObject* so = new cSoudObject();

        int fmod_type = get_FMOD_Type(token);
        
        if (fmod_type == FMOD_3D)
        {
            so->setFMODType(FMOD_3D);
            file >> token;
            so->setfriendlyName(token);
            float xPos, yPos, zPos;
            file >> xPos >> yPos >> zPos;
            glm::vec3 position(xPos, yPos, zPos);
            so->setPosition(position);
            file >> token;
            so->setSource(token);
            this->soundObjects.push_back(so);
            continue;
        }

        so->setFMODType(fmod_type);
        file >> token;
        so->setfriendlyName(token);
        file >> token;
        so->setSource(token);
        this->soundObjects.push_back(so);

        file >> token;

    }


    return true;


}

void errorcheck(FMOD_RESULT result) {
    if (result != FMOD_OK)
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void cSoundManager_Imp::init_fmod() {

    // Create the main system object.
    this->mresult = FMOD::System_Create(&this->msystem);
    if (this->mresult != FMOD_OK)
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", this->mresult, FMOD_ErrorString(this->mresult));
        exit(-1);
    }

    //Initializes the system object, and the msound device. This has to be called at the start of the user's program
    this->mresult = this->msystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (this->mresult != FMOD_OK)
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", this->mresult, FMOD_ErrorString(this->mresult));
        exit(-1);
    }

}

int get_FMOD_Type(std::string token)
{
    if (token == "FMOD_DEFAULT"                  ) return FMOD_DEFAULT                 ;    
    if (token == "FMOD_LOOP_OFF"                 ) return FMOD_LOOP_OFF                ;
    if (token == "FMOD_LOOP_NORMAL"              ) return FMOD_LOOP_NORMAL             ;
    if (token == "FMOD_LOOP_BIDI"                ) return FMOD_LOOP_BIDI               ;
    if (token == "FMOD_2D"                       ) return FMOD_2D                      ;
    if (token == "FMOD_3D"                       ) return FMOD_3D                      ;
    if (token == "FMOD_CREATESTREAM"             ) return FMOD_CREATESTREAM            ;
    if (token == "FMOD_CREATESAMPLE"             ) return FMOD_CREATESAMPLE            ;
    if (token == "FMOD_CREATECOMPRESSEDSAMPLE"   ) return FMOD_CREATECOMPRESSEDSAMPLE  ;
    if (token == "FMOD_OPENUSER"                 ) return FMOD_OPENUSER                ;
    if (token == "FMOD_OPENMEMORY"               ) return FMOD_OPENMEMORY              ;
    if (token == "FMOD_OPENMEMORY_POINT"         ) return FMOD_OPENMEMORY_POINT        ;
    if (token == "FMOD_OPENRAW"                  ) return FMOD_OPENRAW                 ;
    if (token == "FMOD_OPENONLY"                 ) return FMOD_OPENONLY                ;
    if (token == "FMOD_ACCURATETIME"             ) return FMOD_ACCURATETIME            ;
    if (token == "FMOD_MPEGSEARCH"               ) return FMOD_MPEGSEARCH              ;
    if (token == "FMOD_NONBLOCKING"              ) return FMOD_NONBLOCKING             ;
    if (token == "FMOD_UNIQUE"                   ) return FMOD_UNIQUE                  ;
    if (token == "FMOD_3D_HEADRELATIVE"          ) return FMOD_3D_HEADRELATIVE         ;
    if (token == "FMOD_3D_WORLDRELATIVE"         ) return FMOD_3D_WORLDRELATIVE        ;
    if (token == "FMOD_3D_INVERSEROLLOFF"        ) return FMOD_3D_INVERSEROLLOFF       ;
    if (token == "FMOD_3D_LINEARROLLOFF"         ) return FMOD_3D_LINEARROLLOFF        ;
    if (token == "FMOD_3D_LINEARSQUAREROLLOFF"   ) return FMOD_3D_LINEARSQUAREROLLOFF  ;
    if (token == "FMOD_3D_INVERSETAPEREDROLLOFF" ) return FMOD_3D_INVERSETAPEREDROLLOFF;
    if (token == "FMOD_3D_CUSTOMROLLOFF"         ) return FMOD_3D_CUSTOMROLLOFF        ;
    if (token == "FMOD_3D_IGNOREGEOMETRY"        ) return FMOD_3D_IGNOREGEOMETRY       ;
    if (token == "FMOD_IGNORETAGS"               ) return FMOD_IGNORETAGS              ;
    if (token == "FMOD_LOWMEM"                   ) return FMOD_LOWMEM                  ;
    if (token == "FMOD_LOADSECONDARYRAM"         ) return FMOD_LOADSECONDARYRAM        ;
    if (token == "FMOD_VIRTUAL_PLAYFROMSTART"    ) return FMOD_VIRTUAL_PLAYFROMSTART   ;
}