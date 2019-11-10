
#include <iostream>

#include "RayTracer.h"

int main()
{	
	// create a raytracer and render a scene with the desired number of worker threads.
	
	/*
		Scenes:
		scene1
		scene2
		scene3
		scene4
		scene5
		mesh_scene1
		mesh_scene2
	*/
	std::string scene = "scene1";
	int numberOfWorkerThreads = 10;
	RayTracer * raytracer = new RayTracer(scene, numberOfWorkerThreads);
	raytracer->Render();

	delete raytracer;
}