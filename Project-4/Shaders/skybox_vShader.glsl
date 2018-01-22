#version 120

attribute vec3 vPosition;
attribute vec2 vTexCoord;
varying vec2 TexCoord;
varying vec4 viewSpace;

void main()
{
    TexCoord = vTexCoord;
    viewSpace = gl_ModelViewMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition, 1.0);
}
