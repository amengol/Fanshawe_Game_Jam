/**
    Controls Day/Night cycle and other environment parameters

    @author Jorge Amengol
    @version 0.4
    @date April 8th, 2018
*/
#pragma once

class cLight;
class cLightManager;

class cEnvironment
{
public:
    cEnvironment();
    ~cEnvironment();

    cLight* m_midNight;
    cLight* m_dawn;
    cLight* m_noon;
    cLight* m_sunset;
    cLight* m_night;

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
    void initLights(int shaderID, cLightManager* lightManager);

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

    // Sets a full time of the day
    void setTimeOfDay(DaySkyLight daySkyLight);

    // Updates Environment parameters
    void update(float deltaTime);

private:
    Mode m_mode;                // The day progression mode
    DaySkyLight m_daySkyLight;  // Sky State
    float m_timeOfDay;          // Time of the day
    float m_elapsedTime;        // Elapsed time in seconds
    float m_dayDuration;        // Day duration in seconds
};

