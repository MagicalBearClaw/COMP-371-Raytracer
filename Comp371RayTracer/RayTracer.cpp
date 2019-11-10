#include "RayTracer.h"

RayTracer::RayTracer(const std::string& sceneName, size_t numberOfWorkers) : _numberOfWorkers(numberOfWorkers)
{

	_sceneName = sceneName;

	// set the number of threads to use for rendering.
	if (numberOfWorkers < 1)
		_workers = new std::thread[1];
	else
		_workers = new std::thread[numberOfWorkers];

	_scene = new Scene();

	std::stringstream ss;
	ss << "../Content/Scenes/" << sceneName << ".txt";
	_scene->LoadScene(ss.str());
	_camera = _scene->SceneCamera;
	_aspectRatio = _camera->AspectRatio;
	_fov = glm::radians(_camera->FOV);

	CalculateCameraRayMap();

}

RayTracer::~RayTracer()
{
	SAFE_DELETE_ARRAY(_pixelMap);
	SAFE_DELETE(_scene);
}

void RayTracer::Render()
{
	int renderWindowWith = _scene->Width;
	int renderWindowHeight = _scene->Height;
	Camera* camera = _scene->SceneCamera;

	// create image
	int numPixels = renderWindowWith * renderWindowHeight;
	_pixelMap = new glm::vec3[numPixels];

	// create our worker threads.
	for (size_t i = 0; i < _numberOfWorkers; i++)
	{
		const int viewPortWidthStart = i * (renderWindowWith / _numberOfWorkers);
		const int viewPortWidthEnd = (i + 1)*(renderWindowWith / _numberOfWorkers);

		_workers[i] = std::thread(&RayTracer::PartialRender, this, viewPortWidthStart, viewPortWidthEnd, 0, renderWindowHeight);
	}

	for (size_t i = 0; i < _numberOfWorkers; i++)
	{
		_workers[i].join();
	}

	// assign our image to cimg
	cimg_library::CImg<float> image(renderWindowWith, renderWindowHeight, 1, 3, 0);
	for (int x = 0; x < renderWindowWith; x++)
	{
		for (int y = 0; y < renderWindowHeight; y++)
		{
			int index = y * renderWindowWith + x;
			float red = _pixelMap[index].x * 255;
			float green = _pixelMap[index].y * 255;
			float blue = _pixelMap[index].z * 255;

			// flip the y to not render upside down.
			int normalizedHeight = renderWindowHeight - y - 1;

			image(x, normalizedHeight, 0, 0) = red;
			image(x, normalizedHeight, 0, 1) = green;
			image(x, normalizedHeight, 0, 2) = blue;
		}
	}

	std::stringstream ss;
	ss << "../Results/Output/" << _sceneName << ".bmp";
	std::string renderSavePath = ss.str();

	image.save(renderSavePath.c_str());

	cimg_library::CImgDisplay main_disp(image, "RayTracer");
	while (!main_disp.is_closed())
	{
		main_disp.wait();
	}
}

void RayTracer::PartialRender(const int viewPortWidthStart, const int viewPortWidthEnd, const int viewPortHeightStart, const int viewPortHeightEnd)
{
	glm::vec3 cameraOrigin = _camera->Position;

	// for every gnerate camera ray shoot it in the scene, finding intersecting objects and
	// calculate the interesected object's pixels color by performing phong illumination.
	for (size_t rayMapIndex = 0; rayMapIndex < _rayMap.size(); rayMapIndex++)
	{
		glm::vec3 pixelColor = glm::vec3(0.0f);

		glm::vec3 cameraRayDirection = _rayMap[rayMapIndex];
		Ray cameraRay = Ray(cameraOrigin, cameraRayDirection);

		int closetsIntersectedObjectIndex = -1;
		float minimumIntersectionRayLength = INFINITY;

		// for every object check with ray from the camera intersected with an object.
		// find the object with the smallest ray length and keep track of that length and object index.
		for (unsigned int objectIndex = 0; objectIndex < _scene->Objects.size(); objectIndex++)
		{
			float rayLength = _scene->Objects[objectIndex]->Intersect(cameraRay);
			if (rayLength < minimumIntersectionRayLength && rayLength > _episilon)
			{
				minimumIntersectionRayLength = rayLength;
				closetsIntersectedObjectIndex = objectIndex;
			}
		}

		// we found the closets obejct that has intesected the camera ray, proceeed to calculate the pixel colr by
		// performing phong illumination.
		if (closetsIntersectedObjectIndex >= 0)
		{
			glm::vec3 intersectionPoint = cameraOrigin + (cameraRayDirection * minimumIntersectionRayLength);
			glm::vec3 calculatedPixelColor = CalculatePixelColor(intersectionPoint, cameraRayDirection, closetsIntersectedObjectIndex, cameraRay);
			pixelColor = calculatedPixelColor;
		}

		// no intersected color, use default ambient color.
		_pixelMap[rayMapIndex] = pixelColor;
	}
}

glm::vec3 RayTracer::CalculatePixelColor(const glm::vec3& intersectionPoint, const glm::vec3& intersectionDirection, int intersectedObjectIndex, Ray& cameraRay)
{
	const float shadowBias = 0.00311111f;
	Material * objectMaterial = _scene->Objects[intersectedObjectIndex]->ObjectMaterial;
	glm::vec3 objectNormal = _scene->Objects[intersectedObjectIndex]->GetNormalAtPoint(intersectionPoint);

	glm::vec3 finalPixelColor = glm::vec3(0.0f);

	// for every light create a shadow ray and shoot it from the interesected objects point to the light.
	// to determine if the object is obstructed or not. if it is not then calculate the object's pixels color
	// based on the phong illumination model. otherwise the object is in shadow. so use 0 as the object color.
	for (auto light : _scene->Lights)
	{
		glm::vec3 lightDirection = glm::normalize(light->Position - intersectionPoint);
		float dotNL = glm::dot(objectNormal, lightDirection);

		if (dotNL > 0)
		{
			bool isShadowed = false;

			float objectLightDistance = glm::length(light->Position - intersectionPoint);
			glm::vec3 shadowRayOrigin = intersectionPoint + lightDirection * shadowBias;

			Ray shadowRay = Ray(shadowRayOrigin, lightDirection);

			// cast a shadow ray from the point of intersection of the camera ray and the object to each light source.
			// keeping the closets object index.
			float minimumObjectLightIntersectionDistance = INFINITY;
			int closetsIntersectedObjectIndex = -1;
			for (unsigned int objectIndex = 0; objectIndex < _scene->Objects.size(); objectIndex++)
			{
				float rayLength = _scene->Objects[objectIndex]->Intersect(shadowRay);
				if (rayLength < minimumObjectLightIntersectionDistance && rayLength > _episilon)
				{
					minimumObjectLightIntersectionDistance = rayLength;
					closetsIntersectedObjectIndex = objectIndex;
				}
			}

			if (closetsIntersectedObjectIndex >= 0)
			{
				// check if we are in shadow or not.
				if (minimumObjectLightIntersectionDistance <= objectLightDistance)
				{
					isShadowed = true;
				}
			}
			// if we are in not in shadow that calculate diffuse and specular.
			if (!isShadowed)
			{
				glm::vec3 colorContribution = glm::vec3(0, 0, 0);
				glm::vec3 diffuse = objectMaterial->Diffuse * std::max(0.0f, dotNL);

				glm::vec3 reflectionVector = glm::normalize(2 * glm::dot(lightDirection, objectNormal) * objectNormal - lightDirection);
				float dotRL = glm::dot(reflectionVector, -intersectionDirection);

				// add diffuse and specular
				if (dotRL > _episilon) {
					glm::vec3 specular = objectMaterial->Specular * std::max(0.0f, pow(dotRL, objectMaterial->Shininess));
					colorContribution += light->Color * (diffuse + specular);
				}
				else 
				{
					// dot product < 0 only add diffuse.
					colorContribution += light->Color * diffuse;
				}
				finalPixelColor += colorContribution;
			}
		}
	}
	// add the objects ambient color to the final color.
	finalPixelColor += (objectMaterial->Ambient);

	// clamp the color to 0.0 -1.0 range.
	return glm::clamp(finalPixelColor, 0.0f, 1.0f);
}

// pre-compute the camera rays and push them to a vector.
void RayTracer::CalculateCameraRayMap()
{
	// for each pixel caculate a ray direction to send out.
	glm::vec3 ImageCenter = _camera->Position + _camera->Direction * _camera->FocalLength;
	glm::vec3 bottomLeft = ImageCenter -glm::vec3(_scene->Width / 2.0f + 0.5f, _scene->Height / 2.0f + 0.5f, 0.0f);

	for (int row = 0; row < _scene->Height; row++) {
		for (int col = 0; col < _scene->Width; col++) {
			_rayMap.emplace_back(
				glm::normalize(glm::vec3(bottomLeft.x + col, bottomLeft.y + row, bottomLeft.z) - _camera->Position)
			);
		}
	}
}

