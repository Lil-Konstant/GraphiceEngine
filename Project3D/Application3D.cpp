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

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);
	// Initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false) 
	{
		printf("Render Target Error!\n");
		return false;
	}

	// Initialise the scene lights
	m_light.colour = { 1.0f, 1.0f, 1.0f };
	m_light.direction = { 0.707f, -0.707f, -0.707f };
	m_ambientLight = { 0.5f, 0.5f, 0.5f };

	// Initialise the main scene with a camera and some lights
	m_mainScene = new Scene(new Camera(-90.0f, -40.0f, { 0, 10, 10 }), vec2(getWindowWidth(), getWindowHeight()), &m_light, m_ambientLight);
	// Add two point lights to the main scene
	m_mainScene->getPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	m_mainScene->getPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

	// Load the vertex and fragment shaders into the simple and phong shader programs
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");
	// Attempt to link the shaders into a program, return if failed
	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader Error: %s\n", m_simpleShader.getLastError());
		return false;
	}if (m_phongShader.link() == false)
	{
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}if (m_postShader.link() == false)
	{
		printf("Post Shader Error: %s\n", m_postShader.getLastError());
		return false;
	}

	// Attempt to load the bunny obj in and add an instance of it to the scene
	if (m_bunnyMesh.load("./stanford/bunny.obj", true, true) == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}
	m_mainScene->AddObjectInstance(new ObjectInstance(&m_simpleShader, &m_bunnyMesh, vec3(8, 0, 8), vec3(0), vec3(0.2f)));
	// Attempt to load the spear obj in and add an instance of it to the scene
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Mesh Error!\n");
		return false;
	}
	for (int i = -5; i <= 5; i++)
	{
		m_mainScene->AddObjectInstance(new ObjectInstance(&m_phongShader, &m_spearMesh, vec3(i, 0, i), vec3(0, 0, 0)));
	}
	
	
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Couldn't load grid texture!\n");
		return false;
	}

	m_quad.initialiseQuad();
	m_fullscreenQuad.initialiseFullscreenQuad();

	return true;
}

/// <summary>
/// shutdown() is called when the user presses escape during the update() sequence, and simply calls
/// destroy on the Gizmo class and then deletes the mainScene member.
/// </summary>
void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_mainScene;
}

/// <summary>
/// 
/// </summary>
/// <param name="deltaTime"></param>
void Application3D::update(float deltaTime) 
{
	// Trigger the update sequence on the main scene
	m_mainScene->update(deltaTime, getTime());
	// Update the scene's window size incase it has changed
	m_mainScene->setWindowSize(vec2(getWindowWidth(), getWindowHeight()));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// Draw a simple grid with gizmos
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

	// Create a GUI panel for the controls information
	ImGui::Begin("Controls");
	ImGui::Text("- Use WASD to move around the scene.");
	ImGui::Text("- Use Q and E to move up and down respectively.");
	ImGui::Text("- Rotate the camera by moving the mouse while holding right click.");
	ImGui::End();

	// Create a GUI panel for selecting the post processing effect to use, and the direction and colour of the sunlight in the scene
	ImGui::Begin("Main Graphics Settings");
	ImGui::Combo("Post Processor Effect", &m_selectedPostProcessor, m_postProcessors, m_postProcessorsCount, -1);
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();
	
	// Create a GUI panel for the point light settings (pass the selected point light as reference for altering it's position and colour)
	ImGui::Begin("Point Light Settings");
	ImGui::Checkbox("Visible Point Light Gizmos?", m_mainScene->getDrawPointLights());
	ImGui::Combo("", &m_selectedPointLight, m_pointLights, m_pointLightCount, -1);
	std::vector<Light>& pointLights = m_mainScene->getPointLights();
	ImGui::DragFloat3("Position", &pointLights[m_selectedPointLight].direction[0], 0.1f);
	ImGui::DragFloat3("Colour", &pointLights[m_selectedPointLight].colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	// Quit the application if the user has pressed escape this frame
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/// <summary>
/// draw() is used called by the Application base class' update loop. The function first binds the member
/// m_renderTarget for use so whatever is drawn gets drawn to the frame buffer, and then calls draw on 
/// the member scene of the application, which iterates through and draws all ObjectInstance's managed
/// by the scene. The function will then bind the post processing shader for use (as well as it's uniforms),
/// and then finally call draw on the m_fullscreenQuad member, which effectively redraws the screen using the
/// initial scene drawing as a texture, allowing for post processing effects. 
/// </summary>
void Application3D::draw() {

	// Bind the render target for use
	m_renderTarget.bind();
	// wipe the screen to the background colour
	clearScreen();
	// draw all object instances in the scene
	m_mainScene->draw();
	// Draw the scene gizmos (the grid and the point lights if ticked to draw)
	Gizmos::draw(m_mainScene->getCamera()->getProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_mainScene->getCamera()->getViewMatrix());
	// Unbind the render target and clear the backbuffer
	m_renderTarget.unbind();
	clearScreen();

	// Now we bind the post processing shader and uniforms to redraw the scene for post processing
	m_postShader.bind();
	m_postShader.bindUniform("Time", getTime());
	m_postShader.bindUniform("selectedPostProcessor", m_selectedPostProcessor); // Dictates which processing function is called in post.frag
	m_postShader.bindUniform("renderTexture", 0);
	m_renderTarget.getTarget(0).bind(0); // Bind the renderTarget to the 0th texture slot for the uniform

	// Draw the fullscreen quad now that we have the initial scene drawing in the renderTexture uniform
	m_fullscreenQuad.draw();
}
