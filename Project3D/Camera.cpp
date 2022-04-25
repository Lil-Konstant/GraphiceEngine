#include "Camera.h"

/// <summary>
/// update() is a standard update function that gets called by the scene maintaining this camera every
/// scene update(), and simply checks for all valid user input for moving and rotating the camera. This
/// includes WASD inputs (which moves the camera along it's forward and right vectors), Q and E inputs
/// (which moves the camera along it's up vector), and x and y mouse delta while the user is holding right
/// click.
/// </summary>
void Camera::update(float deltaTime)
{
    // Store the input data for this frame
    aie::Input* input = aie::Input::getInstance();

    // Calculate the forward and right axes for the camera based on it's current spherical rotation.
    float thetaRadians = glm::radians(m_theta);
    float phiRadians = glm::radians(m_phi);
    vec3 forward(cos(phiRadians) * cos(thetaRadians), sin(phiRadians), cos(phiRadians) * sin(thetaRadians));
    glm::vec3 right(-sin(thetaRadians), 0, cos(thetaRadians));
    glm::vec3 up(0, 1, 0);

    // W/S move the camera forwards and backwards (scaled by deltaTime and the camera m_moveSpeed)
    if (input->isKeyDown(aie::INPUT_KEY_W))
    {
        m_position += forward * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_S))
    {
        m_position -= forward * deltaTime * m_moveSpeed;
    }

    // A/D move the camera left and right (scaled by deltaTime and the camera m_moveSpeed)
    if (input->isKeyDown(aie::INPUT_KEY_A))
    {
        m_position -= right * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_D))
    {
        m_position += right * deltaTime * m_moveSpeed;
    }

    // E/Q move the camera up and down globally (scaled by deltaTime and the camera m_moveSpeed)
    if (input->isKeyDown(aie::INPUT_KEY_E))
    {
        m_position += up * deltaTime * m_moveSpeed;
    }
    if (input->isKeyDown(aie::INPUT_KEY_Q))
    {
        m_position -= up * deltaTime * m_moveSpeed;
    }

    // Mouse delta's will pan the camera while holding right click (scaled the camera m_panSpeed)
    if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += input->getMouseDeltaX() * m_panSpeed;
        m_phi = clamp<float>(m_phi + input->getMouseDeltaY() * m_panSpeed, -70, 70);
    }
}

/// <summary>
/// getViewMatrix is a utility function used to construct the view matrix of this Camera based on the current
/// theta and phi rotations of the camera, as well as the current position of the camera. The function uses
/// the glm::lookAt function to do this. 
/// </summary>
/// <returns>A 4x4 matrix representing the view transform of this camera.</returns>
mat4 Camera::getViewMatrix()
{
    // Get the forward vector of the camera for the glm::lookAt function
    float thetaRadians = radians(m_theta);
    float phiRadians = radians(m_phi);
    vec3 forwardVector(cos(phiRadians) * cos(thetaRadians), sin(phiRadians), cos(phiRadians) * sin(thetaRadians));
    // Construct the view matrix using a position, a position being looked at, and the world up
    return lookAt(m_position, m_position + forwardVector, vec3(0, 1, 0));
}

/// <summary>
/// getProjectionMatrix is a utility function used to construct the projection matrix of this Camera
/// based on the fov, screen width and height, and near and far planes. The function does this by simply
/// using the glm::perspective function.
/// </summary>
/// <param name="screenWidth">Width of the application screen.</param>
/// <param name="screenHeight">Height of the application screen.</param>
/// <returns>A 4x4 matrix representing the projection transform of this camera.</returns>
mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight)
{
    return perspective(m_fov, screenWidth / screenHeight, m_nearPlane, m_farPlane);
}
