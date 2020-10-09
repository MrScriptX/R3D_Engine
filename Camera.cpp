#include "Camera.h"

#include "Math.h"

Camera::Camera()
{
	m_yaw = .0f;
	m_pitch = .0f;
	last_mouse_pos = glm::vec2(.0f, .0f);

	m_keyboard_press = { false };

	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };

	m_ubo.model = {};
	m_ubo.proj = {};
	m_ubo.view = {};
}


Camera::~Camera()
{
}

void Camera::updateUBO(const float& width, const float& height)
{
	m_ubo.view = updateView(m_pitch, m_yaw, m_position);
	//m_ubo.model = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), m_position);
	m_ubo.model = createModelMatrix(m_rotation, m_position);
	m_ubo.proj = createProjMatrix(width, height);
	m_ubo.proj[1][1] *= -1;
}

void Camera::setPosition(const glm::vec3& position)
{
	m_position = position;
}

const glm::vec3& Camera::getPosition()
{
	return m_position;
}

void Camera::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

const glm::vec3& Camera::getRotation()
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