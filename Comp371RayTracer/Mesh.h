#pragma once

#include <glm/glm.hpp>
#include <string>
#include <iostream>

#include "Material.h"
#include "Object.h"
#include "Triangle.h"
#include "tiny_obj_loader.h"

/*
	Mesh is a colelction of triangles.
	Intersection is done by finiding closets triangle.
*/
class Mesh :  public Object
{
public:
	Mesh(Material * material);
	~Mesh() override;
	bool LoadMesh(const std::string& fileName);
private:
	float Intersect(const Ray &) override;
	glm::vec3 GetNormalAtPoint(const glm::vec3 & point) const override;
	void CreateTriangles(const std::vector<size_t>& indicies);
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec3> _normals;
	std::vector<Triangle *> _triangles;

	glm::vec3 _normal;

	const float _episilon = 0.00001f;
};
