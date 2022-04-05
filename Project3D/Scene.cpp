#include "Scene.h"
#include "ObjectInstance.h"
#include "Camera.h"
#include "Light.h"

Scene::Scene(Camera* camera, vec2 windowSize, Light* light, vec3 ambientLight)
{
	m_mainCamera = camera;
	m_windowSize = windowSize;
	m_sunLight = light;
	m_ambientLight = ambientLight;
}

Scene::~Scene()
{
	for (auto objectInstance : m_objectInstances)
	{
		delete objectInstance;
	}

	delete m_mainCamera;
}

void Scene::AddObjectInstance(ObjectInstance* objInstance)
{
	m_objectInstances.push_back(objInstance);
}

void Scene::RemoveObjectInstance(ObjectInstance* objInstance)
{
	m_objectInstances.remove(objInstance);
}

void Scene::update(float deltaTime, float time)
{
	// check for user input to update the main camera with
	m_mainCamera->update(deltaTime);

	// rotate the sunlight around the z axis
	//m_sunLight->direction = glm::normalize(vec3(glm::cos(time), 0, glm::sin(time)));

	for (auto objectInstance : m_objectInstances)
	{
		//objectInstance->setTransform(objectInstance->makeTransform(objectInstance->getPosition(), vec3(0, glm::cos(time * 10) * 25, 0), vec3(1, 1, 1)));
	}
}

void Scene::draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour * m_pointLights[i].intensity;
	}

	for (auto objectInstance : m_objectInstances)
	{
		objectInstance->draw(this);
	}

	if (m_drawPointLights)
	{
		for (auto pointLight : m_pointLights)
		{
			pointLight.drawGizmo();
		}
	}
}
