#pragma once
#include <vector>


class cGameObject;
struct GLFWwindow;
class cCamera;

// Draws a cGameObject*
void DrawObject(cGameObject* pTheGO);

// The main render call
void RenderScene(std::vector<cGameObject*> &vec_pGOs, 
                 GLFWwindow* pGLFWWindow, 
                 cCamera& camera,                 
                 double deltaTime);

// Render only one GameObject
void RenderScene(cGameObject* pTheGO,
                 GLFWwindow* pGLFWWindow,
                 cCamera& camera,
                 double deltaTime);

// The shadow/depth render call
void RenderScene(std::vector<cGameObject*> &vec_pGOs, unsigned int shaderID);