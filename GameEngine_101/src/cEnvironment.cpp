#include "cEnvironment.h"
#include "cLightManager.h"

cEnvironment::cEnvironment()
{
    m_mode = STEP;
    m_daySkyLight = NOON;
    m_dayDuration = 15.0f;
    m_elapsedTime = 0.0f;

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

void cEnvironment::update(float deltaTime)
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
