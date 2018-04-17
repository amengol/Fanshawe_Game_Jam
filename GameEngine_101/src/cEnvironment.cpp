#include "cEnvironment.h"
#include "cLightManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\rotate_vector.hpp>
#include "cGameObject.h"
#include "cTransparencyManager.h"

cEnvironment::cEnvironment()
{
    m_fogActive = true;
    m_fogPercent = 1.0f;
    m_mode = STEP;
    m_dayDuration = 15.0f;
    m_elapsedTime = 0.0f;
    m_timeOfDay = 0.0f;

    m_colourMidnight = glm::vec3(0.3f, 0.3f, 0.3f);
    m_colourDawn = glm::vec3(1.0f, 1.0f, 0.79f);
    m_colourNoon = glm::vec3(1.6f, 1.6f, 1.44f);
    m_colourSunset = glm::vec3(1.0f, 0.925f, 0.925f);
    m_colourNight = glm::vec3(0.4f, 0.4f, 0.4f);

    // Create all the lights
    m_sunLight = new cLight();
    m_sunLight->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_sunLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    m_sunLight->specular = glm::vec4(0.0f);
    m_sunLight->attenuation = glm::vec3(0.0f);
    m_sunLight->attenuation.x = 1.0f;
    m_sunLight->typeParams = glm::vec4(0.0f);
    m_sunLight->typeParams.x = 3.0f; // Sun/moon light
    m_sunLight->typeParams.y = 1000.0f;
    m_sunLight->typeParams2 = glm::vec4(0.0f);

    m_moonLight = new cLight();
    m_moonLight->position = glm::vec3(0.0f, 400.0f, 0.0f);
    m_moonLight->specular = glm::vec4(0.0f);
    m_moonLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    m_moonLight->attenuation = glm::vec3(0.0f);
    m_moonLight->attenuation.x = 1.0f;
    m_moonLight->typeParams = glm::vec4(0.0f);
    m_moonLight->typeParams.x = 3.0f; // Sun/moon light
    m_moonLight->typeParams.y = 1000.0f;
    m_moonLight->typeParams2 = glm::vec4(0.0f);
    
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::initLights(int shaderID, 
                              cLightManager* lightManager, 
                              cTransparencyManager* transpManager)
{
    std::vector<cLight*> lights;
    lights.push_back(m_sunLight);
    lights.push_back(m_moonLight);

    lightManager->CreateLights(lights);
    lightManager->LoadShaderUniformLocations(shaderID);

    // Sun
    m_sunGO = new cGameObject();
    m_sunGO->meshName = "Plane1x1";
    m_sunGO->textureBlend[0] = 1.0f;
    m_sunGO->textureNames[0] = "Sun.bmp";
    m_sunGO->hasAlpha = true;
    m_sunGO->useDiscardAlpha = false;
    m_sunGO->textureNames[1] = "Sun_Alpha.bmp";
    m_sunGO->rotateToCamera = true;
    m_sunGO->selfLight = true;
    m_sunGO->scale = 25.0f;
    transpManager->transpObjects.push_back(m_sunGO);

    // Moon
    m_moonGO = new cGameObject();
    m_moonGO->meshName = "Plane1x1";
    m_moonGO->textureBlend[0] = 1.0f;
    m_moonGO->textureNames[0] = "Moon.bmp";
    m_moonGO->hasAlpha = true;
    m_moonGO->useDiscardAlpha = false;
    m_moonGO->textureNames[1] = "Moon_Alpha.bmp";
    m_moonGO->rotateToCamera = true;
    m_moonGO->selfLight = true;
    m_moonGO->scale = 20.0f;
    transpManager->transpObjects.push_back(m_moonGO);
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
            
            m_fogColour = colourLight * 0.25f;
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

            m_fogColour = colourLight * 0.25f;
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

            m_fogColour = colourLight * 0.25f;
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

            m_fogColour = colourLight * 0.25f;
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

            m_fogColour = colourLight * 0.25f;
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

        m_moonLight->position = getMoonPosition();
        m_moonLight->typeParams2.x = 1.0f; // Power
        m_moonLight->specular = glm::vec4(1.0f);
        m_moonLight->diffuse = m_colourMidnight;

        m_fogColour = m_colourMidnight * 0.25f;
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

        m_moonLight->position = getMoonPosition();
        m_moonLight->typeParams2.x = 0.0f; // Power
        m_moonLight->specular = glm::vec4(0.0f);

        m_fogColour = m_colourDawn * 0.25f;
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

        m_moonLight->position = getMoonPosition();
        m_moonLight->typeParams2.x = 0.0f; // Power
        m_moonLight->specular = glm::vec4(0.0f);

        m_fogColour = m_colourNoon * 0.25f;
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

        m_moonLight->position = getMoonPosition();
        m_moonLight->typeParams2.x = 0.0f; // Power
        m_moonLight->specular = glm::vec4(0.0f);

        m_fogColour = m_colourSunset * 0.25f;
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

        m_moonLight->position = getMoonPosition();
        m_moonLight->typeParams2.x = 1.0f; // Power
        m_moonLight->specular = glm::vec4(1.0f);
        m_moonLight->diffuse = m_colourNight;

        m_fogColour = m_colourNight * 0.25f;
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
    // Update sun and moon position
    if (m_sunGO != NULL && m_moonGO != NULL)
    {
        m_sunGO->position = getSunPosition();
        m_sunGO->position.y -= 20.0f;

        m_moonGO->position = getMoonPosition();
        m_moonGO->position.y -= 20.0f;

        // Avoid the gilbal lock effect on the moon
        if (getTimeOfDay() >= 5.0f && getTimeOfDay() <= 17.0f)
        {
            m_moonGO->rotateToCamera = false;
        }
        else
        {
            m_moonGO->rotateToCamera = true;
        }
    }


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

            m_fogColour = colourLight * 0.25f;
            m_fogPercent = 1.0f;
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

            m_fogColour = colourLight * 0.25f;
            m_fogPercent = (1.0f - (hour - 6.0f) / 6.0f);
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

            m_fogColour = colourLight * 0.25f;
            m_fogPercent = (hour - 12.0f) / 6.0f;
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

            m_fogColour = colourLight * 0.25f;
            m_fogPercent = 1.0f;
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

            m_fogColour = colourLight * 0.25f;
            m_fogPercent = 1.0f;
        }
    }
}
