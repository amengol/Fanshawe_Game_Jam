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
    glm::vec3 getPosition();
    std::string getFriendlyName();
    std::string getSource();
    int getFMODType();

private:
    int FMOD_Type;
    glm::vec3 position;
    std::string friendlyName;
    std::string source;
};

#endif // !_cSoudObject_HG_

