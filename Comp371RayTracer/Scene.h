#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "common.h"

/*
	Our scene in which all scene objects, lights and camera exist.
*/
class Scene
{
public:
	~Scene();
	bool LoadScene(const std::string& filePath);

	int Width;
	int Height;
	Camera * SceneCamera;
	std::vector<Light*> Lights;
	std::vector<Object*> Objects;
private:
	enum class ObjectType{
		Camera = 0,
		Plane,
		Sphere,
		Mesh,
		Light
	};

	enum class ObjectAttribute {
		Normal = 0,
		Position,
		Ambient,
		Diffuse,
		Specular,
		Shininess,
		FOV,
		FocalLength,
		AspectRatio,
		Radius,
		FileName
	};

	ObjectType GetObjectType(const std::string& line) const;
	ObjectAttribute GetObjectAttribute(const std::string& line) const;

	Light* CreateLight(std::ifstream& stream) const;
	Camera* CreateCamera(std::ifstream& stream) const;
	Plane * CreatePlane(std::ifstream& stream) const;
	Sphere* CreateSphere(std::ifstream& stream) const;
	Mesh* CreateMesh(std::ifstream& stream) const;

	glm::vec3 ParseVector(const std::string line) const;
	float ParseFloat(const std::string line) const;
	std::string ParseString(const std::string line) const;
};
