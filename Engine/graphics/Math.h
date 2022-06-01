#ifndef R3DENGINE_MATH_H_
#define R3DENGINE_MATH_H_

#ifndef GLM_FORCE_RADIANS

#define GLM_FORCE_RADIANS

#endif // !GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 updateView(float& pitch, float& yaw, glm::vec3 position);
glm::mat4 createModelMatrix(glm::vec3 rotation, glm::vec3 position);
glm::mat4 createProjMatrix(float width, float height);

#endif // !R3DENGINE_MATH_H_
