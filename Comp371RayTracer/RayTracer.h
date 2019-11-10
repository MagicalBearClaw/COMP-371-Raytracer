#pragma once

#include <thread>
#include <functional>
#include <cimage/CImg.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "Scene.h"
#include "common.h"

class RayTracer
{
public:
	RayTracer(const std::string& sceneFilePath, size_t numberOfWorkers = 4);
	~RayTracer();
	void Render();

private:
	void PartialRender(const int viewPortWidthStart, const int viewPortWidthEnd, const int viewPortHeightStart, const int viewPortHeightEnd);
	glm::vec3 CalculatePixelColor(const glm::vec3& intersectionPoint, const glm::vec3& intersectionDirection, int intersectedObjectIndex, Ray& cameraRay);
	void CalculateCameraRayMap();

	Scene * _scene;
	Camera * _camera;
	std::thread* _workers;
	std::vector<glm::vec3> _rayMap;
	glm::vec3 * _pixelMap;

	size_t _numberOfWorkers;
	const float _episilon = 0.00001f;
	float _aspectRatio;
	float _fov;
	std::string _sceneName;
};
