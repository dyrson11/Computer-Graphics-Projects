#version 120

varying vec2 TexCoord;
varying vec4 viewSpace;
uniform sampler2D textureBox;
const vec3 fogColor = vec3(0.1, 0.1, 0.1);
const float FogDensity = 0.01;
float dist = 0;
float fogFactor = 0;
vec4 vTexColor;
vec3 finalColor;

void main()
{
    vTexColor = texture2D(textureBox, TexCoord);

    dist = length(viewSpace);

    fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
    fogFactor = clamp( fogFactor, 0.0, 1.0 );

    finalColor = mix(fogColor, vec3(vTexColor.r, vTexColor.g, vTexColor.b), fogFactor);

    gl_FragColor = vec4(finalColor, 1.0);
}
