#ifndef R3DENGINE_LIGHT_PROPERTIES_H_
#define R3DENGINE_LIGHT_PROPERTIES_H_

#include <glm/glm.hpp>

struct DirectionalLight
{
	alignas(4) float ambient_strength;
	alignas(4) float diffuse_strength;
	alignas(4) float specular_strength;
	alignas(16) glm::vec3 color;
	alignas(16) glm::vec3 position;
	alignas(16) glm::vec3 direction;
};

struct SpotLight
{
	alignas(4) float ambient_strength;
	alignas(4) float diffuse_strength;
	alignas(4) float specular_strength;
	alignas(16) glm::vec3 color;
	alignas(16) glm::vec3 position;
	alignas(16) glm::vec3 direction;
};

struct PointLight
{
	alignas(4) float constant;
	alignas(4) float linear;
	alignas(4) float quadratic;

	alignas(4) float ambient_strength;
	alignas(4) float diffuse_strength;
	alignas(4) float specular_strength;

	alignas(16) glm::vec3 color;
	alignas(16) glm::vec3 position;
};

#endif // !R3DENGINE_LIGHT_PROPERTIES_H_
