#version 450

layout(location = 1) in vec3 fragPosition;
layout(location = 0) in vec3 fragNormal;
layout(location = 2) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main()
{
    float ambientStrength = 0.100000001490116119384765625;
    vec3 ambient = vec3(1.0) * ambientStrength;
    vec3 light_pos = vec3(0.0);
    vec3 light_dir = normalize(light_pos - fragPosition);
    float diff = max(dot(fragNormal, light_dir), 0.0);
    vec3 diffuse = vec3(1.0) * diff;
    vec3 viewDir = normalize(vec3(0.0) - fragPosition);
    vec3 reflectDir = reflect(-light_dir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256.0);
    vec3 specular = vec3(1.0) * (1.0 * spec);
    vec3 result = ((ambient + diffuse) + specular) * fragColor;
    outColor = vec4(result, 1.0);
}