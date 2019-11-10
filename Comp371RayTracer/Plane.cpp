#include "Plane.h"

Plane::Plane(glm::vec3 normal, glm::vec3 position, Material * material) : Object(position, material), _normal(normal)
{
}

Plane::~Plane()
{
}

float Plane::Intersect(const Ray & ray)
{
	// influenced by https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	float a = glm::dot(ray.Direction, _normal);

	if (a > 0)
	{
		return -1.0f;
	}

	return glm::dot(Position - ray.Origin, _normal) / a;
}

glm::vec3 Plane::GetNormalAtPoint(const glm::vec3 & point) const
{
	return _normal;
}
