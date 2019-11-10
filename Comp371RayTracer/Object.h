#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Material.h"
#include "common.h"


// base objects for all scene objects, excludes lights and camera.
class Object
{
public:
	Object(glm::vec3 position, Material * material);
	virtual ~Object();
	virtual float Intersect(const Ray&) = 0;
	virtual glm::vec3 GetNormalAtPoint(const glm::vec3 & point) const = 0;
	glm::vec3 Position;
	Material * ObjectMaterial;
};
