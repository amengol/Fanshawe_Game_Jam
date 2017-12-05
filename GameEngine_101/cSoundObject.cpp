#include "cSoundObject.h"
#include <fmod\fmod.hpp>

cSoudObject::cSoudObject()
{
    this->FMOD_Type = FMOD_DEFAULT;
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
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