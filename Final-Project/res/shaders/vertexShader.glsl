#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in uint id;
out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const vec3 colors[6] =  vec3[]( vec3( 0.8, 0.0, 0.0 ),
                                vec3( 0.8, 0.8, 0.0 ),
                                vec3( 0.0, 0.8, 0.0 ),
                                vec3( 0.0, 0.8, 0.8 ),
                                vec3( 0.0, 0.0, 0.8 ),
                                vec3( 0.8, 0.0, 0.8 ) );

void main(void)
{
    int res;
    gl_Position = projection * view * model * vec4( position, 1.0 );
    if( id == uint(500000) )
    {
        vColor = vec3( 0.0, 0.0, 0.0 );
        return;
    }
    res = int( mod( float(id), 3.0 ) );
    vColor = colors[ int(id) % 6 ];
}
