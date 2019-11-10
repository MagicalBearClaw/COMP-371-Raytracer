#pragma once

#include <glm/glm.hpp>

#include "Material.h"

struct Light
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 SpecularColor;
};
