#pragma once
#include <vector>


class cGameObject;
struct GLFWwindow;
class cCamera;

// Draws a cGameObject*
void DrawObject(cGameObject* pTheGO);

// The main render call
void RenderScene(std::vector<cGameObject*> &vec_pGOs, GLFWwindow* pGLFWWindow, cCamera& camera, double deltaTime);