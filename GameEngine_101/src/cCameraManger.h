#pragma once
#include <vector>
#include <glm\vec3.hpp>

class cCameraObject;
class cGameObject;

class cCameraManger
{
public:
    cCameraManger();
    ~cCameraManger();

    // Makes a camera circles around a Game Object
    void CircleAroundObject(cCameraObject*,
                            cGameObject*,
                            float speed,
                            bool counterClockWise);

    // Sets the active Camera
    void SetActiveCamera(cCameraObject*);

    // Gets the active Camera
    inline cCameraObject* GetAcitveCamera() { return mActiveCamera; }

    // Adds a camera to the Manager
    void AddCamera(cCameraObject*);

    // Updates all managed cameras
    void Update(float deltaTIme);

private:
    cCameraObject* mActiveCamera;
    std::vector<cCameraObject*> mVecCameras;    // Vector of managed cameras

    struct sCircleAround
    {
        cCameraObject* camera;
        cGameObject* GO;
        float speed;
        bool counterClockWise;
    };

    std::vector<sCircleAround> mVecCircleAround;    // A vector of circle around struct

    struct RK4_State
    {
        glm::vec3 x;      // position
        glm::vec3 v;      // velocity
    };

    struct RK4_Derivative
    {
        RK4_Derivative() :
            dx(0.0f), dv(0.0f)
        {
        }
        glm::vec3 dx;      // dx/dt = velocity
        glm::vec3 dv;      // dv/dt = acceleration
    };

    RK4_Derivative evaluate(const RK4_State& initial,
                            float dt,
                            const RK4_Derivative& d);

    void integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt);
};

