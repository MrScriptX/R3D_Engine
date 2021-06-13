#ifndef R3DENGINE_CAMERA_UBO_H_
#define R3DENGINE_CAMERA_UBO_H_

#include <glm/glm.hpp>

struct UniformBufferObject
{
	alignas(16) glm::vec3 position;
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

#endif // !R3DENGINE_CAMERA_UBO_H_
