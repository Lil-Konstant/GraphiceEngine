#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "imgui.h"
#include "Scene.h"

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
	// Initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// Initialise the scene lights
	m_light.colour = { 1.0f, 1.0f, 1.0f };
	m_ambientLight = { 0.5f, 0.5f, 0.5f };

	// Initialise the main scene with a camera and some lights
	m_mainScene = new Scene(new Camera(-90.0f, -40.0f, { 0, 10, 10 }), vec2(getWindowWidth(), getWindowHeight()), &m_light, m_ambientLight);

	// Load the vertex and fragment shaders into the shader program
	m_shaderProgram.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_shaderProgram.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	// Attempt to link the shaders into a program, return if failed
	if (m_shaderProgram.link() == false)
	{
		printf("Shader Error: %s\n", m_shaderProgram.getLastError());
		return false;
	}

	// Attempt to load the spear obj in and add an instance of it to the scene
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}
	for (int i = -5; i <= 5; i++)
	{
		m_mainScene->AddObjectInstance(new ObjectInstance(&m_shaderProgram, &m_spearMesh, vec3(i, 0, i), vec3(0, 0, 0)));
	}
	
	
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Couldn't load grid texture!\n");
		return false;
	}

	m_quadTransform = {
		15, 0, 0, 0,
		0, 15, 0, 0,
		0, 0, 15, 0,
		0, 0, 0, 1 };

	m_quad.initialiseQuad();

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_mainScene;
}

void Application3D::update(float deltaTime) 
{
	m_mainScene->update(deltaTime, getTime());
	// Update the scene's window size incase it has changed
	m_mainScene->setWindowSize(vec2(getWindowWidth(), getWindowHeight()));

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

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// draw all object instances in the scene
	m_mainScene->draw();
	
	m_shaderProgram.bindUniform("Ka", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("Kd", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("Ks", vec3(1, 1, 1));
	m_shaderProgram.bindUniform("ModelTransform", m_quadTransform);
	// bind the quad texture for quad drawing
	m_shaderProgram.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);
	m_quad.draw();

	// draw 3D gizmos
	Gizmos::draw(m_mainScene->getCamera()->getProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_mainScene->getCamera()->getViewMatrix());
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
	//m_unitCube.initialise(8, vertices, 36, indices);

	// Make the quad 1 units wide
	/*m_unitCubeTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0, 1 };*/
}
