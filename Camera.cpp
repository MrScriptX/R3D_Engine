#include "Camera.h"



Camera::Camera()
{
	m_yaw = .0f;
	m_pitch = .0f;
	last_mouse_pos = glm::vec2(.0f, .0f);
	delta_time = .0f;

	m_keyboard_press = { false };

	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
}


Camera::~Camera()
{
}

void Camera::setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action)
{
	if (action == GLFW_PRESS)
	{
		m_keyboard_press.set(key, true);
	}
	else if(action == GLFW_RELEASE)
	{
		m_keyboard_press.set(key, false);
	}
}

void Camera::mouse(double xpos, double ypos)
{
	float sensibility = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(xpos, ypos);
	
	m_yaw = mouse_delta.x * sensibility;
	m_pitch = mouse_delta.y * sensibility;
}

void Camera::updatePos()
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };
	float speed = 10.0f;

	if (m_keyboard_press[GLFW_KEY_W] == true)
	{
		change.x -= -glm::cos(glm::radians(m_rotation.y)) * speed;//rotation still not working
		change.z -= -glm::sin(glm::radians(m_rotation.y)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_S] == true)
	{
		change.x += -glm::cos(glm::radians(m_rotation.y)) * speed;
		change.z += -glm::sin(glm::radians(m_rotation.y)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_A] == true)
	{
		change.x += -glm::cos(glm::radians(m_rotation.y + 90.0f)) * speed;
		change.z += -glm::sin(glm::radians(m_rotation.y + 90.0f)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_D] == true)
	{
		change.x -= -glm::cos(glm::radians(m_rotation.y + 90.0f)) * speed;
		change.z -= -glm::sin(glm::radians(m_rotation.y + 90.0f)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_SHIFT] == true)
	{
		change.y += 1.0f * speed;
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_CONTROL] == true)
	{
		change.y -= 1.0f * speed;
	}

	m_position += change * delta_time;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

glm::vec3 Camera::getRotation()
{
	return m_rotation;
}

float & Camera::getPitch()
{
	return m_pitch;
}

float & Camera::getYaw()
{
	return m_yaw;
}

void Camera::setDeltaTime(float dt)
{
	delta_time = dt;
}
