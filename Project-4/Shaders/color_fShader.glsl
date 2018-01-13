/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

#version 120  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 color;
varying out vec4 fColor;

void main()
{
    fColor = color;
}
