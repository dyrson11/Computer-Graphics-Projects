#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 newPos;

void main(void)
{
    int res;
    newPos = position + (aNormal * -0.001);
    FragPos = vec3( model * vec4( newPos, 1.0 ) );

    gl_Position = projection * view * model * vec4( newPos, 1.0 );
    Normal =  mat3( transpose( inverse( model ) ) ) * aNormal;
}
