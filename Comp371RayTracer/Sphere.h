#pragma once

#include <algorithm>

#include "Material.h"
#include "Object.h"

// sphere primitive
// use analytic solution for intersection.
class Sphere : public Object
{
public:
	Sphere(glm::vec3 position, float radius, Material * material);
	~Sphere() override;

	float Intersect(const Ray &ray) override;
	glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const override;

private:
	float _radius;
	bool solveQuadraticEquation(const float a, const float b, const float c, float &x0, float &x1) const;
};