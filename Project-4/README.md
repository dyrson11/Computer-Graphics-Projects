# Project 4: Terrain Rendering

## Synopsis
This project generates a infinite Heightmap using Perlin noise, it has 5 different texures applied depending on 'z' value of a point, it has a skybox for the background, and fog to generate a better effect. Also, it has a dynamic camera.

It has no support for Mac OS yet.

## Usage
To move the position of the camera, use 'A' 'W' 'S' 'D' keys, to rotate the camera use mouse drag

## Libraries
- GLUT
- GLU
- GL
- GLEW


## Code execution
To execute the main code: **Source-Files/main_modern.cpp** on Linux use the following commands
```
$ g++ main_modern.cpp -lGL -lGLU -lGLEW -lglut -lm -o main.out
$ ./main.out
```
if you wanna execute the first version of the code, only mesh: **Source-Files/main.cpp** on Linux use the following commands
```
$ g++ main.cpp -lGL -lGLU -lGLEW -lglut -lm -o main.out
$ ./main.out
```
