#pragma once

#include <glm/glm.hpp>

struct Camera
{;
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 CameraRight;
	glm::vec3 CameraDown;
	float FOV;
	float AspectRatio;
	float FocalLength;

};
