#version 460
#extension GL_ARB_separate_shader_objects : enable

struct Transform {
    vec3 position;
    vec3 rotation;
};

layout(std140, set = 1, binding = 0) uniform Lights {
    uint nb_light;
    Transform transform[10];
} lights;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    vec3 light_pos = lights.transform[0].position;

    // diffuse
    vec3 light_dir = normalize(-light_pos - fragPosition);
    float diff = max(dot(fragNormal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(0.0, 0.0, 0.0) - fragPosition);
    vec3 reflectDir = reflect(-light_dir, fragNormal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * fragColor; 

    outColor = vec4(result, 1.0f);
}
