/**
    Controls the screens and other params in the scene
    
    @author Jorge Amengol
    @version 1.1
    @date April 23th, 2018
*/
#pragma once
#include <list>

class cSoundObject;
class cSceneManager
{
public:
    cSceneManager()
        : m_elapsedTime(0.0f)
    { }
    ~cSceneManager();

    enum ScreenType
    {
        NORMAL,
        START,
        LAST
    };

    // Sets the active screen
    void setActiveScreen(std::string name);

    // Returns the active screen
    int getActiveScreenType() { return m_activeScreen.type; }

    // Returns the active screen ID
    int getActiveScreenID() { return m_activeScreen.ID; }

    // Returns the fade value
    float getFade() { return m_activeScreen.fade; }

    // Returns wether the screen is on or not
    bool inNoiseOn() { return m_activeScreen.m_isNoiseOn; }

    // Sets the current scene mute state
    void setMute(bool mute = true);

    // Gets the current scene mute state
    bool getMute();

    // Adds a screen
    void addScreen(std::string name,
                   int ID,
                   float length,
                   bool noiseOn,
                   bool fadeToNext,
                   cSoundObject* sound = NULL,
                   ScreenType type = NORMAL);

    // Initializes the scene. 
    // Should run only after all screens added
    void init();

    // Pass to next screen
    void nextScreen();

    // Change the screen according to the start buttom
    void joystickStart();

    // Resumes the screenplay
    void resumeScreenPlay();

    // Pauses the screenplay
    void pauseSceenPlay();

    // Resets the sceenplay
    void resetSceenPlay();

    // Update the sreenplay
    void update(float deltaTime);

private:
    
    
    struct sScreenInfo
    {
        sScreenInfo()
            : ID(0)
            , length(0.0f)
            , fadeToNext(false)
            , fade(0.0f)
            , type(NORMAL)
            , m_isNoiseOn(false)
        { }

        std::string name;
        int ID;
        float length;
        cSoundObject* sound;
        bool fadeToNext;
        float fade;
        ScreenType type;
        bool m_isNoiseOn;
    };

    sScreenInfo m_activeScreen;
    sScreenInfo m_nextScreen;
    std::list<sScreenInfo> m_listScreenInfo;
    std::list<sScreenInfo>::iterator m_itScreen;
    float m_elapsedTime;
};

