#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	glm::vec3 Origin;
	glm::vec3 Direction;
};