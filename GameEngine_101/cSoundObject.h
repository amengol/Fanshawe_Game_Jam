#ifndef _cSoudObject_HG_
#define _cSoudObject_HG_

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>

class cSoudObject
{
public:
    cSoudObject();
    ~cSoudObject();

    void setPosition(glm::vec3 pos);
    void setfriendlyName(std::string name);
    void setSource(std::string src);
    void setFMODType(int type);
    void setVolume(float vol);
    void setMute(bool mute);
    void setMovType(int type);
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
};

#endif // !_cSoudObject_HG_

