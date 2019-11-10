
#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, float radius, Material * material) : Object(position, material), _radius(radius)
{
}

Sphere::~Sphere()
{
}

float Sphere::Intersect(const Ray & ray)
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
	float t0 = 0.0f;
	float t1 = 0.0f;
	glm::vec3 dir = ray.Direction;
	glm::vec3 L = ray.Origin - Position;
	float a = glm::dot(dir, dir);
	float b = 2 * glm::dot(dir, L);
	float c = glm::dot(L, L) - pow(_radius, 2);

	if (!solveQuadraticEquation(a, b, c, t0, t1)) 
		return -1;

	if (t0 > t1) 
		std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; 
		if (t0 < 0) 
			return -1;
	}
	// perform backface culling.
	if (glm::dot(ray.Direction, glm::normalize((ray.Origin + ray.Direction * t0) - Position)) > 0)
		return -1;

	return t0;
}

glm::vec3 Sphere::GetNormalAtPoint(const glm::vec3 & point) const
{
	return normalize(point - Position);
}

bool Sphere::solveQuadraticEquation(const float a, const float b, const float c, float& x0, float& x1) const
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}
