#version 450 core

out vec4 color;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;

vec3 lightPos = vec3(-4.0f, 4.0f, 0.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
//vec3 objectColor = vec3(0.87f, 0.74f, 0.62f);
vec3 objectColor = vec3(0.98f, 0.90f, 0.77f);

void main(void)
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.05;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 0.2);
}
