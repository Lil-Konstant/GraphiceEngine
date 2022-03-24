#pragma once
#include "glm/vec3.hpp"

struct Light
{
	Light() : direction(glm::vec3(0)), colour(glm::vec3(0)) {}
	Light(glm::vec3 aDirection, glm::vec3 aColour, float intensity)
	{
		direction = aDirection;
		colour = aColour * intensity;
	}

	glm::vec3 direction;
	glm::vec3 colour;
};