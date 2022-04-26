#include "Scene.h"
#include "ObjectInstance.h"
#include "Camera.h"
#include "Light.h"

/// <summary>
/// Scene's only constructor simply takes it's inputs and with them 
/// initialises the main camera to be used for drawing from, the size 
/// of the application, the main directional light in the scene, as 
/// well as the colour value of the ambient lighting of the scene.
/// </summary>
/// <param name="camera">Main camera for the scene, of which the perspective will be used to draw from.</param>
/// <param name="windowSize">Width (x) and height (y) of the application window.</param>
/// <param name="mainLight">Main directional light for the scene.</param>
/// <param name="ambientLight">Background ambient lighting of the scene, as a colour value.</param>
Scene::Scene(Camera* camera, vec2 windowSize, Light* mainLight, vec3 ambientLight)
{
	m_mainCamera = camera;
	m_windowSize = windowSize;
	m_sunLight = mainLight;
	m_ambientLight = ambientLight;
}

/// <summary>
/// ~Scene() simply calls delete on all of the ObjectInstance's managed by this scene, and then calls delete
/// on the main camera of the scene.
/// </summary>
Scene::~Scene()
{
	for (auto objectInstance : m_objectInstances)
	{
		delete objectInstance;
	}

	delete m_mainCamera;
}

/// <summary>
/// AddObjectInstance takes an input of a pointer to the specific ObjectInstance to add to this scene,
/// and simply adds it to the m_objectInstances list using the std::push_back() function.
/// </summary>
/// <param name="objInstance"></param>
void Scene::AddObjectInstance(ObjectInstance* objInstance)
{
	m_objectInstances.push_back(objInstance);
}

/// <summary>
/// RemoveObjectInstance takes an input of a pointer to the specific ObjectInstance to remove from this
/// scene, and will attempt to remove it from the scene using the std::remove() function.
/// </summary>
/// <param name="objInstance"></param>
void Scene::RemoveObjectInstance(ObjectInstance* objInstance)
{
	m_objectInstances.remove(objInstance);
}

/// <summary>
/// The Scene::update() function is called each loop of Application3D::update(), and simply calls update on
/// the main camera of the scene to check for user input in moving or rotating it this frame.
/// </summary>
void Scene::update(float deltaTime, float time)
{
	// check for user input to update the main camera with
	m_mainCamera->update(deltaTime);
}

/// <summary>
/// draw() is called each loop of Application3D::draw(), and first simply updates the lightPositions and lightColours
/// arrays with the current positions and colours of the scene's point lights. Then, the function iterates through
/// each objectInstance managed by the scene and calls draw on it, passing the scene itself as a parameter into
/// the ObjectInstance draw() function. The function will then iterate through all of the point lights and draw
/// gizmos to visualise their positions if the member bool m_drawPointLights is true.
/// </summary>
void Scene::draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		// Multiply each point light's colour by it's intensity so it is passed to the shader uniform when drawing objects
		m_pointLightColours[i] = m_pointLights[i].colour * m_pointLights[i].intensity;
	}

	// Draw all of the objectInstance's in this scene
	for (auto objectInstance : m_objectInstances)
	{
		objectInstance->draw(this);
	}

	// Draw the point light gizmos if drawPointLights is true
	if (m_drawPointLights)
	{
		for (auto pointLight : m_pointLights)
		{
			pointLight.drawGizmo();
		}
	}
}
