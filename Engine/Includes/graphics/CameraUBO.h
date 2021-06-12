#ifndef R3DENGINE_CAMERA_UBO_H_
#define R3DENGINE_CAMERA_UBO_H_

#include <glm/glm.hpp>

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 view_inv;
};

#endif // !R3DENGINE_CAMERA_UBO_H_
