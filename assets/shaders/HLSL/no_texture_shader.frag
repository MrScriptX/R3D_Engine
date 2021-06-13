#version 460
#extension GL_ARB_separate_shader_objects : enable

struct DirectionalLight {
    float ambient;
    float diffuse;
    float specular;

    vec3 color;
    vec3 position;
    vec3 direction;
};

struct PointLight {
    float constant;
    float linear;
    float quadratic;

    float ambient;
    float diffuse;
    float specular;

    vec3 color;
    vec3 position;
};

struct SpotLight {
    float ambient;
    float diffuse;
    float specular;

    vec3 color;
    vec3 position;
    vec3 direction;
};

layout(std140, set = 1, binding = 0) uniform Lights {
    uint nb_directionallight;
    uint nb_spotlight;
    uint nb_pointlight;

    DirectionalLight directionals[10];
    SpotLight spots[10];
    PointLight points[10];
} lights;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;
layout(location = 4) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

vec3 compute_directional_lighting(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 compute_point_lighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main() {
    vec3 color = vec3(0.0, 0.0, 0.0);
    vec3 viewDir = normalize(viewPos - fragPosition); // bug

    for(int i = 0; i < lights.nb_directionallight; i++)
        color += compute_directional_lighting(lights.directionals[i], fragNormal, viewDir);
    
    for(int i = 0; i < lights.nb_pointlight; i++)
        color += compute_point_lighting(lights.points[i], fragNormal, fragPosition, viewDir);

    vec3 result = color * fragColor; 

    outColor = vec4(result, 1.0f);
}

vec3 compute_directional_lighting(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // combine results
    vec3 ambient  = light.ambient * light.color;
    vec3 diffuse  = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    return ambient + diffuse + specular;
}

vec3 compute_point_lighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, -lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * light.color;
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}