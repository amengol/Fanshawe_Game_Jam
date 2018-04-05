/**
   Handles camera movements and modes 

   @author Jorge Amengol
   @version 1.1
   @date April 4th, 2018
   @note start code based on the tutorial:
   https://learnopengl.com/Getting-started/Camera
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. 
// Used as abstraction to stay away from window-system specific input methods
enum eCameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input 
// and calculates the corresponding Euler Angles, 
// Vectors and Matrices for use in OpenGL
class cCamera
{
public:
    // Camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_lookAt;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldm_up;
    // Euler Angles
    float m_yaw;
    float m_pitch;
    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    // Constructor with vectors
    cCamera(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);

    // Constructor with scalar values
    cCamera(float posX, float posY, float posZ,
            float upX, float upY, float upZ,
            float yaw, float pitch);

    // Returns the view matrix calculated using 
    // Euler Angles and the m_lookAt Matrix
    glm::mat4 getViewMatrix();

    // Processes input received from any keyboard-like input system. 
    //Accepts input parameter in the form of camera defined ENUM
    void processKeyboard(eCameraMovement direction, float deltaTime);

    // Processes input received from a mouse input system. 
    // Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, bool constrainm_pitch = true);

    // Processes input received from a mouse scroll-wheel event. 
    // Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

