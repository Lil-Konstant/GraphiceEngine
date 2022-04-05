#pragma once
#include "Gizmos.h"
#include "glm/vec3.hpp"

struct Light
{
	Light() : direction(glm::vec3(0)), colour(glm::vec3(0)), intensity(0) {}
	Light(glm::vec3 aDirection, glm::vec3 aColour, float aIntensity)
	{
		direction = aDirection;
		colour = aColour;
		intensity = aIntensity;
	}

	void drawGizmo() { aie::Gizmos::addSphere(direction, 0.2f, 20, 20, vec4(colour * intensity, 1)); }

	glm::vec3 direction;
	glm::vec3 colour;
	float intensity;
};