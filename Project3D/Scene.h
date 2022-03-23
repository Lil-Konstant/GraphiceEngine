#pragma once
#include<list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
	Light* getLight() { return m_mainLight; }
	vec3 getAmbientLight() { return m_ambientLight; }
	// Setters
	void setWindowSize(vec2 windowSize) { m_windowSize = windowSize; }

protected:

	vec2 m_windowSize;
	Camera* m_mainCamera;
	Light* m_mainLight;
	vec3 m_ambientLight;

	std::list<ObjectInstance*> m_objectInstances;
};

