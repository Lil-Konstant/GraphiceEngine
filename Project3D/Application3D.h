#pragma once
#include "Application.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "ObjectInstance.h"
#include "RenderTarget.h"

using namespace glm;
using namespace aie;

/// <summary>
/// Application3D is a class that derives from AIE::Application, and implements the logic for initialising and setting
/// up a scene composed of objects with meshes affected by multiple lights, drawn each frame loop using custom shaders, 
/// drawn first to a render texture and then to a fullscreen quad with post processing functionality. The class manages
/// setting up all scene objects, loading their meshes and adding them to the member scene. The class also manages displaying
/// the application's UI used for manipulating the scene in it's update loop (i.e. changing post processors, editing point 
/// light positions and colours). The Application base class runs an update loop that triggers the update and draw functions
/// every frame, which are used to trigger the same functions in the m_mainScene member variable, which in turn update and
/// draw all relevent scene objects and components.
/// </summary>
class Application3D : public aie::Application {
public:
	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// Reference to the main scene that encompasses the entire demonstration
	Scene* m_mainScene;
	
	// Model meshes used in the scene
	OBJMesh m_bunnyMesh;
	OBJMesh m_spearMesh;

	// Shader's used in application
	ShaderProgram m_simpleShader; // used for bunny object
	ShaderProgram m_phongShader; // used for spear objects
	ShaderProgram m_postShader; // used during post processing pass

	// Render target and quad mesh encompassing screenspace for post processing
	RenderTarget m_renderTarget;
	Mesh m_fullscreenQuad;

	// Scene lights (point lights are added to scene in initialisation)
	Light m_light;
	vec3 m_ambientLight;

	// Variables for selecting which post processor logic to use via ImGui UI
	static const int m_postProcessorsCount = 6; // Number of post processors to select from
	const char* m_postProcessors[m_postProcessorsCount] = { "Default", "Box Blur", "Distort", "Water Distort", "Invert", "Edge Detect" }; // Names of post processors to select from
	int m_selectedPostProcessor = 0; // Tracker for which in the dropdown is currently selected

	// Variables for selecting which point light to be editing via ImGui UI
	static const int m_pointLightCount = 2; // Number of point lights in the scene
	const char* m_pointLights[m_pointLightCount] = { "Point Light 1", "Point Light 2"}; // Names of point lights in the scene
	int m_selectedPointLight = 0; // Tracker for which point light is currently selected
};