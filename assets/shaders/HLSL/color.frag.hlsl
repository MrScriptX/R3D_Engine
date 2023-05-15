#pragma target 6.0

struct DirectionalLight
{
    float ambient;
    float diffuse;
    float specular;

    float3 color;
    float3 position;
    float3 direction;
};

struct PointLight
{
    float constant;
    float _linear;
    float quadratic;

    float ambient;
    float diffuse;
    float specular;

    float3 color;
    float3 position;
};

struct SpotLight
{
    float ambient;
    float diffuse;
    float specular;

    float3 color;
    float3 position;
    float3 direction;
};

cbuffer Lights: register(b0, space1) {
    uint nb_directionallight;
    uint nb_spotlight;
    uint nb_pointlight;

    DirectionalLight directionals[10];
    SpotLight spots[10];
    PointLight points[10];
};


struct Input {
    float3 fragNormal: NORMAL;
    float3 fragPosition: POSITION;
    float3 fragColor: COLOR;
    float3 viewPos: POSITION;
};

float3 compute_directional_lighting(DirectionalLight light, float3 normal, float3 viewDir);
float3 compute_point_lighting(PointLight light, float3 normal, float3 fragPos, float3 viewDir);

float4 main(Input input): SV_TARGET
{
    float3 color = float3(0.0, 0.0, 0.0);
    float3 view_direction = normalize(input.viewPos - input.fragPosition);

    for(int i = 0; i < nb_directionallight; i++)
        color += compute_directional_lighting(directionals[i], input.fragNormal, view_direction);

    for(int i = 0; i < nb_pointlight; i++)
        color += compute_point_lighting(points[i], input.fragNormal, input.fragPosition, view_direction);
    
    float3 result = color * input.fragColor;
    return float4(result, 1.0);
}

float3 compute_directional_lighting(DirectionalLight light, float3 normal, float3 viewDir)
{
    float3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // combine results
    float3 ambient  = light.ambient * light.color;
    float3 diffuse  = light.diffuse * diff * light.color;
    float3 specular = light.specular * spec * light.color;

    return ambient + diffuse + specular;
}

float3 compute_point_lighting(PointLight light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, -lightDir), 0.0);

    // specular shading
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light._linear * distance + light.quadratic * (distance * distance));

    // combine results
    float3 ambient = light.ambient * light.color;
    float3 diffuse = light.diffuse * diff * light.color;
    float3 specular = light.specular * spec * light.color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
