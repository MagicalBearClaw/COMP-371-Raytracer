#include "Triangle.h"

Triangle::Triangle(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3, glm::vec3 normal, Material * material) :Object(glm::vec3(0), material), _vertice1(vert1), _vertice2(vert2), _vertice3(vert3), _normal(normal)
{
	// calculate the normals manually because of the 
	_normal = glm::normalize(
		glm::cross(_vertice2 - _vertice1, _vertice3 - _vertice1)
	);
}

float Triangle::Intersect(const Ray & ray)
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	const float epsilon = 0.0001;
	glm::vec3 edge1 = _vertice2 - _vertice1;
	glm::vec3 edge2 = _vertice3 - _vertice1;
	glm::vec3 pvec = glm::cross(ray.Direction, edge2);

	float determinant = glm::dot(edge1, pvec);
	if (std::fabs(determinant) < epsilon)
	{
		return -1;
	}

	float inverseDeterminant = 1 / determinant;

	glm::vec3 tvec = ray.Origin - _vertice1;
	float u = glm::dot(tvec, pvec) * inverseDeterminant;

	if (u < 0 || u > 1)
	{
		return -1;
	}

	glm::vec3 qvec = glm::cross(tvec, edge1);
	float v = glm::dot(ray.Direction, qvec) * inverseDeterminant;;

	if (v < 0 || u + v > 1)
	{
		return -1;
	}

	return glm::dot(edge2, qvec) * inverseDeterminant;
}

glm::vec3 Triangle::GetNormalAtPoint(const glm::vec3 & point) const
{
	return _normal;
}

Triangle::~Triangle()
{
}
