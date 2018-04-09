#include "GLFW_callbacks.h"
#include "globalGameStuff.h"
#include "Utilities.h"

float lastX = g_scrWidth * 0.5f;
float lastY = g_scrHeight * 0.5f;
bool firstMouse = true;
bool cursorOn = true;
bool isCharacterMoving = false;

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (cursorOn)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    else
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorOn = true;
        }

        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            g_camera.lockOnGameObject(g_characterManager.GetActiveCharacter()->GetCharacter());
        }

        if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        {
            g_camera.releaseGameObject();
        }
    }    
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    g_scrWidth = width;
    g_scrHeight = height;
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cursorOn)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        g_camera.processMouseMovement(xoffset, yoffset);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (cursorOn)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorOn = false;
            firstMouse = true;
        }
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_camera.processMouseScroll(yoffset);
}

void processCameraInput(GLFWwindow* window, float deltaTime)
{
    switch (g_camera.getCameraMode())
    {
    case FREE:
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            g_camera.processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            g_camera.processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            g_camera.processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            g_camera.processKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            g_camera.processKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            g_camera.processKeyboard(DOWN, deltaTime);
        break;
    case THIRD_PERSON:
    {
        // Get the character
        cCharacterControl* pCharacterControl = NULL;
        pCharacterControl = g_characterManager.GetActiveCharacter();
        if (pCharacterControl == NULL)
            return;

        // Controller test
        int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

        if (present == 1)
        {
            int count;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

            if (axes[0] != 0.0f || axes[1] != 0.0f)
            {
                isCharacterMoving = true;

                // Mount a normalized vector direction from the controller
                glm::vec3 controllerDir = glm::vec3(-axes[0], 0.0f, axes[1]);
                glm::vec3 normilizedControllerDir = glm::normalize(controllerDir);

                // Get the new character orientaion
                glm::mat4 chacacterRotation = getMatrixFromVector(normilizedControllerDir);
                glm::mat4 cameraOrientation = getMatrixFromVector(g_camera.getCameraFrontVector());
                cameraOrientation *= chacacterRotation;
                pCharacterControl->GetCharacter()->rigidBody->SetMatOrientation(cameraOrientation);
                
                if (glm::length(controllerDir) <= 0.9f)
                {
                    pCharacterControl->GetCharacter()->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 1.5f));
                    pCharacterControl->Forward();
                }
                else
                {
                    pCharacterControl->GetCharacter()->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 4.75f));
                    pCharacterControl->ForwardRun();
                }
            }// !if (axes[0] != 0.0f || axes[1] != 0.0f)
            else
            {
                isCharacterMoving = false;

                pCharacterControl->GetCharacter()->rigidBody->SetVelocityLocal(glm::vec3(0.0f, 0.0f, 0.0f));
                pCharacterControl->Idle();
            }// !else if (axes[0] != 0.0f || axes[1] != 0.0f)

            if (axes[2] != 0.0f || axes[3] != 0.0f)
            {
                g_camera.processJoystickMovement(axes[2] * deltaTime, axes[3] * deltaTime);
            }// !if (axes[0] != 0.0f || axes[1] != 0.0f)
        }
    }
        break;
    default:
        break;
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        g_camera.m_movementSpeedFactor = 4.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        g_camera.m_movementSpeedFactor = 1.0f;

}
