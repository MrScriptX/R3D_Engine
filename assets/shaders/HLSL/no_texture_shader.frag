#version 460
#extension GL_ARB_separate_shader_objects : enable

struct Light {
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
    Light directionals[10];
    Light spots[10];
    Light points[10];
} lights;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;
layout(location = 4) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

vec3 compute_directional_lighting(Light light, vec3 normal, vec3 viewDir);

void main() {
    vec3 color = vec3(0.0, 0.0, 0.0);
    vec3 viewDir = normalize(viewPos - fragPosition);

    for(int i = 0; i < lights.nb_directionallight; i++)
        color += compute_directional_lighting(lights.directionals[i], fragNormal, viewDir);
    
    vec3 result = color * fragColor; 

    outColor = vec4(result, 1.0f);
}

vec3 compute_directional_lighting(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // combine results
    vec3 ambient  = light.ambient * light.color;
    vec3 diffuse  = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    return ambient + diffuse + specular;
}