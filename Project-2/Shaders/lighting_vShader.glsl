#version 120

in  vec3 vPosition;
in  vec3 vColor;
in  vec3 vNormal;
//varying out vec4 color;
varying out vec3 Normal;
varying out vec3 FragPos;

uniform mat4 model;
uniform mat4 imodel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vPosition4 = vec4(vPosition, 1.0);
    FragPos = -vec3(view * model * vPosition4);
    Normal = mat3(transpose(imodel)) * vNormal;
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
}
