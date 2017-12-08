#include "cSoundObject.h"
#include <fmod\fmod.hpp>

cSoudObject::cSoudObject()
{
    this->FMOD_Type = FMOD_DEFAULT;
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->volume = 1.0f;
    this->isMute = false;
    this->movType = 1;
}

cSoudObject::~cSoudObject()
{
    return;
}

void cSoudObject::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void cSoudObject::setfriendlyName(std::string name)
{
    this->friendlyName = name;
}

void cSoudObject::setSource(std::string src)
{
    this->source = src;
}

void cSoudObject::setFMODType(int type)
{
    this->FMOD_Type = type;
}

void cSoudObject::setVolume(float vol)
{
    if (vol > 1.0)
        this->volume = 1.0f;
    else if (vol < 0.0f)
        this->volume = 0.0f;
    else
        this->volume = vol;
}

void cSoudObject::setMute(bool mute)
{
    this->isMute = mute;
}

void cSoudObject::setMovType(int type)
{
    this->movType = type;
}

glm::vec3 cSoudObject::getPosition()
{
    return this->position;
}

std::string cSoudObject::getFriendlyName()
{
    return this->friendlyName;
}

std::string cSoudObject::getSource()
{
    return this->source;
}

int cSoudObject::getFMODType()
{
    return this->FMOD_Type;
}

float cSoudObject::getVolume()
{
    return this->volume;
}

bool cSoudObject::getMute()
{
    return this->isMute;
}

int cSoudObject::getMovType()
{
    return this->movType;
}