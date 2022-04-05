#pragma once
#include<list>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define MAX_LIGHTS 4

using namespace glm;

class Camera;
class ObjectInstance;
struct Light;

class Scene
{
public:

	Scene(Camera* camera, vec2 windowSize, Light* light, vec3 ambientLight);
	~Scene();

	void AddObjectInstance(ObjectInstance* objInstance);
	void RemoveObjectInstance(ObjectInstance* objInstance);

	void update(float deltaTime, float time);
	void draw();

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
	Camera* m_mainCamera;
	std::list<ObjectInstance*> m_objectInstances;

	// Lights
	Light* m_sunLight;
	vec3 m_ambientLight;
	std::vector<Light> m_pointLights;
	vec3 m_pointLightPositions[MAX_LIGHTS];
	vec3 m_pointLightColours[MAX_LIGHTS];
	bool m_drawPointLights = true;
};
