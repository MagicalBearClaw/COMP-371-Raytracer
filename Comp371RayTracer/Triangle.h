#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Object.h"
#include "Material.h"

// triangle primitive
// use analytic solution for intersection.
class Triangle : public Object
{
public:
	Triangle(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3, glm::vec3 normal, Material * material);
	float Intersect(const Ray &ray) override;
	glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const override;
	~Triangle();
private:
	glm::vec3 _vertice1;
	glm::vec3 _vertice2;
	glm::vec3 _vertice3;
	glm::vec3 _normal;
};