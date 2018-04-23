#include "cSceneManager.h"
#include "cSoundObject.h"

cSceneManager::~cSceneManager()
{
}

void cSceneManager::setActiveScreen(std::string name)
{
    m_itScreen = m_listScreenInfo.begin();
    for (; m_itScreen != m_listScreenInfo.end(); m_itScreen++)
    {
        if (m_itScreen->name == name)
        {
            m_activeScreen = *m_itScreen;
            m_itScreen++;
            if (m_itScreen == m_listScreenInfo.end())
            {
                m_itScreen = m_listScreenInfo.begin();
            }
            m_nextScreen = *m_itScreen;
            break;
        }
    }
}

void cSceneManager::setMute(bool mute)
{
    m_activeScreen.sound->setMute(mute);
}

bool cSceneManager::getMute()
{
    return m_activeScreen.sound->getMute();
}

void cSceneManager::addScreen(std::string name,
                              int ID, 
                              float length, 
                              bool noiseOn, 
                              bool fadeToNext, 
                              cSoundObject* sound,
                              ScreenType type)
{
    sScreenInfo screen;
    screen.name = name;
    screen.ID = ID;
    screen.length = length;
    screen.m_isNoiseOn = noiseOn;
    screen.sound = sound;
    screen.fadeToNext = fadeToNext;
    screen.type = type;
    if (type == LAST)
    {
        screen.fade = 1.0f;
    }

    m_listScreenInfo.push_back(screen);
}

void cSceneManager::init()
{
    m_itScreen = m_listScreenInfo.begin();
    m_activeScreen = *m_itScreen;
    m_itScreen++;
    if (m_itScreen == m_listScreenInfo.end())
    {
        m_itScreen = m_listScreenInfo.begin();
    }
    m_nextScreen = *m_itScreen;
}

void cSceneManager::nextScreen()
{
    if (m_activeScreen.sound != NULL)
    {
        m_activeScreen.sound->stop();
    }
    m_activeScreen = m_nextScreen;
    m_itScreen++;
    if (m_itScreen == m_listScreenInfo.end())
    {
        m_itScreen = m_listScreenInfo.begin();
        m_nextScreen = *m_itScreen;
        m_elapsedTime = 0.0f;
    }
    else
    {
        m_nextScreen = *m_itScreen;
        m_elapsedTime = 0.0f;
    }
}

void cSceneManager::joystickStart()
{
    if (m_activeScreen.type == START)
    {
        this->nextScreen();
    }
}

void cSceneManager::update(float deltaTime)
{
    if (m_activeScreen.length != 0.0f)
    {
        if (m_activeScreen.sound != NULL)
        {
            m_activeScreen.sound->play();
        }

        m_elapsedTime += deltaTime;
        if (m_activeScreen.fadeToNext)
        {
            m_activeScreen.fade = m_elapsedTime / m_activeScreen.length;
        }

        if (m_elapsedTime > m_activeScreen.length)
        {
            if (m_activeScreen.sound != NULL)
            {
                m_activeScreen.sound->stop();
            }
            
            m_activeScreen = m_nextScreen;
            
            if (m_activeScreen.sound != NULL)
            {
                m_activeScreen.sound->play();
            }

            m_itScreen++;

            if (m_itScreen == m_listScreenInfo.end())
            {
                m_itScreen = m_listScreenInfo.begin();
            }

            m_nextScreen = *m_itScreen;
            m_elapsedTime = 0.0f;
        }
    }
    else
    {
        if (m_activeScreen.sound != NULL)
        {
            m_activeScreen.sound->play();
        }
    }
}
