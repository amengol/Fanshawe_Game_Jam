//#include "cCameraManger.h"
//#include "cCameraObject.h"
//#include "cGameObject.h"
//
//
//cCameraManger::cCameraManger()
//{
//    mActiveCamera = NULL;
//    mElapsedTime = 0.0f;
//    mTimeToCircle = 0.0f;
//}
//
//
//cCameraManger::~cCameraManger()
//{
//}
//
//void cCameraManger::CircleAroundObject(cCameraObject* camera,
//                                       cGameObject* GO, 
//                                       float speed, 
//                                       bool counterClockWise)
//{
//    if (GO == NULL)
//        return;
//
//    sCircleAround ca;
//    ca.camera = camera;
//    ca.GO = GO;
//    ca.speed = speed;
//    ca.counterClockWise = counterClockWise;
//
//    mVecCircleAround.push_back(ca);
//}
//
//void cCameraManger::SetActiveCamera(cCameraObject* camera)
//{
//    if (camera != NULL)
//    {
//        mActiveCamera = camera;
//    }
//}
//
//void cCameraManger::AddCamera(cCameraObject* Camera)
//{
//    mVecCameras.push_back(Camera);
//}
//
//
//void cCameraManger::Update(float deltaTIme)
//{
//    if (mCirclingCameras)
//    {
//        mElapsedTime += deltaTIme;
//
//        if (mElapsedTime >= mTimeToCircle)
//        {
//            CircleToNextCamera();
//            mElapsedTime = 0.0f;
//        }
//    }
//
//    for (size_t i = 0; i < mVecCircleAround.size(); i++)
//    {
//        sCircleAround ca = mVecCircleAround[i];
//
//        // The direction to the GameObject
//        glm::vec3 camPostiion = ca.camera->getCameraPosition();
//        glm::vec3 objectPostiion = ca.GO->position;
//        glm::vec3 GO_Direction = ca.GO->position - ca.camera->getCameraPosition();
//
//        // Get rid of Y information
//        GO_Direction.y = 0.0f;
//
//        // Normilize it
//        GO_Direction = glm::normalize(GO_Direction);
//
//        // The desired direction will be:
//        glm::vec3 desiredDir = glm::cross(GO_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
//
//        // Get the velocity
//        glm::vec3 newVelocity = glm::normalize(desiredDir) * ca.speed;
//
//        //RK4
//        glm::vec3 camPosition = ca.camera->getCameraPosition();
//        ca.camera->camLastPos = camPosition;    // Save the last position
//        integrate(camPosition, newVelocity, glm::vec3(0.0f), deltaTIme);
//        ca.camera->setCameraPosition(camPosition);
//
//        // hack to see the horizon
//        objectPostiion.y += 25.0f;
//        ca.camera->setCameraTarget(objectPostiion);
//    }
//}
//
//void cCameraManger::CircleToNextCamera()
//{
//    if (mVecCameras.size() != 0)
//    {
//        if (mActiveCamera == NULL)
//        {
//            mActiveCamera = mVecCameras[0];
//            return;
//        }
//
//        std::vector<cCameraObject*>::iterator it = mVecCameras.begin();
//        for (; it != mVecCameras.end(); it++)
//        {
//            if (*it == mActiveCamera)
//            {
//                it++;
//                if (it == mVecCameras.end())
//                {
//                    it = mVecCameras.begin();
//                    mActiveCamera = *it;
//                    return;
//                }
//                mActiveCamera = *it;
//                return;
//            }
//        }
//
//        mActiveCamera = mVecCameras[0];
//    }
//}
//
//void cCameraManger::AutoCircling(float timeToCircle)
//{
//    mCirclingCameras = true;
//    mTimeToCircle = timeToCircle;
//    mElapsedTime = 0.0f;
//}
//
//void cCameraManger::AutoCirclingOff()
//{
//    mCirclingCameras = false;
//}
//
//cCameraManger::RK4_Derivative cCameraManger::evaluate(const RK4_State& initial, float dt, const RK4_Derivative& d)
//{
//    RK4_State state;
//    state.x = initial.x + d.dx*dt;
//    state.v = initial.v + d.dv*dt;
//
//    RK4_Derivative output;
//    output.dx = state.v;
//    //output.dv = acceleration(state, t + dt);
//    return output;
//}
//
//void cCameraManger::integrate(glm::vec3 & pos, glm::vec3 & vel, glm::vec3 accel, float dt)
//{
//    {
//        // Put the acceleration into the velocity
//        glm::vec3 newVel = vel + accel * dt;
//
//        RK4_State state;
//        state.x = pos;
//        state.v = newVel;
//
//        RK4_Derivative a, b, c, d;
//
//        a = evaluate(state, 0.0f, RK4_Derivative());
//        b = evaluate(state, dt*0.5f, a);
//        c = evaluate(state, dt*0.5f, b);
//        d = evaluate(state, dt, c);
//
//        glm::vec3 dxdt = 1.0f / 6.0f *
//            (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
//
//        glm::vec3 dvdt = 1.0f / 6.0f *
//            (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
//
//        pos = state.x + dxdt * dt;
//        vel = state.v + dvdt * dt;
//    }
//}
