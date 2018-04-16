/**
    Controls Day/Night cycle and other environment parameters

    @author Jorge Amengol
    @version 1.1
    @date April 16th, 2018
*/
#pragma once
#include <glm\vec3.hpp>

class cLight;
class cLightManager;
class cGameObject;
class cTransparencyManager;

class cEnvironment
{
public:
    cEnvironment();
    ~cEnvironment();

    // Sky blend values
    float m_midNight;
    float m_dawn;
    float m_noon;
    float m_sunset;
    float m_night;

    // Fog
    bool m_fogActive;
    glm::vec3 m_fogColour;
    float m_fogPercent;

    enum Mode
    {
        CONTINUOUS,
        STEP
    };

    enum DaySkyLight
    {
        MIDNIGHT = 0,
        DAWN = 6,
        NOON = 12,
        SUNSET = 18,
        NIGHT = 21
    };

    // Init Lights
    void initLights(int shaderID, cLightManager*, cTransparencyManager*);

    // Sets the mode
    inline void setMode(Mode mode) { m_mode = mode; }

    // Sets the day duration in minutes
    void setDayDuration(float minutes);

    // Advances to the next time of the day
    void advance(float seconds);

    // Start the clock
    void startClock();

    // Stop the clock
    void stopClock();

    // Sets a specifc time of the day in hours
    void setTimeOfDay(float hour);

    // Returns the sun position
    glm::vec3 getSunPosition();

    // Returns the moon position
    glm::vec3 getMoonPosition();

    // Sets a full time of the day
    void setTimeOfDay(DaySkyLight daySkyLight);

    // Updates Environment parameters
    void update(float deltaTime);

    // Returns the time of the day in hours
    inline float getTimeOfDay() { return m_timeOfDay * 24.0f; }

private:
    Mode m_mode;                // The day progression mode
    DaySkyLight m_daySkyLight;  // Sky State
    float m_timeOfDay;          // Time of the day
    float m_elapsedTime;        // Elapsed time in seconds
    float m_dayDuration;        // Day duration in seconds

    cLight* m_sunLight;
    cLight* m_moonLight;
    
    // Light colours
    glm::vec3 m_colourMidnight;
    glm::vec3 m_colourDawn;
    glm::vec3 m_colourNoon;
    glm::vec3 m_colourSunset;
    glm::vec3 m_colourNight;

    // Sun and Moon objects
    cGameObject* m_sunGO;
    cGameObject* m_moonGO;
};

