#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Input.h"

using namespace glm;

class Camera
{
public:
	Camera() : m_theta(0), m_phi(0), m_position({ 0, 0, 0 }), m_fov(pi<float>() * 0.25f) {}
	Camera(float theta, float phi, vec3 position, float fov = pi<float>() * 0.25f) : m_theta(theta), m_phi(phi), m_position(position), m_fov(fov) {}
	~Camera() {}

	void update(float deltaTime);

	mat4 getViewMatrix();
	mat4 getProjectionMatrix(float screenWidth, float screenHeight);
	vec3 getPosition() { return m_position; }

private:
	//View variables
	float m_theta; // left right angle
	float m_phi; // up down angle
	vec3 m_position;
	
	// Perspective variables
	float m_fov;
	float m_nearPlane = 0.1f;
	float m_farPlane = 1000.0f;

	// Camera movement variables
	float m_moveSpeed = 7.0f;
	float m_panSpeed = 0.1f;
};

