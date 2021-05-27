#ifndef _SHADERS_H
#define _SHADERS_H

#include <array>
#include <string>

struct Shaders
{
	std::array<const std::string, 3> fragment_shader_files = {
		"../assets/shaders/frag.spv",
		"../assets/shaders/no_texture_shader.spv",
		"../assets/shaders/texture_shader.spv"
	};

	std::array<const std::string, 2> vertex_shader_files = { 
		"../assets/shaders/vert.spv",
		"../assets/shaders/normalmap.spv"
	};
};

#endif // !_SHADERS_H