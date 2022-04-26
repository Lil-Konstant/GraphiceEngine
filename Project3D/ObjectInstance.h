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

/// <summary>
/// ObjectInstance is a wrapper class that wraps the variables and drawing logic that is common for any object
/// placed in the application's main scene. That is, the class contains a member variable to track the current
/// transform of this ObjectInstance, as well as references to the Mesh and ShaderProgram used to draw this
/// ObjectInstance. The draw() function for this class simply binds the member shader program for use, and
/// then bind's the appropriate scene uniforms to the shader, as well as binding the appropriate uniforms
/// corresponding to this ObjectInstance (i.e. the Model transform).
/// </summary>
class ObjectInstance
{
public:

	// Custom constructors for initialising the m_transform of this ObjectInstance with either a matrix or a set of vectors
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
	aie::OBJMesh* m_mesh; // Pre-loaded mesh reference to draw this ObjectInstance with
	aie::ShaderProgram* m_shaderProgram; // Shader reference used to draw this ObjectInstance
};
