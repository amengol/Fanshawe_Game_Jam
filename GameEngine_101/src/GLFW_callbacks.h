/**
    Contains all GLFW callbacks and iput process
    
    @author Jorge Amengol
    @version 1.1
    @date April 6th, 2018
*/
#pragma once
#include "globalOpenGL_GLFW.h"

// Error Callback
void errorCallback(int error, const char* description);

// Keyboard Callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Framebuffer Callback
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// Mouse Position Callback
void mousePosCallback(GLFWwindow* window, double xpos, double ypos);

// Mouse Button Callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Scroll Callback
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// Process Camera Input
void processCameraInput(GLFWwindow* window, float deltaTime);