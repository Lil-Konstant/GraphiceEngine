#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	m_light.colour = { 1.0f, 1.0f, 1.0f };
	m_ambientLight = { 0.5f, 0.5f, 0.5f };

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// Load the vertex and fragment shaders into the shader program
	m_shaderProgram.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_shaderProgram.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	
	// Attempt to link the shaders into a program, return if failed
	if (m_shaderProgram.link() == false)
	{
		printf("Shader Error: %s\n", m_shaderProgram.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/buddha.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	createUnitCube();

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate the light around the z axis
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// rotate camera
	/*m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, glm::sin(time) * 10, glm::cos(time) * 10),
							   vec3(0), vec3(0, 1, 0));*/

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	//Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	/*Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);*/

	//// demonstrate 2D gizmos
	//Gizmos::add2DAABB(glm::vec2(getWindowWidth() / 2, 100),
	//				  glm::vec2(getWindowWidth() / 2 * (fmod(getTime(), 3.f) / 3), 20),
	//				  vec4(0, 1, 1, 1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// Bind the shader program for use
	m_shaderProgram.bind();

	// Bind the ProjectionView and Model uniforms for the bunny
	auto pv = m_projectionMatrix * m_viewMatrix;
	m_shaderProgram.bindUniform("ProjectionViewTransform", pv);
	m_shaderProgram.bindUniform("ModelTransform", m_bunnyTransform);
	// Bind the light uniforms
	m_shaderProgram.bindUniform("AmbientColour", m_ambientLight);
	m_shaderProgram.bindUniform("LightColour", m_light.colour);
	m_shaderProgram.bindUniform("LightDirection", m_light.direction);
	// Bind the camera uniform
	m_shaderProgram.bindUniform("CameraPosition", glm::inverse(m_viewMatrix)[3]);

	m_bunnyMesh.draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

void Application3D::createUnitCube()
{
	Mesh::Vertex vertices[8];
	vertices[0].position = { -0.5f, -0.5f, 0.5f, 1 };
	vertices[1].position = { 0.5f, -0.5f, 0.5f, 1 };
	vertices[2].position = { -0.5f, -0.5f, -0.5f, 1 };
	vertices[3].position = { 0.5f, -0.5f, -0.5f, 1 };
	vertices[4].position = { -0.5f, 0.5f, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0.5f, 0.5f, 1 };
	vertices[6].position = { -0.5f, 0.5f, -0.5f, 1 };
	vertices[7].position = { 0.5f, 0.5f, -0.5f, 1 };

	unsigned int indices[36] = { 2, 1, 0, 3, 1, 2, 4, 5, 6, 5, 7, 6, 2, 6, 3, 6, 7, 3, 4, 6, 0, 6, 2, 0, 5, 4, 0, 5, 0, 1, 7, 5, 1, 1, 3, 7 };

	// Initialise a test quad with custom vertices
	m_unitCube.initialise(8, vertices, 36, indices);

	// Make the quad 1 units wide
	m_unitCubeTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0, 1 };
}
