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
    // Load sound configuration file
    if (!loadSoundParameters("../sounds/sound.conf"))
    {
        std::cout << "Something went wrong while loading the sound configuration!\n";
        system("pause");
        exit(-1);
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

    // Sounds initializations
    for (int i = 0; i < this->soundObjects.size(); i++)
    {
        FMOD::Sound* tempSd;
        this->msounds.push_back(tempSd);
        FMOD::Channel* tempCh;
        this->mchannels.push_back(tempCh);

        if (soundObjects.at(i)->getFMODType() == FMOD_3D)
        {
            //we need position and velocity vecors for 3d sounds
            glm::vec3 soundPos = this->soundObjects[i]->getPosition();
            FMOD_VECTOR sound_position;
            sound_position.x = soundPos.x;
            sound_position.y = soundPos.y;
            sound_position.z = soundPos.z;
            FMOD_VECTOR sound_velocity = { 0.0f, 0.0f, 0.0f };

            mresult = msystem->createSound(this->soundObjects[i]->getSource().c_str(), FMOD_3D, 0, &msounds[i]);
            errorcheck(mresult);
            mresult = msounds[i]->setMode(FMOD_LOOP_NORMAL);
            errorcheck(mresult);

            mresult = msounds[i]->set3DMinMaxDistance(0.3f, 5000.0f);
            errorcheck(mresult);

            //play sound as paused, we will set 3d settings after
            mresult = msystem->playSound(msounds[i], 0, true, &mchannels[i]);
            errorcheck(mresult);
            //set 3d attributes
            mresult = mchannels[i]->set3DAttributes(&sound_position, &sound_velocity);
            errorcheck(mresult);
            //unpause
            mresult = mchannels[i]->setPaused(false);
            errorcheck(mresult);

        }
        else
        {
            mresult = msystem->createSound(this->soundObjects[i]->getSource().c_str(), this->soundObjects[i]->getFMODType(), 0, &msounds[i]);
            errorcheck(mresult);
            mresult = msounds[i]->setMode(FMOD_LOOP_NORMAL);
            errorcheck(mresult);


            //play sound 
            mresult = msystem->playSound(msounds[i], 0, false, &mchannels[i]);
            errorcheck(mresult);

            mresult = mchannels[i]->setVolume(0.005f);
            errorcheck(mresult);
        }



    }
    
    if (this->soundObjects.size() == 0)
    {
        std::cout << "There were no sound objects to load!\n";
        system("pause");
        exit(-1);
    }



    



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

    int line = 0; // Keep track of what line we are reading

    while (!file.eof())
    {
        line++;

        if (token.at(0) == '#')
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            file >> token;
            continue;
        }

        cSoudObject* so = new cSoudObject();

        int fmod_type = get_FMOD_Type(token);

        if (fmod_type == -1)
        {
            std::cout << "Invalid type at line " << line
                << " in the sound configuration file"
                << std::endl;
            return false;
        }
        
        if (fmod_type == FMOD_3D)
        {
            so->setFMODType(FMOD_3D);
            file >> token;
            so->setfriendlyName(token);
            float xPos, yPos, zPos;
            if (!(file >> xPos >> yPos >> zPos))
            {
                std::cout << "There were a problem reading a model "
                    "coordinate in the config file at line "
                    << line << std::endl;
                return false;
            }
            glm::vec3 position(xPos, yPos, zPos);
            so->setPosition(position);
            file >> token;
            so->setSource(token);
            this->soundObjects.push_back(so);
            file >> token;
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
        system("pause");
        exit(-1);
    }
}

void cSoundManager_Imp::init_fmod() {

    // Create the main system object.
    this->mresult = FMOD::System_Create(&this->msystem);
    if (this->mresult != FMOD_OK)
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", this->mresult, FMOD_ErrorString(this->mresult));
        system("pause");
        exit(-1);
    }

    //Initializes the system object, and the msound device. This has to be called at the start of the user's program
    this->mresult = this->msystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (this->mresult != FMOD_OK)
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", this->mresult, FMOD_ErrorString(this->mresult));
        system("pause");
        exit(-1);
    }

}

int get_FMOD_Type(std::string token)
{
    if      (token == "FMOD_DEFAULT"                  ) return FMOD_DEFAULT                 ;    
    else if (token == "FMOD_LOOP_OFF"                 ) return FMOD_LOOP_OFF                ;
    else if (token == "FMOD_LOOP_NORMAL"              ) return FMOD_LOOP_NORMAL             ;
    else if (token == "FMOD_LOOP_BIDI"                ) return FMOD_LOOP_BIDI               ;
    else if (token == "FMOD_2D"                       ) return FMOD_2D                      ;
    else if (token == "FMOD_3D"                       ) return FMOD_3D                      ;
    else if (token == "FMOD_CREATESTREAM"             ) return FMOD_CREATESTREAM            ;
    else if (token == "FMOD_CREATESAMPLE"             ) return FMOD_CREATESAMPLE            ;
    else if (token == "FMOD_CREATECOMPRESSEDSAMPLE"   ) return FMOD_CREATECOMPRESSEDSAMPLE  ;
    else if (token == "FMOD_OPENUSER"                 ) return FMOD_OPENUSER                ;
    else if (token == "FMOD_OPENMEMORY"               ) return FMOD_OPENMEMORY              ;
    else if (token == "FMOD_OPENMEMORY_POINT"         ) return FMOD_OPENMEMORY_POINT        ;
    else if (token == "FMOD_OPENRAW"                  ) return FMOD_OPENRAW                 ;
    else if (token == "FMOD_OPENONLY"                 ) return FMOD_OPENONLY                ;
    else if (token == "FMOD_ACCURATETIME"             ) return FMOD_ACCURATETIME            ;
    else if (token == "FMOD_MPEGSEARCH"               ) return FMOD_MPEGSEARCH              ;
    else if (token == "FMOD_NONBLOCKING"              ) return FMOD_NONBLOCKING             ;
    else if (token == "FMOD_UNIQUE"                   ) return FMOD_UNIQUE                  ;
    else if (token == "FMOD_3D_HEADRELATIVE"          ) return FMOD_3D_HEADRELATIVE         ;
    else if (token == "FMOD_3D_WORLDRELATIVE"         ) return FMOD_3D_WORLDRELATIVE        ;
    else if (token == "FMOD_3D_INVERSEROLLOFF"        ) return FMOD_3D_INVERSEROLLOFF       ;
    else if (token == "FMOD_3D_LINEARROLLOFF"         ) return FMOD_3D_LINEARROLLOFF        ;
    else if (token == "FMOD_3D_LINEARSQUAREROLLOFF"   ) return FMOD_3D_LINEARSQUAREROLLOFF  ;
    else if (token == "FMOD_3D_INVERSETAPEREDROLLOFF" ) return FMOD_3D_INVERSETAPEREDROLLOFF;
    else if (token == "FMOD_3D_CUSTOMROLLOFF"         ) return FMOD_3D_CUSTOMROLLOFF        ;
    else if (token == "FMOD_3D_IGNOREGEOMETRY"        ) return FMOD_3D_IGNOREGEOMETRY       ;
    else if (token == "FMOD_IGNORETAGS"               ) return FMOD_IGNORETAGS              ;
    else if (token == "FMOD_LOWMEM"                   ) return FMOD_LOWMEM                  ;
    else if (token == "FMOD_LOADSECONDARYRAM"         ) return FMOD_LOADSECONDARYRAM        ;
    else if (token == "FMOD_VIRTUAL_PLAYFROMSTART"    ) return FMOD_VIRTUAL_PLAYFROMSTART   ;
    else return -1;
}