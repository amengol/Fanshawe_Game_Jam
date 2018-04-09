#include "cCamera.h"
#include "cGameObject.h"

cCamera::cCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    m_lookAt(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movementSpeed(SPEED),
    m_movementSpeedFactor(1.0f),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM),
    m_cameraMode(FREE),
    m_radiusFromTarget(5.0f),
    m_cameraGO(NULL)
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
    m_movementSpeedFactor(1.0f),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM),
    m_cameraMode(FREE),
    m_radiusFromTarget(5.0f),
    m_cameraGO(NULL)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldm_up = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

glm::mat4 cCamera::getViewMatrix()
{
    switch (m_cameraMode)
    {
    case FREE:
        return glm::lookAt(m_position, m_position + m_lookAt, m_up);
        break;
    case THIRD_PERSON:
        return glm::lookAt(m_position, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    default:
        break;
    }
    
}

void cCamera::processKeyboard(eCameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * m_movementSpeedFactor * deltaTime;
    if (direction == FORWARD)
        m_position += m_lookAt * velocity;
    if (direction == BACKWARD)
        m_position -= m_lookAt * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
    if (direction == UP)
        m_position += m_up * velocity;
    if (direction == DOWN)
        m_position -= m_up * velocity;

    // Constraint the camera to not go under the ground
    if (m_position.y <= 0.3f)
        m_position.y = 0.3f;
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

    // Limit the pitch in THIRD_PERSON mode
    if (m_cameraMode == THIRD_PERSON)
    {
        if (m_pitch > 5.0f)
            m_pitch = 5.0f;
    }

    // m_update Front, m_right and m_up Vectors using the updated Euler angles
    //updateCameraVectors();
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

void cCamera::setYaw(float degrees)
{
    if (fabs(degrees) > 360.0f)
    {
        m_yaw = fmod(degrees, 360.0f);
    }
    else
    {
        m_yaw = degrees;
    }
    
    //updateCameraVectors();
}

void cCamera::setPitch(float degrees)
{
    if (degrees > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (degrees < -89.0f)
    {
        m_pitch = -89.0f;
    }
    else
    {
        m_pitch = degrees;
    }
    
    //updateCameraVectors();
}

void cCamera::lockOnGameObject(cGameObject* GO)
{
    if (GO != NULL)
    {
        if (GO->rigidBody != NULL)
        {
            m_cameraGO = GO;
            m_cameraMode = THIRD_PERSON;
            GO->rigidBody->GetPostion(m_lookAt);
            m_lookAt += glm::vec3(0.0f, 1.5f, 0.0f);
            m_yaw -= -180.0f;

            // Calculate a direction to guide the new position of the camera
            // related to the center of the Point of Interest
            glm::vec3 posDirection;
            posDirection.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            posDirection.y = sin(glm::radians(-m_pitch));
            posDirection.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            posDirection = glm::normalize(posDirection);

            // Project the new position and assign        
            m_position = m_lookAt + posDirection * m_radiusFromTarget;
        }
    }
}

void cCamera::releaseGameObject()
{
    m_cameraGO = NULL;
    m_cameraMode = FREE;
    
    // Make the way back to the FREE camera mode
    m_lookAt = glm::normalize(m_position - m_lookAt) * -1.0f;
    m_yaw -= -180.0f;

    // Also re-calculate the m_right and m_up vector
    m_right = glm::normalize(glm::cross(m_lookAt, m_worldm_up));    // Normalize the vectors, 
                                                                    // because their length gets closer to 0 
                                                                    // the more you look up or down which 
                                                                    // results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_lookAt));
}

void cCamera::updateCameraVectors()
{
    switch (m_cameraMode)
    {
    case FREE:
    {
        // Calculate the new LookAt vector
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
        break;
    case THIRD_PERSON:
    {
        // LookAt by GameObject
        m_cameraGO->rigidBody->GetPostion(m_lookAt);
        m_lookAt += glm::vec3(0.0f, 1.5f, 0.0f);

        // Calculate a direction to guide the new position of the camera
        // related to the center of the Point of Interest
        glm::vec3 posDirection;
        posDirection.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        posDirection.y = sin(glm::radians(-m_pitch));
        posDirection.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        posDirection = glm::normalize(posDirection);
        
        // Project the new position and assign        
        m_position = m_lookAt + posDirection * m_radiusFromTarget;
    }
        break;
    default:
        break;
    }
}
