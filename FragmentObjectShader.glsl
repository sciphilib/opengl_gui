#version 330 core
// fragment shader

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

struct Light
{
    vec3 position;
    // ambient light intensity
    vec3 ambient;
    // the exact color we'd like a light to half 
    vec3 diffuse;
    // usually kept at 1.0f shining at full intensity
    vec3 specular;
};

struct Material 
{
    // the color the surf reflects (= surface's color)
    vec3 ambient;
    // the color of the surf under diff lighting (= surface's color)
    vec3 diffuse;
    // the color of the specular highlight on the surf
    // (or possible even reflect a surface-specific color)
    vec3 specular;
    // the scattering/radius of the specular highlight
    float shininess;
};


uniform Light light;
uniform Material material;
uniform vec3 viewPos;


void main()
{
    // ambient (фоновое освещение) 
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse (рассеяное освещение)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // specular (отражённое освещение)
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result  = (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0f);
}