#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    vec3 light_pos = vec3(0.0, 0.0, -2.0);
    vec3 light_dir = normalize(light_pos - fragPosition);

    float diff = max(dot(fragNormal, light_dir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 result = (ambient + diffuse) * fragColor;

    outColor = vec4(result, 1.0f);
}
