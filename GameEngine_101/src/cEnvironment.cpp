#include "cEnvironment.h"
#include "cLightManager.h"

cEnvironment::cEnvironment()
{
    m_mode = STEP;
    m_dayDuration = 15.0f;
    m_elapsedTime = 0.0f;
    m_timeOfDay = 0.0f;

    // Create all the lights
    m_midNight = new cLight();
    m_midNight->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_midNight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    m_midNight->attenuation = glm::vec3(0.0f);
    m_midNight->attenuation.x = 0.8f;
    m_midNight->typeParams = glm::vec4(0.0f);
    m_midNight->typeParams.y = 1000000.0f;
    m_midNight->typeParams2 = glm::vec4(0.0f);

    m_dawn = new cLight();
    m_dawn->position = glm::vec3(100.0f, 100.0f, 0.0f);
    m_dawn->diffuse = glm::vec3(1.0f, 1.0f, 0.79f);
    m_dawn->attenuation = glm::vec3(0.0f);
    m_dawn->attenuation.x = 0.8f;
    m_dawn->typeParams = glm::vec4(0.0f);
    m_dawn->typeParams.y = 1000000.0f;
    m_dawn->typeParams2 = glm::vec4(0.0f);

    m_noon = new cLight();
    m_noon->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_noon->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    m_noon->attenuation = glm::vec3(0.0f);
    m_noon->attenuation.x = 0.8f;
    m_noon->typeParams = glm::vec4(0.0f);
    m_noon->typeParams.y = 1000000.0f;
    m_noon->typeParams2 = glm::vec4(0.0f);

    m_sunset = new cLight();
    m_sunset->position = glm::vec3(-100.0f, 100.0f, 0.0f);
    m_sunset->diffuse = glm::vec3(1.0f, 0.925f, 0.925f);
    m_sunset->attenuation = glm::vec3(0.0f);
    m_sunset->attenuation.x = 0.8f;
    m_sunset->typeParams = glm::vec4(0.0f);
    m_sunset->typeParams.y = 1000000.0f;
    m_sunset->typeParams2 = glm::vec4(0.0f);

    m_night = new cLight();
    m_night->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_night->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    m_night->attenuation = glm::vec3(0.0f);
    m_night->attenuation.x = 0.8f;
    m_night->typeParams = glm::vec4(0.0f);
    m_night->typeParams.y = 1000000.0f;
    m_night->typeParams2 = glm::vec4(0.0f);
    
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::initLights(int shaderID, cLightManager* lightManager)
{
    std::vector<cLight*> lights;
    lights.push_back(m_midNight);
    lights.push_back(m_dawn);
    lights.push_back(m_noon);
    lights.push_back(m_sunset);
    lights.push_back(m_night);

    lightManager->CreateLights(lights);
    lightManager->LoadShaderUniformLocations(shaderID);
}

void cEnvironment::setDayDuration(float minutes)
{
    if (minutes < 0.0f)
        return;

    m_dayDuration = minutes * 60.0f;
}

void cEnvironment::setTimeOfDay(float hour)
{
    if (hour == 24.0f)
        hour = 0.0f;

    if (hour >= 0.0f && hour < 24.0f)
    {
        m_timeOfDay = hour / 24.0f;

        if (hour >= 0.0f && hour < 6.0f)
        {
            m_midNight->typeParams2.x = (6.0f - hour) / 6.0f;
            m_dawn->typeParams2.x = hour / 6.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 6.0f && hour < 12.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = (12.0f - hour) / 6.0f;
            m_noon->typeParams2.x = (hour - 6.0f) / 6.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 12.0f && hour < 18.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = (18.0f - hour) / 6.0f;
            m_sunset->typeParams2.x = (hour - 12.0f) / 6.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 18.0f && hour < 21.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = (21.0f - hour) / 3.0f;
            m_night->typeParams2.x = (hour - 18.0f) / 3.0f;
        }
        else
        {
            m_midNight->typeParams2.x = (hour - 21.0f) / 3.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = (24.0f - hour) / 3.0f;
        }
    }

    // To ensure we continue from where we set the time
    m_elapsedTime = m_timeOfDay * m_dayDuration;
}

void cEnvironment::setTimeOfDay(DaySkyLight daySkyLight)
{
    switch (daySkyLight)
    {
    case cEnvironment::MIDNIGHT:
        m_timeOfDay = 0.0f;
        m_midNight->typeParams2.x = 1.0f;
        m_dawn->typeParams2.x = 0.0f;
        m_noon->typeParams2.x = 0.0f;
        m_sunset->typeParams2.x = 0.0f;
        m_night->typeParams2.x = 0.0f;
        break;
    case cEnvironment::DAWN:
        m_timeOfDay = 0.25f;
        m_midNight->typeParams2.x = 0.0f;
        m_dawn->typeParams2.x = 1.0f;
        m_noon->typeParams2.x = 0.0f;
        m_sunset->typeParams2.x = 0.0f;
        m_night->typeParams2.x = 0.0f;
        break;
    case cEnvironment::NOON:
        m_timeOfDay = 0.5f;
        m_midNight->typeParams2.x = 0.0f;
        m_dawn->typeParams2.x = 0.0f;
        m_noon->typeParams2.x = 1.0f;
        m_sunset->typeParams2.x = 0.0f;
        m_night->typeParams2.x = 0.0f;
        break;
    case cEnvironment::SUNSET:
        m_timeOfDay = 0.75f;
        m_midNight->typeParams2.x = 0.0f;
        m_dawn->typeParams2.x = 0.0f;
        m_noon->typeParams2.x = 0.0f;
        m_sunset->typeParams2.x = 1.0f;
        m_night->typeParams2.x = 0.0f;
        break;
    case cEnvironment::NIGHT:
        m_timeOfDay = 0.875f;
        m_midNight->typeParams2.x = 0.0f;
        m_dawn->typeParams2.x = 0.0f;
        m_noon->typeParams2.x = 0.0f;
        m_sunset->typeParams2.x = 0.0f;
        m_night->typeParams2.x = 1.0f;
        break;
    default:
        break;
    }

    // To ensure we continue from where we set the time
    m_elapsedTime = m_timeOfDay * m_dayDuration;
}

void cEnvironment::update(float deltaTime)
{
    if (m_mode == CONTINUOUS)
    {
        m_elapsedTime += deltaTime;
        m_timeOfDay = fmod(m_elapsedTime, m_dayDuration) / m_dayDuration;

        float hour = m_timeOfDay * 24.0f;
        if (hour >= 0.0f && hour < 6.0f)
        {
            m_midNight->typeParams2.x = (6.0f - hour) / 6.0f;
            m_dawn->typeParams2.x = hour / 6.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 6.0f && hour < 12.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = (12.0f - hour) / 6.0f;
            m_noon->typeParams2.x = (hour - 6.0f) / 6.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 12.0f && hour < 18.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = (18.0f - hour) / 6.0f;
            m_sunset->typeParams2.x = (hour - 12.0f) / 6.0f;
            m_night->typeParams2.x = 0.0f;
        }
        else if (hour >= 18.0f && hour < 21.0f)
        {
            m_midNight->typeParams2.x = 0.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = (21.0f - hour) / 3.0f;
            m_night->typeParams2.x = (hour - 18.0f) / 3.0f;
        }
        else
        {
            m_midNight->typeParams2.x = (hour - 21.0f) / 3.0f;
            m_dawn->typeParams2.x = 0.0f;
            m_noon->typeParams2.x = 0.0f;
            m_sunset->typeParams2.x = 0.0f;
            m_night->typeParams2.x = (24.0f - hour) / 3.0f;
        }
    }
}
