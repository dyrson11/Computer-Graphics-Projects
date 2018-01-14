/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

#version 120  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

vec4 color = vec4(1,1,1,1);
//out vec4 fragColor;
void main()
{
    gl_FragColor = vec4(1,1,1,1);
}
