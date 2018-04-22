#include "cSoundObject.h"
#include <fmod\fmod.hpp>

cSoundObject::cSoundObject()
{
    this->FMOD_Type = FMOD_DEFAULT;
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->volume = 1.0f;
    this->isMute = false;
    this->movType = 1;
    this->m_isPaused = true;
    this->m_isStopped = true;
    this->m_isLooping = false;
}

cSoundObject::~cSoundObject()
{
    return;
}

void cSoundObject::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void cSoundObject::setfriendlyName(std::string name)
{
    this->friendlyName = name;
}

void cSoundObject::setSource(std::string src)
{
    this->source = src;
}

void cSoundObject::setFMODType(int type)
{
    this->FMOD_Type = type;
}

void cSoundObject::setVolume(float vol)
{
    if (vol > 1.0)
        this->volume = 1.0f;
    else if (vol < 0.0f)
        this->volume = 0.0f;
    else
        this->volume = vol;
}

void cSoundObject::setMute(bool mute)
{
    this->isMute = mute;
}

void cSoundObject::setMovType(int type)
{
    this->movType = type;
}

void cSoundObject::play()
{
    this->m_isPaused = false;
    this->m_isStopped = false;
}

void cSoundObject::pause()
{
    this->m_isPaused = true;
    this->m_isStopped = false;
}

void cSoundObject::stop()
{
    this->m_isPaused = true;
    this->m_isStopped = true;
}

glm::vec3 cSoundObject::getPosition()
{
    return this->position;
}

std::string cSoundObject::getFriendlyName()
{
    return this->friendlyName;
}

std::string cSoundObject::getSource()
{
    return this->source;
}

int cSoundObject::getFMODType()
{
    return this->FMOD_Type;
}

float cSoundObject::getVolume()
{
    return this->volume;
}

bool cSoundObject::getMute()
{
    return this->isMute;
}

int cSoundObject::getMovType()
{
    return this->movType;
}