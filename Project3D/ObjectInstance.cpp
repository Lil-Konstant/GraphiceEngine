#include "ObjectInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "OBJMesh.h"
#include "Shader.h"

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

	// Bind the light uniforms
	m_shaderProgram->bindUniform("AmbientColour", scene->getAmbientLight());
	m_shaderProgram->bindUniform("LightColour", scene->getLight()->colour);
	m_shaderProgram->bindUniform("LightDirection", scene->getLight()->direction);
	
	// Draw the mesh of this ObjectInstance now the uniforms have been set correctly
	m_mesh->draw();
}

glm::mat4 ObjectInstance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}
