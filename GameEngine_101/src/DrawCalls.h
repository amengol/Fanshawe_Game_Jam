#pragma once
#include <vector>


class cGameObject;
struct GLFWwindow;
class cCameraObject;

// Draws cloth soft bodies
void ClothDraw(cGameObject* pTheGO);

// Draws a cGameObject*
void DrawObject(cGameObject* pTheGO);

// The main render call
void RenderScene(std::vector<cGameObject*> &vec_pGOs, GLFWwindow* pGLFWWindow, cCameraObject* pCamera, double deltaTime);

//// Draws a debug cube at an AABB location with a specific size
//void DrawAABB(cGameObject* pTheGO, float size);
//
//// Draws an AABB for specific points
//void DrawAABBforPoints(std::vector<glm::vec3> vertices, float AABBSize);