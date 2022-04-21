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

class Application3D : public aie::Application {
public:
	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void createUnitCube();

protected:

	Scene* m_mainScene;
	
	ShaderProgram m_simpleShader;
	ShaderProgram m_phongShader;
	ShaderProgram m_postShader;
	RenderTarget m_renderTarget;

	Light m_light;
	vec3 m_ambientLight;

	// Model meshes
	OBJMesh m_bunnyMesh;
	OBJMesh m_spearMesh;

	Texture m_gridTexture;
	Mesh m_quad;
	mat4 m_quadTransform;

	Mesh m_fullscreenQuad;
	
	static const int m_postProcessorsCount = 6;
	const char* m_postProcessors[m_postProcessorsCount] = { "Default", "Box Blur", "Distort", "Water Distort", "Invert", "Cellshade" };
	int m_selectedPostProcessor = 0;

	static const int m_pointLightCount = 2;
	const char* m_pointLights[m_pointLightCount] = { "Point Light 1", "Point Light 2"};
	int m_selectedPointLight = 0;
};