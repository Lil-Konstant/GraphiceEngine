#include "Camera.h"

void Camera::update(float deltaTime)
{
    aie::Input* input = aie::Input::getInstance();

    float thetaRadians = glm::radians(m_theta);
    float phiRadians = glm::radians(m_phi);
    //calculate the forwards and right axes and up axis for the camera 
    vec3 forward(cos(phiRadians) * cos(thetaRadians), sin(phiRadians), cos(phiRadians) * sin(thetaRadians));
    glm::vec3 right(-sin(thetaRadians), 0, cos(thetaRadians));
    glm::vec3 up(0, 1, 0);

    // W/S move the camera forwards and backwards
    if (input->isKeyDown(aie::INPUT_KEY_W))
    {
        m_position += forward * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_S))
    {
        m_position -= forward * deltaTime * m_moveSpeed;
    }

    // A/D move the camera left and right
    if (input->isKeyDown(aie::INPUT_KEY_A))
    {
        m_position -= right * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_D))
    {
        m_position += right * deltaTime * m_moveSpeed;
    }

    // Z/X move the camera up and down (globally)
    if (input->isKeyDown(aie::INPUT_KEY_E))
    {
        m_position += up * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_Q))
    {
        m_position -= up * deltaTime * m_moveSpeed;
    }

    if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += input->getMouseDeltaX() * m_panSpeed;
        m_phi = clamp<float>(m_phi + input->getMouseDeltaY() * m_panSpeed, -70, 70);
    }
}

mat4 Camera::getViewMatrix()
{
    // Get the forward vector of the camera for the glm::lookAt function
    float thetaRadians = radians(m_theta);
    float phiRadians = radians(m_phi);
    vec3 forwardVector(cos(phiRadians) * cos(thetaRadians), sin(phiRadians), cos(phiRadians) * sin(thetaRadians));
    // Construct the view matrix using a position, a position being looked at, and the world up
    return lookAt(m_position, m_position + forwardVector, vec3(0, 1, 0));
}

mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight)
{
    return perspective(m_fov, screenWidth / screenHeight, m_nearPlane, m_farPlane);
}
