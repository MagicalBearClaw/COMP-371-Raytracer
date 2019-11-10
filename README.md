# COMP-371-Raytracer
Concordia university computer graphics project. Build a minimal raytracer.
This ray tracer is multithreaded.

# credits

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




