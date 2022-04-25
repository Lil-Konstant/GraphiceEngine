#pragma once
#include "Gizmos.h"
#include "glm/vec3.hpp"

/// <summary>
/// Light is a class used to represent both the directional lights and point lights in the main scene.
/// When representing the main directional light in the scene, the member variable direction represents
/// the direction the global direction of the light, however when representing the points lights in the 
/// scene, direction is instead used to represent the position of the point light. The class contains the
/// member variable intensity which is multiplied with the light colour before being passed to the shader
/// uniform each scene draw call.
/// </summary>
struct Light
{
	Light() : direction(glm::vec3(0)), colour(glm::vec3(0)), intensity(0) {}
	Light(glm::vec3 aDirection, glm::vec3 aColour, float aIntensity)
	{
		direction = aDirection;
		colour = aColour;
		intensity = aIntensity;
	}

	// drawGizmo simply draws of small sphere gizmo at the position of the point light using it's colour
	void drawGizmo() { aie::Gizmos::addSphere(direction, 0.2f, 20, 20, vec4(colour * intensity, 1)); }

	glm::vec3 direction;
	glm::vec3 colour;
	float intensity;
};