#pragma once
#include "Application.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "ObjectInstance.h"

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
	ShaderProgram m_shaderProgram;
	Light m_light;
	vec3 m_ambientLight;

	OBJMesh m_spearMesh;
	ObjectInstance* m_spearInstance;

	Texture m_gridTexture;
	Mesh m_quad;
	mat4 m_quadTransform;
};