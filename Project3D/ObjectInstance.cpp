#include "ObjectInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "OBJMesh.h"
#include "Shader.h"

/// <summary>
/// draw() takes an an input pointer to the scene we're drawing from, and goes through and binds
/// all of the appropriate and corresponding uniforms for this ObjectInstance, before calling the
/// draw function on the mesh of this ObjectInstance itself.
/// </summary>
/// <param name="scene">The scene this ObjectInstance is being drawn from.</param>
void ObjectInstance::draw(Scene* scene)
{
	// Bind the shader program used for this ObjectInstance
	m_shaderProgram->bind();

	// Bind the camera position and ProjectionView uniforms using the scene main camera
	m_shaderProgram->bindUniform("CameraPosition", scene->getCamera()->getPosition());
	auto pv = scene->getCamera()->getProjectionMatrix(scene->getWindowSize().x, scene->getWindowSize().y) * scene->getCamera()->getViewMatrix();
	m_shaderProgram->bindUniform("ProjectionViewTransform", pv);
	// Bind the model transform uniform using this ObjectInstance's transform
	m_shaderProgram->bindUniform("ModelTransform", m_transform);

	// Bind the light uniforms for ambient lighting, main sunlighting, and the point lights in the scene
	m_shaderProgram->bindUniform("AmbientColour", scene->getAmbientLight());
	m_shaderProgram->bindUniform("LightColour", scene->getSunlight()->colour);
	m_shaderProgram->bindUniform("LightDirection", scene->getSunlight()->direction);
	int numLights = scene->getNumLights();
	m_shaderProgram->bindUniform("numLights", numLights);
	m_shaderProgram->bindUniform("PointLightColours", numLights, scene->getPointLightColours());
	m_shaderProgram->bindUniform("PointLightPositions", numLights, scene->getPointLightPositions());
	
	// Draw the mesh of this ObjectInstance now the uniforms have been set correctly
	m_mesh->draw();
}

/// <summary>
/// makeTransform() is a utility function that takes vec3 inputs for a position, set of euler angles, 
/// and scale, and uses them to create a corresponding transformation matrix to represent said 
/// position, rotation and scale. This function is called by one of the two custom constructors for
/// ObjectInstance, and allows objects to be instantiated using vectors without having to input the
/// transformation matrix yourself.
/// </summary>
/// <param name="position">The desired position for the transformation matrix.</param>
/// <param name="eulerAngles">The desired rotation in euler angles for the transformation matrix.</param>
/// <param name="scale">The desired scale for the transformation matrix.</param>
/// <returns>A 4x4 transformation matrix made up of the input parameters.</returns>
glm::mat4 ObjectInstance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}
