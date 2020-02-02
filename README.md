# Learn-OpenGL
---
I created this project to teach myself OpenGL and graphics programming.

## How to build

This is a CMAKE project. Make sure at least CMAKE 3.0.0 or newer is installed on your system first.  

To generate a build file:
```
cmake -G '<build options>'
```
Substitute `<build options>` with your system specific build options. On Linux or MacOS, this is `Unix Makefiles`, and don't forget to define `CMAKE_BUILD_TYPE` (eg. `-DCMAKE_BUILD_TYPE=Debug` for debug mode) if using this build option; on Windows platforms you can build with visual studio solutions, e.g. `Visual Studio 15 2017`.

The compiled executable is called `2` (bad naming I know), but you can change it to whatever you want in the `CMakeLists.txt` file.

## Current Progress

* Core render loop
* Camera mechanics
* Single source Prong lighting
* Shader loading function
* Texture loading procedure

## TODO:

* Prong lighting with materials
* Multisource Prong lighting
* Model loading system
* Animation system

## Further Plans:

* Implement a 3D math library/engine from scratch
* Implement a physics library/engine based on aforementioned math library/engine.
* Integrate the renderer with the above libraries/engines.