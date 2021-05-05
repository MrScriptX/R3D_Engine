#ifndef _CAMERA_H
#define _CAMERA_H

#include <memory>
#include <bitset>
#include <iostream>

#include "../graphics/Graphics.h"
#include "../graphics/Vertex.h"

class Camera
{
public:
	Camera();
	~Camera();

	void updateUBO(const float& width, const float& height);

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition();

	void setRotation(const glm::vec3& rotation);
	const glm::vec3& getRotation();

	void setPitch(const float& pitch);
	const float& getPitch();

	void setYaw(const float& yaw);
	const float& getYaw();

	UniformBufferObject& getUBO();

private:

	std::bitset<348> m_keyboard_press;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	float m_yaw;
	float m_pitch;
	glm::vec2 last_mouse_pos;

	UniformBufferObject m_ubo;
};

#endif // !_CAMERA_H