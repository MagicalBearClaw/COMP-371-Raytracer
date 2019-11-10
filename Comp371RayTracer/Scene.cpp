#include "Scene.h"

Scene::~Scene()
{
	SAFE_DELETE(SceneCamera);

	for (size_t i = 0; i < Lights.size(); i++)
	{
		SAFE_DELETE(Lights[i]);
	}

	for (size_t i = 0; i < Objects.size(); i++)
	{
		SAFE_DELETE(Objects[i]);
	}
}

bool Scene::LoadScene(const std::string & filePath)
{
	std::ifstream fileStream;

	fileStream.open(filePath);

	if (!fileStream.is_open())
		return false;
	std::string currentLine = "";
	std::getline(fileStream, currentLine);
	int objectCount = std::stoi(currentLine);
	if (objectCount == 0)
	{
		fileStream.close();
		return true;
	}

	// figure what type of object we are currently parsing and create that object
	// and add it to the scene.
	while (std::getline(fileStream, currentLine))
	{
		if (currentLine == "")
			continue;
		ObjectType type = GetObjectType(currentLine);
		switch (type)
		{
		case ObjectType::Camera:
			SceneCamera = CreateCamera(fileStream);
			break;
		case ObjectType::Light:
			Lights.push_back(CreateLight(fileStream));
			break;
		case ObjectType::Plane:
			Objects.push_back(CreatePlane(fileStream));
			break;
		case ObjectType::Sphere:
			Objects.push_back(CreateSphere(fileStream));
			break;
		case ObjectType::Mesh:
			Objects.push_back(CreateMesh(fileStream));
			break;
		default:
			return false;
		}
	}


	// calculate camera image plane to shoot rays through.
	Height = 2 * (tan(glm::radians(SceneCamera->FOV) / 2) * SceneCamera->FocalLength);
	Width = SceneCamera->AspectRatio * Height;

	fileStream.close();
	return true;
}




Scene::ObjectType Scene::GetObjectType(const std::string & line) const
{
	if (line == "camera") 
		return ObjectType::Camera;
	if (line == "plane") 
		return ObjectType::Plane;
	if (line == "sphere") 
		return ObjectType::Sphere;
	if (line == "mesh") 
		return ObjectType::Mesh;
	if (line == "light") 
		return ObjectType::Light;
}


Scene::ObjectAttribute Scene::GetObjectAttribute(const std::string & line) const
{
	if (line == "pos")
		return ObjectAttribute::Position;
	if (line == "fov")
		return ObjectAttribute::FOV;
	if (line == "f")
		return ObjectAttribute::FocalLength;
	if (line == "a")
		return ObjectAttribute::AspectRatio;
	if (line == "nor")
		return ObjectAttribute::Normal;
	if (line == "amb")
		return ObjectAttribute::Ambient;
	if (line == "dif")
		return ObjectAttribute::Diffuse;
	if (line == "spe")
		return ObjectAttribute::Specular;
	if (line == "shi")
		return ObjectAttribute::Shininess;
	if (line == "rad")
		return ObjectAttribute::Radius;
	if (line == "file")
		return ObjectAttribute::FileName;
}



Light * Scene::CreateLight(std::ifstream& stream) const
{
	std::string currentLine = "";
	Light * light = new Light();

	size_t numOfAttributes = 3;
	size_t currentAttribute = 0;

	while (currentAttribute < numOfAttributes)
	{
		std::getline(stream, currentLine);
		std::stringstream ss(currentLine);
		std::string attribute = "";
		std::getline(ss, attribute, ':');

		ObjectAttribute attributeType = GetObjectAttribute(attribute);

		switch (attributeType)
		{
		case ObjectAttribute::Position:
			light->Position = ParseVector(currentLine);
			break;
		case ObjectAttribute::Diffuse:
			light->Color = ParseVector(currentLine);
			break;
		case ObjectAttribute::Specular:
			light->SpecularColor = ParseVector(currentLine);
			break;
		default:
			break;
		}
		currentAttribute++;
	}

	return light;
}

Camera * Scene::CreateCamera(std::ifstream& stream) const
{
	std::string currentLine = "";

	Camera * camera = new Camera();

	camera->Direction = glm::vec3(0, 0, -1);
	camera->CameraRight = glm::vec3(1, 0, 0);
	camera->CameraDown = glm::vec3(0, -1, 0);

	size_t numOfAttributes = 4;
	size_t currentAttribute = 0;

	while (currentAttribute < numOfAttributes)
	{
		std::getline(stream, currentLine);
		std::stringstream ss(currentLine);
		std::string attribute = "";
		std::getline(ss, attribute, ':');

		ObjectAttribute attributeType = GetObjectAttribute(attribute);

		switch (attributeType)
		{
		case ObjectAttribute::Position:
			camera->Position = ParseVector(currentLine);
			break;
		case ObjectAttribute::FOV:
			camera->FOV = ParseFloat(currentLine);
			break;
		case ObjectAttribute::FocalLength:
			camera->FocalLength = ParseFloat(currentLine);
			break;
		case ObjectAttribute::AspectRatio:
			camera->AspectRatio = ParseFloat(currentLine);
			break;
		default:
			break;
		}
		currentAttribute++;
	}

	return camera;
}

Plane * Scene::CreatePlane(std::ifstream& stream) const
{
	std::string currentLine = "";
	
	Material * material = new Material();

	size_t numOfAttributes = 6;
	size_t currentAttribute = 0;

	glm::vec3 normal;
	glm::vec3 position;

	while (currentAttribute < numOfAttributes)
	{
		std::getline(stream, currentLine);
		std::stringstream ss(currentLine);
		std::string attribute = "";
		std::getline(ss, attribute, ':');

		ObjectAttribute attributeType = GetObjectAttribute(attribute);

		switch (attributeType)
		{
		case ObjectAttribute::Normal:
			normal = ParseVector(currentLine);
			break;
		case ObjectAttribute::Position:
			position = ParseVector(currentLine);
			break;
		case ObjectAttribute::Ambient:
			material->Ambient = ParseVector(currentLine);
			break;
		case ObjectAttribute::Diffuse:
			material->Diffuse = ParseVector(currentLine);
			break;
		case ObjectAttribute::Specular:
			material->Specular = ParseVector(currentLine);
			break;
		case ObjectAttribute::Shininess:
			material->Shininess = ParseFloat(currentLine);
			break;
		default:
			break;
		}
		currentAttribute++;
	}

	Plane * plane = new Plane(normal, position, material);

	return plane;
}

Sphere * Scene::CreateSphere(std::ifstream& stream) const
{
	std::string currentLine = "";

	Material * material = new Material();

	size_t numOfAttributes = 6;
	size_t currentAttribute = 0;

	float radius;
	glm::vec3 position;

	while (currentAttribute < numOfAttributes)
	{
		std::getline(stream, currentLine);
		std::stringstream ss(currentLine);
		std::string attribute = "";
		std::getline(ss, attribute, ':');

		ObjectAttribute attributeType = GetObjectAttribute(attribute);

		switch (attributeType)
		{
		case ObjectAttribute::Radius:
			radius = ParseFloat(currentLine);
			break;
		case ObjectAttribute::Position:
			position = ParseVector(currentLine);
			break;
		case ObjectAttribute::Ambient:
			material->Ambient = ParseVector(currentLine);
			break;
		case ObjectAttribute::Diffuse:
			material->Diffuse = ParseVector(currentLine);
			break;
		case ObjectAttribute::Specular:
			material->Specular = ParseVector(currentLine);
			break;
		case ObjectAttribute::Shininess:
			material->Shininess = ParseFloat(currentLine);
			break;
		default:
			break;
		}
		currentAttribute++;
	}

	Sphere * sphere = new Sphere(position, radius, material);

	return sphere;
}

Mesh * Scene::CreateMesh(std::ifstream & stream) const
{
	std::string currentLine = "";

	Material * material = new Material();

	size_t numOfAttributes = 5;
	size_t currentAttribute = 0;

	std::string fileName;
	glm::vec3 position;

	while (currentAttribute < numOfAttributes)
	{
		std::getline(stream, currentLine);
		std::stringstream ss(currentLine);
		std::string attribute = "";
		std::getline(ss, attribute, ':');

		ObjectAttribute attributeType = GetObjectAttribute(attribute);

		switch (attributeType)
		{
		case ObjectAttribute::FileName:
			fileName = ParseString(currentLine);
			break;
		case ObjectAttribute::Ambient:
			material->Ambient = ParseVector(currentLine);
			break;
		case ObjectAttribute::Diffuse:
			material->Diffuse = ParseVector(currentLine);
			break;
		case ObjectAttribute::Specular:
			material->Specular = ParseVector(currentLine);
			break;
		case ObjectAttribute::Shininess:
			material->Shininess = ParseFloat(currentLine);
			break;
		default:
			break;
		}
		currentAttribute++;
	}
	Mesh *  mesh = new Mesh(material);
	mesh->LoadMesh("../Content/Meshes/" + fileName);
	return mesh;
}



glm::vec3 Scene::ParseVector(const std::string line) const
{
	std::stringstream stringStream(line);
	std::istream_iterator<std::string> begin(stringStream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> splitValues(begin, end);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	x = std::stof(splitValues[1]);
	y = std::stof(splitValues[2]);
	z = std::stof(splitValues[3]);

	return glm::vec3(x, y, z);
}

float Scene::ParseFloat(const std::string line) const
{
	std::stringstream stringStream(line);
	std::istream_iterator<std::string> begin(stringStream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> stringValues(begin, end);

	return std::stof(stringValues[1]);
}

std::string Scene::ParseString(const std::string line) const
{
	std::stringstream stringStream(line);
	std::istream_iterator<std::string> begin(stringStream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> stringValues(begin, end);

	return stringValues[1];
}
