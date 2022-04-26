#pragma once
#include<list>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define MAX_LIGHTS 4 // Max number of lights that can affect one object in the scene

using namespace glm;

// Forward declarations of classes defined elsewhere
class Camera;
class ObjectInstance;
struct Light;

/// <summary>
/// The Scene class holds reference to all of the common world variables required for an
/// ObjectInstance to be drawn to the screen, that is, primarily, a reference to the main
/// camera for transforming ObjectInstance's into screenspace for drawing, as well as references
/// to all of the lights present in the scene that should affect the colouring and lighting
/// of scene objects. The Scene class also holds a list of reference to all the ObjectInstance's
/// currently in the scene, and each frame cycle will iterate through them and trigger their
/// draw() functions, passing itself as a reference to each ObjectInstance for uniform binding.
/// </summary>
class Scene
{
public:

	Scene(Camera* camera, vec2 windowSize, Light* mainLight, vec3 ambientLight);
	~Scene();

	// Utility functions
	void AddObjectInstance(ObjectInstance* objInstance);
	void RemoveObjectInstance(ObjectInstance* objInstance);

	void update(float deltaTime, float time); // Call update on the camera to check for user input
	void draw(); // Call draw on all objects in the scene

	// Getters
	vec2 getWindowSize() { return m_windowSize; }
	Camera* getCamera() { return m_mainCamera; }
	Light* getSunlight() { return m_sunLight; }
	std::vector<Light>& getPointLights() { return m_pointLights; }
	vec3 getAmbientLight() { return m_ambientLight; }
	int getNumLights() { return m_pointLights.size(); }
	vec3* getPointLightPositions() { return &m_pointLightPositions[0]; }
	vec3* getPointLightColours() { return &m_pointLightColours[0]; }
	bool* getDrawPointLights() { return &m_drawPointLights; }
	// Setters
	void setWindowSize(vec2 windowSize) { m_windowSize = windowSize; }

protected:

	vec2 m_windowSize;
	Camera* m_mainCamera; // Virtual camera for transforming mesh data to screenspace
	std::list<ObjectInstance*> m_objectInstances; // List of ObjectInstance references to maintain and draw in this scene object

	// Variables for the scene lights
	Light* m_sunLight;
	vec3 m_ambientLight;
	std::vector<Light> m_pointLights;
	vec3 m_pointLightPositions[MAX_LIGHTS]; // Array of point light positions, filled using m_pointLights every update for shader uniform
	vec3 m_pointLightColours[MAX_LIGHTS]; // Array of point light colours, filled using m_pointLights every update for shader uniform
	bool m_drawPointLights = true; // Whether or not to draw point light gizmos, variable is altered by ImGui UI
};
