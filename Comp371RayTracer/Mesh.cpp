#include "Mesh.h"

Mesh::Mesh(Material * material) : Object(glm::vec3(0.f), material)
{
}

Mesh::~Mesh()
{
}

bool Mesh::LoadMesh(const std::string & fileName)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str());

	tinyobj::shape_t shape = shapes[0];

	for (size_t i = 0, len = attrib.vertices.size(); i < len; i += 3) {
		_vertices.push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i+1], attrib.vertices[i+2]));
	}

	for (size_t i = 0, len = attrib.normals.size(); i < len; i += 3) {
		_normals.push_back(glm::vec3(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]));
	}

	// iteriate over the faces to get the vertex indicies and create triangles base on the
	// data.
	size_t index_offset = 0;
	for (int fv : shape.mesh.num_face_vertices) {
		std::vector<size_t> vertex_indices;
		for (size_t v = 0; v < fv; v++) {
			vertex_indices.emplace_back(shape.mesh.indices[index_offset + v].vertex_index);
		}
		// create the triangles
		CreateTriangles(vertex_indices);
		// go to next face.
		index_offset += fv;
	}

	return true;
}

float Mesh::Intersect(const Ray& ray)
{
	int closetsIntersectedTriangleIndex = -1;
	float minimumIntersectionRayLength = INFINITY;
	// find the closets intersecting triangle on the mesh.
	for (size_t triangleIndex = 0; triangleIndex < _triangles.size(); triangleIndex++)
	{
		float rayLength = _triangles[triangleIndex]->Intersect(ray);
		if (rayLength < minimumIntersectionRayLength && rayLength > _episilon)
		{
			minimumIntersectionRayLength = rayLength;
			closetsIntersectedTriangleIndex = triangleIndex;
		}
	}
	// if we found an intersecting triangle set the mesh normal to the triangles normal.
	if (closetsIntersectedTriangleIndex >= 0)
	{
		_normal = _triangles[closetsIntersectedTriangleIndex]->GetNormalAtPoint(glm::vec3(0.f));
	}

	return minimumIntersectionRayLength;
}

glm::vec3 Mesh::GetNormalAtPoint(const glm::vec3 & point) const
{
	return _normal;
}

void Mesh::CreateTriangles(const std::vector<size_t>& indicies)
{
	// create faces/triangles for mesh interection and coloring.
	for (size_t i = 1, last = indicies.size() - 1; i < last; i++) {
		Triangle * t = new Triangle(_vertices[indicies[0]], _vertices[indicies[i]], _vertices[indicies[i + 1]], _normals[indicies[1]], ObjectMaterial);

		_triangles.push_back(t);
	}
}
