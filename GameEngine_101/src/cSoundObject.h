#ifndef _cSoudObject_HG_
#define _cSoudObject_HG_

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>

class cSoundObject
{
public:
    cSoundObject();
    ~cSoundObject();

    void setPosition(glm::vec3 pos);
    void setfriendlyName(std::string name);
    void setSource(std::string src);
    void setFMODType(int type);
    void setVolume(float vol);
    void setMute(bool mute);
    void setMovType(int type);
    void setLoop(bool loop = true) { m_isLooping = loop; }
    void play();
    void pause();
    void stop();
    bool isPaused() { return m_isPaused; }
    bool isLooping() { return m_isLooping; }
    bool isStopped() { return m_isStopped; }
    glm::vec3 getPosition();
    std::string getFriendlyName();
    std::string getSource();
    int getFMODType();
    float getVolume();
    bool getMute();
    int getMovType();

private:
    int FMOD_Type;
    char movType;               // 1 static, 2 mobile
    glm::vec3 position;
    std::string friendlyName;
    std::string source;
    float volume;
    bool isMute;
    bool m_isPaused;
    bool m_isLooping;
    bool m_isStopped;
};

#endif // !_cSoudObject_HG_

