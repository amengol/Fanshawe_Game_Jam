#ifndef _Physics_HG_
#define _Physics_HG_

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>

struct RK4_State
{
    glm::vec3 x;      // position
    glm::vec3 v;      // velocity
};

struct RK4_Derivative
{
    RK4_Derivative() :
        dx(0.0f), dv(0.0f)
    { }
    glm::vec3 dx;      // dx/dt = velocity
    glm::vec3 dv;      // dv/dt = acceleration
};

RK4_Derivative evaluate(const RK4_State& initial,
                        float dt,
                        const RK4_Derivative& d);

void integrate(glm::vec3& pos, glm::vec3& vel, glm::vec3 accel, float dt);

enum LimitPlaneType
{
    FLOOR,
    FRONT,
    BACK,
    LEFT,
    RIGHT
};

struct LimitPlane
{
    glm::vec3 position;
    LimitPlaneType type;
};

class cGameObject;

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime);

bool TestSphereTriangle(cGameObject* s, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 &p);

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

#endif // !_Physics_HG_

