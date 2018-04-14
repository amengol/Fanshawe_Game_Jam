#include "cEnvironment.h"
#include "cLightManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\rotate_vector.hpp>

cEnvironment::cEnvironment()
{
    m_mode = STEP;
    m_dayDuration = 15.0f;
    m_elapsedTime = 0.0f;
    m_timeOfDay = 0.0f;

    m_colourMidnight = glm::vec3(0.2f, 0.2f, 0.2f);
    m_colourDawn = glm::vec3(0.5f, 0.5f, 0.395f);
    m_colourNoon = glm::vec3(1.0f, 1.0f, 1.0f);
    m_colourSunset = glm::vec3(1.0f, 0.925f, 0.925f);
    m_colourNight = glm::vec3(0.3f, 0.3f, 0.3f);

    // Create all the lights
    m_sunLight = new cLight();
    m_sunLight->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_sunLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    m_sunLight->specular = glm::vec4(0.0f);
    m_sunLight->attenuation = glm::vec3(0.0f);
    m_sunLight->attenuation.x = 1.0f;
    m_sunLight->typeParams = glm::vec4(0.0f);
    m_sunLight->typeParams.y = 1000.0f;
    m_sunLight->typeParams2 = glm::vec4(0.0f);

    m_moonLight = new cLight();
    m_moonLight->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_moonLight->specular = glm::vec4(0.0f);
    m_moonLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    m_moonLight->attenuation = glm::vec3(0.0f);
    m_moonLight->attenuation.x = 1.0f;
    m_moonLight->typeParams = glm::vec4(0.0f);
    m_moonLight->typeParams.y = 1000.0f;
    m_moonLight->typeParams2 = glm::vec4(0.0f);
    
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::initLights(int shaderID, cLightManager* lightManager)
{
    std::vector<cLight*> lights;
    lights.push_back(m_sunLight);
    lights.push_back(m_moonLight);

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
            m_midNight = (6.0f - hour) / 6.0f;
            m_dawn = hour / 6.0f;
            m_noon = 0.0f;
            m_sunset = 0.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (6.0f - hour) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourDawn + factor * m_colourMidnight;
            m_moonLight->diffuse = colourLight;
        }
        else if (hour >= 6.0f && hour < 12.0f)
        {
            m_midNight = 0.0f;
            m_dawn = (12.0f - hour) / 6.0f;
            m_noon = (hour - 6.0f) / 6.0f;
            m_sunset = 0.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 1.0f; // Power
            m_sunLight->specular = glm::vec4(1.0f);
            float factor = (hour - 6.0f) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourDawn + factor * m_colourNoon;
            m_sunLight->diffuse = colourLight;

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 0.0f; // Power
            m_moonLight->specular = glm::vec4(0.0f);
        }
        else if (hour >= 12.0f && hour < 18.0f)
        {
            m_midNight = 0.0f;
            m_dawn = 0.0f;
            m_noon = (18.0f - hour) / 6.0f;
            m_sunset = (hour - 12.0f) / 6.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 1.0f; // Power
            m_sunLight->specular = glm::vec4(1.0f);
            float factor = (hour - 12.0f) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourNoon + factor * m_colourSunset;
            m_sunLight->diffuse = colourLight;

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 0.0f; // Power
            m_moonLight->specular = glm::vec4(0.0f);

        }
        else if (hour >= 18.0f && hour < 21.0f)
        {
            m_midNight = 0.0f;
            m_dawn = 0.0f;
            m_noon = 0.0f;
            m_sunset = (21.0f - hour) / 3.0f;
            m_night = (hour - 18.0f) / 3.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (hour - 18.0f) / 3.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourSunset + factor * m_colourNight;
            m_moonLight->diffuse = colourLight;
        }
        else
        {
            m_midNight = (hour - 21.0f) / 3.0f;
            m_dawn = 0.0f;
            m_noon = 0.0f;
            m_sunset = 0.0f;
            m_night = (24.0f - hour) / 3.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (hour - 21.0f) / 3.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourNight + factor * m_colourMidnight;
            m_moonLight->diffuse = colourLight;
        }
    }

    // To ensure we continue from where we set the time
    m_elapsedTime = m_timeOfDay * m_dayDuration;
}

glm::vec3 cEnvironment::getSunPosition()
{
    float hour = m_timeOfDay * 24.0f;

    // Sunlight 6am-6pm
    if (hour >= 6.0f && hour <= 18.0f)
    {
        float dayPercent = (hour - 6.0f) / 12.0f;
        float angle = glm::radians(dayPercent * 180.0f);
        glm::vec3 sunDirection = glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
        return sunDirection * 200.0f;
    }
    else
    {
        return glm::vec3(-10.0f);
    }
    

}

glm::vec3 cEnvironment::getMoonPosition()
{
    float hour = m_timeOfDay * 24.0f;

    // Moonlight 6pm-6am
    if (hour < 6.0f)
    {
        float nightPercent = (hour / 6.0f);
        float angle = glm::radians(nightPercent * 90.0f + 90.0f);
        glm::vec3 moonDirection = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
        return moonDirection * 200.0f;
    }
    else if (hour > 18.0f)
    {
        float nightPercent = 1 - (24.0f - hour) / 6.0f;
        float angle = glm::radians(nightPercent * 90.0f);
        glm::vec3 moonDirection = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
        return moonDirection * 200.0f;
    }
    else
    {
        return glm::vec3(-10.0f);
    }
}

void cEnvironment::setTimeOfDay(DaySkyLight daySkyLight)
{
    switch (daySkyLight)
    {
    case cEnvironment::MIDNIGHT:
        m_timeOfDay = 0.0f;
        m_midNight = 1.0f;
        m_dawn = 0.0f;
        m_noon = 0.0f;
        m_sunset = 0.0f;
        m_night = 0.0f;

        // Lights control
        m_sunLight->position = getSunPosition();
        m_sunLight->typeParams2.x = 0.0f; // Power
        m_sunLight->specular = glm::vec4(0.0f);

        break;
    case cEnvironment::DAWN:
        m_timeOfDay = 0.25f;
        m_midNight = 0.0f;
        m_dawn = 1.0f;
        m_noon = 0.0f;
        m_sunset = 0.0f;
        m_night = 0.0f;

        // Lights control
        m_sunLight->position = getSunPosition();
        m_sunLight->typeParams2.x = 1.0f; // Power
        m_sunLight->specular = glm::vec4(1.0f);
        m_sunLight->diffuse = m_colourDawn;

        break;
    case cEnvironment::NOON:
        m_timeOfDay = 0.5f;
        m_midNight = 0.0f;
        m_dawn = 0.0f;
        m_noon = 1.0f;
        m_sunset = 0.0f;
        m_night = 0.0f;

        // Lights control
        m_sunLight->position = getSunPosition();
        m_sunLight->typeParams2.x = 1.0f; // Power
        m_sunLight->specular = glm::vec4(1.0f);
        m_sunLight->diffuse = m_colourNoon;

        break;
    case cEnvironment::SUNSET:
        m_timeOfDay = 0.75f;
        m_midNight = 0.0f;
        m_dawn = 0.0f;
        m_noon = 0.0f;
        m_sunset = 1.0f;
        m_night = 0.0f;

        // Lights control
        m_sunLight->position = getSunPosition();
        m_sunLight->typeParams2.x = 1.0f; // Power
        m_sunLight->specular = glm::vec4(1.0f);
        m_sunLight->diffuse = m_colourSunset;

        break;
    case cEnvironment::NIGHT:
        m_timeOfDay = 0.875f;
        m_midNight = 0.0f;
        m_dawn = 0.0f;
        m_noon = 0.0f;
        m_sunset = 0.0f;
        m_night = 1.0f;

        // Lights control
        m_sunLight->position = getSunPosition();
        m_sunLight->typeParams2.x = 0.0f; // Power
        m_sunLight->specular = glm::vec4(0.0f);

        break;
    default:
        break;
    }

    // To ensure we continue from where we set the time
    m_elapsedTime = m_timeOfDay * m_dayDuration;

    // Update light positions
    m_sunLight->position = getSunPosition();
}

void cEnvironment::update(float deltaTime)
{
    if (m_mode == CONTINUOUS)
    {
        m_elapsedTime += deltaTime;
        m_timeOfDay = fmod(m_elapsedTime, m_dayDuration) / m_dayDuration;

        // Update light positions
        m_sunLight->position = getSunPosition();

        float hour = m_timeOfDay * 24.0f;
        if (hour >= 0.0f && hour < 6.0f)
        {
            m_midNight = (6.0f - hour) / 6.0f;
            m_dawn = hour / 6.0f;
            m_noon = 0.0f;
            m_sunset = 0.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (6.0f - hour) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourDawn + factor * m_colourMidnight;
            m_moonLight->diffuse = colourLight;
        }
        else if (hour >= 6.0f && hour < 12.0f)
        {
            m_midNight = 0.0f;
            m_dawn = (12.0f - hour) / 6.0f;
            m_noon = (hour - 6.0f) / 6.0f;
            m_sunset = 0.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 1.0f; // Power
            m_sunLight->specular = glm::vec4(1.0f);
            float factor = (hour - 6.0f) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourDawn + factor * m_colourNoon;
            m_sunLight->diffuse = colourLight;

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 0.0f; // Power
            m_moonLight->specular = glm::vec4(0.0f);
        }
        else if (hour >= 12.0f && hour < 18.0f)
        {
            m_midNight = 0.0f;
            m_dawn = 0.0f;
            m_noon = (18.0f - hour) / 6.0f;
            m_sunset = (hour - 12.0f) / 6.0f;
            m_night = 0.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 1.0f; // Power
            m_sunLight->specular = glm::vec4(1.0f);
            float factor = (hour - 12.0f) / 6.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourNoon + factor * m_colourSunset;
            m_sunLight->diffuse = colourLight;

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 0.0f; // Power
            m_moonLight->specular = glm::vec4(0.0f);
        }
        else if (hour >= 18.0f && hour < 21.0f)
        {
            m_midNight = 0.0f;
            m_dawn = 0.0f;
            m_noon = 0.0f;
            m_sunset = (21.0f - hour) / 3.0f;
            m_night = (hour - 18.0f) / 3.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (hour - 18.0f) / 3.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourSunset + factor * m_colourNight;
            m_moonLight->diffuse = colourLight;
        }
        else
        {
            m_midNight = (hour - 21.0f) / 3.0f;
            m_dawn = 0.0f;
            m_noon = 0.0f;
            m_sunset = 0.0f;
            m_night = (24.0f - hour) / 3.0f;

            // Lights control
            m_sunLight->position = getSunPosition();
            m_sunLight->typeParams2.x = 0.0f; // Power
            m_sunLight->specular = glm::vec4(0.0f);

            m_moonLight->position = getMoonPosition();
            m_moonLight->typeParams2.x = 1.0f; // Power
            m_moonLight->specular = glm::vec4(1.0f);
            float factor = (hour - 21.0f) / 3.0f;
            glm::vec3 colourLight = (1.0f - factor) * m_colourNight + factor * m_colourMidnight;
            m_moonLight->diffuse = colourLight;
        }
    }
}
