#include "../Includes/graphics/Math.h"

glm::mat4 updateView(float& pitch, float& yaw, glm::vec3 position)
{
	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

	glm::quat orientation = qPitch * qYaw;
	orientation = glm::normalize(orientation);
	glm::mat4 rotate = glm::mat4_cast(orientation);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -position);

	return glm::mat4(rotate * translate);
}

glm::mat4 createModelMatrix(glm::vec3 rotation, glm::vec3 position)
{
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::translate(matrix, -position);

	matrix = glm::rotate(matrix, glm::radians(rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(rotation.z), { 0, 0, 1 });

	return matrix;
}

glm::mat4 createProjMatrix(float width, float height)
{
	glm::mat4 matrix;

	matrix = glm::perspective(glm::radians(45.0f), width / height, 0.001f, 1000.0f);

	return matrix;
}