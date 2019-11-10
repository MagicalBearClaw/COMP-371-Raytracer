#pragma once

#include "Object.h"
#include "Material.h"

// plane primitive
// use analytic solution for intersection.
class Plane : public Object
{
public:
	Plane(glm::vec3 normal, glm::vec3 position, Material * material);
	~Plane() override;
	float Intersect(const Ray & ray) override;
	glm::vec3 GetNormalAtPoint(const glm::vec3& point) const override;

private:
	glm::vec3 _normal;
};
