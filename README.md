# COMP-371-Raytracer
Concordia university computer graphics project. Build a minimal raytracer.
This ray tracer is multithreaded.

# Credits

Model/Mesh loading is implemented by using tiny obj loader

https://github.com/syoyo/tinyobjloader

Math lib used is GLM

https://glm.g-truc.net/0.9.9/index.html

Image Displaying and saving of pixel buffer is implemented by using cimg

http://cimg.eu/

# Configuration

Must be used in x86(either debug or release) in order to build and run the project.
release will optimize the ray tracing time by a lot.
on an i7 8700k in release mode, with 10 worker threads, the render time should be sub 1 second.

# Running
in main.cpp you can change the scene and the number of worker threads to raytrace the selected scene.


# Sample Output


![Scene 1](Results/Output/scene1.bmp?raw=true "Scene 1")
![Scene 2](Results/Output/scene2.bmp?raw=true "Scene 2")
![Scene 3](Results/Output/scene3.bmp?raw=true "Scene 3")
![Scene 4](Results/Output/scene4.bmp?raw=true "Scene 4")
![Scene 5](Results/Output/scene5.bmp?raw=true "Scene 5")
![Scene 6](Results/Output/mesh_scene1.bmp?raw=true "Scene 6")
![Scene 7](Results/Output/mesh_scene2.bmp?raw=true "Scene 7")
