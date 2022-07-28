#version 330 core
// fragment shader

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform float ambientStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ambient (фоновое освещение) 
    vec3 ambient = ambientStrength * lightColor;

    // diffuse (рассеяное освещение)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    // specular (отражённое освещение)
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result  = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0f);
}