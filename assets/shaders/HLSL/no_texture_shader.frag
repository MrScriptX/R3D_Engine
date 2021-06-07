#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform Light {
    vec3 position;
    vec3 rotation;
} light;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    vec3 light_pos = light.position;
    vec3 light_dir = normalize(light_pos - fragPosition);

    float diff = max(dot(fragNormal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 viewDir = normalize(vec3(0.0, 0.0, 0.0) - fragPosition);
    vec3 reflectDir = reflect(-light_dir, fragNormal); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = 1 * spec * vec3(1.0, 1.0, 1.0);

    vec3 result = (ambient + diffuse + specular) * fragColor; 

    outColor = vec4(result, 1.0f);
}
