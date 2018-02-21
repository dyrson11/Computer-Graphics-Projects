#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in int id;
out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const vec3 colors[3] =  vec3[]( vec3( 0.5, 0.0, 0.0 ),
                                vec3( 0.0, 0.5, 0.0 ),
                                vec3( 0.0, 0.0, 0.5 ) );

void main(void)
{
    gl_Position = projection * view * model * vec4( position, 1.0 );
    vColor = colors[id % 3];
}
