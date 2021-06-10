#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 result = fragColor; 

    outColor = vec4(result, 1.0f);
}