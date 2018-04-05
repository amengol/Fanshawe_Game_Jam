#include "cCamera.h"

cCamera::cCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    m_lookAt(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movementSpeed(SPEED),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM)
{
    m_position = position;
    m_worldm_up = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

cCamera::cCamera(float posX, float posY, float posZ, 
                 float upX, float upY, float upZ, 
                 float yaw, float pitch) :
    m_lookAt(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movementSpeed(SPEED),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldm_up = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

glm::mat4 cCamera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_lookAt, m_up);
}

void cCamera::processKeyboard(eCameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_lookAt * velocity;
    if (direction == BACKWARD)
        m_position -= m_lookAt * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void cCamera::processMouseMovement(float xoffset, float yoffset, bool constrainm_pitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainm_pitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // m_update Front, m_right and m_up Vectors using the updated Euler angles
    updateCameraVectors();
}

void cCamera::processMouseScroll(float yoffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yoffset;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

void cCamera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 lookAt;
    lookAt.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    lookAt.y = sin(glm::radians(m_pitch));
    lookAt.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_lookAt = glm::normalize(lookAt);

    // Also re-calculate the m_right and m_up vector
    m_right = glm::normalize(glm::cross(m_lookAt, m_worldm_up));    // Normalize the vectors, 
                                                                    // because their length gets closer to 0 
                                                                    // the more you look up or down which 
                                                                    // results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_lookAt));
}
