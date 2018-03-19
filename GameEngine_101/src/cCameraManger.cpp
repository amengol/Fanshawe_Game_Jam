#include "cCameraManger.h"
#include "cCameraObject.h"


cCameraManger::cCameraManger()
{
}


cCameraManger::~cCameraManger()
{
}

void cCameraManger::CircleAroundObject(cCameraObject* camera,
                                       cGameObject* GO, 
                                       glm::vec3 speed, 
                                       glm::vec3 height,
                                       float distaceRadius, 
                                       bool counterClockWise)
{
    if (GO == NULL)
        return;

    sCircleAround ca;
    ca.camera = camera;
    ca.GO = GO;
    ca.speed = speed;
    ca.height = height;
    ca.distaceRadius = distaceRadius;
    ca.counterClockWise = counterClockWise;

    mVecCircleAround.push_back(ca);
}

void cCameraManger::AddCamera(cCameraObject* Camera)
{
    mVecCameras.push_back(Camera);
}


void cCameraManger::Update(float deltaTIme)
{
    for (size_t i = 0; i < mVecCircleAround.size(); i++)
    {
        sCircleAround ca = mVecCircleAround[i];

        //RK4
        glm::vec3 camPosition = ca.camera->getCameraPosition();
        ca.camera->camLastPos = camPosition;    // Save the last position
        integrate(camPosition, ca.speed, glm::vec3(0.0f), deltaTIme);
        ca.camera->setCameraPosition(camPosition);
    }
}

cCameraManger::RK4_Derivative cCameraManger::evaluate(const RK4_State& initial, float dt, const RK4_Derivative& d)
{
    RK4_State state;
    state.x = initial.x + d.dx*dt;
    state.v = initial.v + d.dv*dt;

    RK4_Derivative output;
    output.dx = state.v;
    //output.dv = acceleration(state, t + dt);
    return output;
}

void cCameraManger::integrate(glm::vec3 & pos, glm::vec3 & vel, glm::vec3 accel, float dt)
{
    {
        // Put the acceleration into the velocity
        glm::vec3 newVel = vel + accel * dt;

        RK4_State state;
        state.x = pos;
        state.v = newVel;

        RK4_Derivative a, b, c, d;

        a = evaluate(state, 0.0f, RK4_Derivative());
        b = evaluate(state, dt*0.5f, a);
        c = evaluate(state, dt*0.5f, b);
        d = evaluate(state, dt, c);

        glm::vec3 dxdt = 1.0f / 6.0f *
            (a.dx + 2.0f * (b.dx + c.dx) + d.dx);

        glm::vec3 dvdt = 1.0f / 6.0f *
            (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

        pos = state.x + dxdt * dt;
        vel = state.v + dvdt * dt;
    }
}
