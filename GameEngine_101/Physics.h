#ifndef _Physics_HG_
#define _Physics_HG_

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>

class cGameObject;

// Update the world 1 "step" in time
void PhysicsStep(double deltaTime);

bool TestSphereTriangle(cGameObject* s, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 &p);

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

#endif // !_Physics_HG_

