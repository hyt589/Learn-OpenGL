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