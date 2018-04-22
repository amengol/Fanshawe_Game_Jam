/**
    Controls the screens and other params in the scene
    
    @author Jorge Amengol
    @version 1.0
    @date April 22th, 2018
*/
#pragma once
#include <list>
class cSceneManager
{
public:
    cSceneManager()
        : m_elapsedTime(0.0f)
        , m_activeScreen(-1)
    { }
    ~cSceneManager();

    // Sets the active screen
    void setActiveScreen(std::string name);

    // Returns the active screen ID
    int getActiveScreen() { return m_activeScreen; }

    // Adds a screen
    void addScreen(std::string name, int ID, float length);

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
        { }

        std::string name;
        int ID;
        float length;
    };

    int m_activeScreen;
    std::list<sScreenInfo> m_listScreenInfo;
    std::list<sScreenInfo>::iterator m_itScreen;
    float m_elapsedTime;
};

