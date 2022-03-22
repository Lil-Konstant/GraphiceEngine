#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui.h"

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

	// initialise the main camera
	m_mainCamera = Camera(-90.0f, -40.0f, { 0, 10, 10 });

	// Load the vertex and fragment shaders into the shader program
	m_shaderProgram.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_shaderProgram.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	// Attempt to link the shaders into a program, return if failed
	if (m_shaderProgram.link() == false)
	{
		printf("Shader Error: %s\n", m_shaderProgram.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Couldn't load grid texture!\n");
		return false;
	}

	m_bunnyTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1 };

	m_quad.initialiseQuad();

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// check for user input to update the main camera with
	m_mainCamera.update(deltaTime);

	// rotate the light around the z axis
	m_light.direction = glm::normalize(vec3(glm::cos(time), 0, glm::sin(time)));

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

	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

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

	// Bind the shader program for use
	m_shaderProgram.bind();

	// Bind the ProjectionView transform
	auto pv = m_mainCamera.getProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_mainCamera.getViewMatrix();
	m_shaderProgram.bindUniform("ProjectionViewTransform", pv);
	// Bind the light uniforms
	m_shaderProgram.bindUniform("AmbientColour", m_ambientLight);
	m_shaderProgram.bindUniform("LightColour", m_light.colour);
	m_shaderProgram.bindUniform("LightDirection", m_light.direction);
	// Bind the camera uniform
	m_shaderProgram.bindUniform("CameraPosition", m_mainCamera.getPosition());

	m_shaderProgram.bindUniform("ModelTransform", m_bunnyTransform);
	m_bunnyMesh.draw();
	
	m_shaderProgram.bindUniform("Ka", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("Kd", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("Ks", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("ModelTransform", m_quadTransform);
	// bind the quad texture for quad drawing
	m_shaderProgram.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);
	m_quad.draw();

	// draw 3D gizmos
	Gizmos::draw(pv);
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
