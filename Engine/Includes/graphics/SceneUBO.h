#ifndef R3DENGINE_SCENE_UBO_H_
#define R3DENGINE_SCENE_UBO_H_

#include <glm/glm.hpp>

constexpr size_t MAX_LIGHT = 10;

struct LightProperties
{
	alignas(4) float ambient_strength;
	alignas(4) float diffuse_strength;
	alignas(4) float specular_strength;
	alignas(16) glm::vec3 color;
	alignas(16) glm::vec3 position;
	alignas(16) glm::vec3 direction;
};

struct SceneUBO
{
	alignas(4) unsigned int nb_directional = 0;
	alignas(4) unsigned int nb_spotlight = 0;
	alignas(4) unsigned int nb_pointlight = 0;
	LightProperties directionals[MAX_LIGHT];
	LightProperties spots[MAX_LIGHT];
	LightProperties points[MAX_LIGHT];
};

#endif // !R3DENGINE_SCENE_UBO_H_
