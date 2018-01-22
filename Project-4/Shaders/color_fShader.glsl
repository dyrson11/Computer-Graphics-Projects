#version 120

varying vec2 TexCoord;
varying float height;
varying vec4 viewSpace;
vec4 vTexColor;
vec3 finalColor;
uniform sampler2D texture1, texture2, texture3, texture4, texture5;
const vec3 fogColor = vec3(0.1, 0.1, 0.1);
const float FogDensity = 0.05;
float dist = 0;
float fogFactor = 0;


void main()
{
    float scale = height;
    vec4 vTexColor = vec4(0.0);
    if(height < 0.0)
    {
        scale /= -0.3;
        float scale2 = 1 - scale;
        vTexColor += texture2D(texture1, TexCoord)*scale2;
        vTexColor += texture2D(texture2, TexCoord)*scale;
    }
    else if(height < 0.2)
    {
        scale /= (0.2);
        float scale2 = 1 - scale;
        vTexColor += texture2D(texture2, TexCoord)*scale2;
        vTexColor += texture2D(texture3, TexCoord)*scale;
    }
    else if(height < 0.9)
    {
        scale -= 0.2;
        scale /= (0.4);
        float scale2 = 1 - scale;
        vTexColor += texture2D(texture3, TexCoord)*scale2;
        vTexColor += texture2D(texture4, TexCoord)*scale;
    }
    else if(height < 1.1)
    {
        scale -= 0.9;
        scale /= (0.2);
        float scale2 = 1 - scale;
        vTexColor += texture2D(texture4, TexCoord)*scale2;
        vTexColor += texture2D(texture5, TexCoord)*scale;
    }
    else
        vTexColor = texture2D(texture5, TexCoord);

    dist = length(viewSpace);

    fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
    fogFactor = clamp( fogFactor, 0.0, 1.0 );

    finalColor = mix(fogColor, vec3(vTexColor.r, vTexColor.g, vTexColor.b), fogFactor);

    gl_FragColor = vec4(finalColor, 1.0 / (dist * 2));
}
