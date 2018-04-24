#include "GLFW_callbacks.h"
#include "globalGameStuff.h"
#include "Utilities.h"

float lastX = g_scrWidth * 0.5f;
float lastY = g_scrHeight * 0.5f;
bool firstMouse = true;
bool cursorOn = true;
bool g_isCharacterMoving = false;
bool G_Pressed = false;
bool isJumping = false;
bool g_propsEnabled = true;
glm::mat4 characterOrientation(1.0f); // Used to avoid the tilt of the capsule

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
    }   

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        g_camera.lockOnGameObject(g_characterManager.GetActiveCharacter()->GetCharacter());
    }

    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        g_camera.releaseGameObject();
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        g_environment.m_fogActive = !g_environment.m_fogActive;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        g_debugEnable = !g_debugEnable;
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (g_environment.getMode() == cEnvironment::Mode::STEP)
        {
            g_environment.setMode(cEnvironment::Mode::CONTINUOUS);
        }
        else
        {
            g_environment.setMode(cEnvironment::Mode::STEP);
        }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        g_propsEnabled = !g_propsEnabled;
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        g_pSeceneManager->setMute(!g_pSeceneManager->getMute());
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        g_pSeceneManager->nextScreen();
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        G_Pressed = true;
    }

    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
    {
        G_Pressed = false;
    }

    if (G_Pressed)
    {
        if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
        {
            g_gamaCorrection -= 0.1f;
            printf("Gama Correction: %f\n", g_gamaCorrection);
        }

        if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
        {
            g_gamaCorrection += 0.1f;
            printf("Gama Correction: %f\n", g_gamaCorrection);
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
    // Get the character
    cCharacterControl* pCharacterControl = NULL;
    pCharacterControl = g_characterManager.GetActiveCharacter();
    

    // Get the current velocity to reference what to do
    glm::vec3 velocity;
    if (pCharacterControl != NULL)
    {
        pCharacterControl->GetCharacter()->rigidBody->GetLinearVelocity(velocity);
    }

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

        // Avoid the tilt of the capsule
        pCharacterControl->GetCharacter()->rigidBody->SetMatOrientation(characterOrientation);

        // Don't stop the falling
        pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));

        break;
    case THIRD_PERSON:
    {
        if (pCharacterControl == NULL)
            return;

        // Controller test
        int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

        if (present == 1)
        {
            int count;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

            if (axes[0] != 0.0f || axes[1] != 0.0f)
            {
                // Only move in the "Game Scene"
                if (g_pSeceneManager->getActiveScreenType() != cSceneManager::LAST)
                    return;

                g_isCharacterMoving = true;

                // Mount a normalized vector direction from the controller
                glm::vec3 controllerDir = glm::vec3(-axes[0], 0.0f, axes[1]);
                glm::vec3 normilizedControllerDir = glm::normalize(controllerDir);

                // Get the new character orientaion
                characterOrientation = getMatrixFromVector(normilizedControllerDir);
                glm::mat4 cameraOrientation = getMatrixFromVector(g_camera.getCameraFrontVector());
                characterOrientation *= cameraOrientation;
                pCharacterControl->GetCharacter()->rigidBody->SetMatOrientation(characterOrientation);
                
                if (glm::length(controllerDir) <= 0.9f)
                {
                    // Joystick
                    if (buttons[0] == GLFW_PRESS)
                    {
                        pCharacterControl->ForwardJumpWalking();
                        isJumping = true;
                    }
                    else if (buttons[2] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_01();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else if (buttons[3] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_02();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else if (buttons[1] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_03();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_01
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_02
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_03)
                        {
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 1.5f));
                            pCharacterControl->Forward();
                            isJumping = false;
                        }
                    }
                }
                else
                {
                    // Joystick
                    if (buttons[0] == GLFW_PRESS)
                    {
                        pCharacterControl->ForwardJump();
                        isJumping = true;
                    }
                    else if (buttons[2] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_01();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else if (buttons[3] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_02();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else if (buttons[1] == GLFW_PRESS)
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::JUMP_FORWARD_WALKING)
                        {
                            pCharacterControl->attack_03();
                            isJumping = false;
                            g_isCharacterMoving = false;
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));
                        }
                    }
                    else
                    {
                        if (pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_01
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_02
                            && pCharacterControl->GetAnimationState() != eCharacterAnim::ATTACK_03)
                        {
                            pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 4.75f));
                            pCharacterControl->ForwardRun();
                            isJumping = false;
                        }
                    }
                }

                

            }// !if (axes[0] != 0.0f || axes[1] != 0.0f)
            else
            {
                g_isCharacterMoving = false;
                
                // Joystick
                if (buttons[0] == GLFW_PRESS)
                    pCharacterControl->Jump();
                if (buttons[1] == GLFW_PRESS)
                    pCharacterControl->attack_03();
                else if (buttons[2] == GLFW_PRESS)
                    pCharacterControl->attack_01();
                else if (buttons[3] == GLFW_PRESS)
                    pCharacterControl->attack_02();
                else if (buttons[1] == GLFW_PRESS)
                    pCharacterControl->attack_03();
                else if (buttons[4] == GLFW_PRESS)
                {
                    characterOrientation = glm::rotate(characterOrientation, deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
                    pCharacterControl->TurnLeft180();
                }
                else if (buttons[5] == GLFW_PRESS)
                {
                    characterOrientation = glm::rotate(characterOrientation, deltaTime, glm::vec3(0.0f, -1.0f, 0.0f));
                    pCharacterControl->TurnRight180();
                }
                else
                    pCharacterControl->Idle();

                // Avoid the tilt of the capsule
                pCharacterControl->GetCharacter()->rigidBody->SetMatOrientation(characterOrientation);


                // Don't stop the falling
                pCharacterControl->GetCharacter()->rigidBody->SetLinearVelocityLocal(glm::vec3(0.0f, velocity.y, 0.0f));

            }// !else if (axes[0] != 0.0f || axes[1] != 0.0f)

            if (axes[2] != 0.0f || axes[3] != 0.0f)
            {
                g_camera.processJoystickMovement(axes[2] * deltaTime, axes[3] * deltaTime);
            }// !if (axes[0] != 0.0f || axes[1] != 0.0f)

            if (buttons[6] == GLFW_PRESS)
            {
                g_pSeceneManager->joystickStart();
            }
            
            if (buttons[7] == GLFW_PRESS)
            {
                g_pSeceneManager->joystickStart();
            }
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
