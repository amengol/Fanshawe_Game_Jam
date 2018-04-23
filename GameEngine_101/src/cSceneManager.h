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
        , m_isScreenOn(true)
    { }
    ~cSceneManager();

    // Sets the active screen
    void setActiveScreen(std::string name);

    // Returns the active screen ID
    int getActiveScreen() { return m_activeScreen.ID; }

    // Returns the fade value
    float getFade() { return m_activeScreen.fade; }

    // Returns wether the screen is on or not
    bool isScreenOn() { return m_isScreenOn; }

    // Adds a screen
    void addScreen(std::string name, 
                   int ID, 
                   float length, 
                   cSoundObject* sound = NULL, 
                   bool fadeToNext = false,
                   bool lastScreen = false);

    // Initializes the scene. 
    // Should run only after all screens added
    void init();

    // Pass to next screen
    void nextScreen();

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
            , isLastScreen(false)
        { }

        std::string name;
        int ID;
        float length;
        cSoundObject* sound;
        bool fadeToNext;
        float fade;
        bool isLastScreen;
    };

    sScreenInfo m_activeScreen;
    sScreenInfo m_nextScreen;
    std::list<sScreenInfo> m_listScreenInfo;
    std::list<sScreenInfo>::iterator m_itScreen;
    float m_elapsedTime;
    bool m_isScreenOn;
};

