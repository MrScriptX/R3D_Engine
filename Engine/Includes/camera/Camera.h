#ifndef _CAMERA_H
#define _CAMERA_H

#include <memory>
#include <bitset>
#include <iostream>

#include "../graphics/Math.h"
#include "../graphics/Graphics.h"
#include "../graphics/Vertex.h"

class Camera
{
public:
	Camera();
	~Camera();

	void UpdateUBO(const float& width, const float& height, const int32_t frame);
	void UpdatePosition(const float& dt);

	void MoveForward(const float& speed);
	void MoveBackward(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition();

	void SetRotation(const glm::vec3& rotation);
	const glm::vec3& GetRotation();

	void setPitch(const float& pitch);
	const float& getPitch();

	void setYaw(const float& yaw);
	const float& getYaw();

	UniformBufferObject& GetUBO(const int32_t frame);

private:

	std::bitset<348> m_keyboard_press;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_delta_position;

	float m_yaw;
	float m_pitch;
	glm::vec2 last_mouse_pos;

	std::array<UniformBufferObject, MAX_FRAMES_IN_FLIGHT> m_ubo;
};

#endif // !_CAMERA_H