#version 120  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

attribute vec2 vPosition;
//varying vec3 vColor;
//varying out vec4 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    /*vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0);*/
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition, -1.0,1.0);
    //color = vec4(1,1,1,1);
}
