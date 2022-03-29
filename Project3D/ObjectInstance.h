#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

// Forward declarations of classes defined elsewhere
namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}
class Scene;
class Camera;
struct Light;

class ObjectInstance
{
public:

	ObjectInstance(aie::ShaderProgram* shaderProgram, aie::OBJMesh* mesh, glm::mat4 transform = glm::mat4(1)) : m_shaderProgram(shaderProgram), m_mesh(mesh), m_transform(transform) {}
	ObjectInstance(aie::ShaderProgram* shaderProgram, aie::OBJMesh* mesh, glm::vec3 position, glm::vec3 eulerRotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1)) : m_shaderProgram(shaderProgram), m_mesh(mesh), m_transform(makeTransform(position, eulerRotation, scale)) {}
	~ObjectInstance() {}

	void draw(Scene* scene);
	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
	
	// Getters
	glm::vec3 getPosition() { return glm::vec3(m_transform[3][0], m_transform[3][1], m_transform[3][2]); }
	// Setters
	void setTransform(glm::mat4 transform) { m_transform = transform; }

protected:

	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shaderProgram;
};

