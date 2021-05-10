#include "Camera.h"

#include "../graphics/Math.h"

Camera::Camera()
{
	m_yaw = .0f;
	m_pitch = .0f;
	last_mouse_pos = glm::vec2(.0f, .0f);

	m_keyboard_press = { false };

	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
	m_delta_position = { 0.f, 0.f, 0.f };

	m_ubo.model = {};
	m_ubo.proj = {};
	m_ubo.view = {};
}


Camera::~Camera()
{
}

void Camera::UpdateUBO(const float& width, const float& height)
{
	m_ubo.view = updateView(m_pitch, m_yaw, m_position);
	m_ubo.proj = createProjMatrix(width, height);
	m_ubo.proj[1][1] *= -1;
}

void Camera::UpdatePosition(const float& dt)
{
	m_position += m_delta_position * dt;

	m_delta_position = glm::vec3{ 0.f, 0.f, 0.f };
}

void Camera::MoveForward(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.x -= -glm::sin(m_yaw) * speed;
	change.z -= glm::cos(m_yaw) * speed;

	m_delta_position += change;
}

void Camera::MoveBackward(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.x += -glm::sin(m_yaw) * speed;
	change.z += glm::cos(m_yaw) * speed;

	m_delta_position += change;
}

void Camera::MoveRight(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.x -= -glm::sin(m_yaw + glm::radians(90.0f)) * speed;
	change.z -= glm::cos(m_yaw + glm::radians(90.0f)) * speed;

	m_delta_position += change;
}

void Camera::MoveLeft(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.x += -glm::sin(m_yaw + glm::radians(90.0f)) * speed;
	change.z += glm::cos(m_yaw + glm::radians(90.0f)) * speed;

	m_delta_position += change;
}

void Camera::MoveUp(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.y += 1.0f * speed;

	m_delta_position += change;
}

void Camera::MoveDown(const float& speed)
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };

	change.y -= 1.0f * speed;

	m_delta_position += change;
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

const glm::vec3& Camera::GetPosition()
{
	return m_position;
}

void Camera::SetRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

const glm::vec3& Camera::GetRotation()
{
	return m_rotation;
}

void Camera::setPitch(const float& pitch)
{
	m_pitch = pitch;
}

const float & Camera::getPitch()
{
	return m_pitch;
}

void Camera::setYaw(const float& yaw)
{
	m_yaw = yaw;
}

const float & Camera::getYaw()
{
	return m_yaw;
}

UniformBufferObject& Camera::getUBO()
{
	return m_ubo;
}