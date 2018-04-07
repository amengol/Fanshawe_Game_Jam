#pragma once
class cEnvironment
{
public:
    cEnvironment();
    ~cEnvironment();

    enum Mode
    {
        CONTINUOUS,
        STEP
    };

    enum TimeOfDay
    {
        MORNING,
        NOON,
        AFTERNOON,
        NIGHT,
        MIDNIGHT
    };

    // Sets the mode
    void setMode(Mode mode);

    // Advances to the next time of the day
    void advance(float seconds);

    // Sets the day duration in minutes
    void setDayDuration(float duration);

    // Start the clock
    void startClock();

    // Stop the clock
    void stopClock();

    // Sets a specifc time of the day in hours
    void setTimeOfDay(float hours);

    // Sets a full time of the day
    void setTimeOfDay(TimeOfDay tod);
};

