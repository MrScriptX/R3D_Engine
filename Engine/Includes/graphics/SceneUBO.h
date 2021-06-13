#ifndef R3DENGINE_SCENE_UBO_H_
#define R3DENGINE_SCENE_UBO_H_

#include "LightProperties.h"

constexpr size_t MAX_LIGHT = 10;

struct SceneUBO
{
	alignas(4) unsigned int nb_directional = 0;
	alignas(4) unsigned int nb_spotlight = 0;
	alignas(4) unsigned int nb_pointlight = 0;
	DirectionalLight directionals[MAX_LIGHT];
	SpotLight spots[MAX_LIGHT];
	PointLight points[MAX_LIGHT];
};

#endif // !R3DENGINE_SCENE_UBO_H_
