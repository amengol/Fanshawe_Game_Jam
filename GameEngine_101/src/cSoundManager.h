#ifndef _cSoundManager_HG_
#define _cSoundManager_HG_

#include <string>
#include <glm\vec3.hpp>

// Forward declare to the actual implementation class
class cSoundManager_Imp;
class cSoundObject;

class cSoundManager
{
public:
    cSoundManager();
    ~cSoundManager();

    void loadConfigFile(std::string file);
    void loadSoundObjectsIntoScene();
    void initSoundScene();
    void updateSoundScene(glm::vec3 listener);
    bool loadSoundParameters(std::string configFile);
    cSoundObject* getSoundFromName(std::string name);

private:
    // Pointer to the actual implementation class
    cSoundManager_Imp* pImp;
};

#endif // !_cSoundManager_HG_

