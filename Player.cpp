#include "Player.h"


Player::Player(std::shared_ptr<Camera> p_camera)
{
	mp_camera = p_camera;
}


Player::~Player()
{
}

void Player::setDeltaTime(const float& delta_time)
{
	m_delta_time = delta_time;
}

void Player::setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action)
{
	if (action == GLFW_PRESS)
	{
		m_keyboard_press.set(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		m_keyboard_press.set(key, false);
	}
}

void Player::updateRotation(const double& xpos, const double& ypos)
{
	const float sensibility = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(xpos, ypos);

	mp_camera->setYaw(mouse_delta.x * sensibility);
	mp_camera->setPitch(mouse_delta.y * sensibility);
}

void Player::updatePosition()
{
	glm::vec3 change = { 0.0f, 0.0f, 0.0f };
	const float speed = 2.0f;

	if (m_keyboard_press[GLFW_KEY_W] == true)
	{
		change.x -= -glm::cos(glm::radians(mp_camera->getRotation().y)) * speed;//rotation still not working
		change.z -= -glm::sin(glm::radians(mp_camera->getRotation().y)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_S] == true)
	{
		change.x += -glm::cos(glm::radians(mp_camera->getRotation().y)) * speed;
		change.z += -glm::sin(glm::radians(mp_camera->getRotation().y)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_A] == true)
	{
		change.x += -glm::cos(glm::radians(mp_camera->getRotation().y + 90.0f)) * speed;
		change.z += -glm::sin(glm::radians(mp_camera->getRotation().y + 90.0f)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_D] == true)
	{
		change.x -= -glm::cos(glm::radians(mp_camera->getRotation().y + 90.0f)) * speed;
		change.z -= -glm::sin(glm::radians(mp_camera->getRotation().y + 90.0f)) * speed;
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_SHIFT] == true)
	{
		change.y += 1.0f * speed;
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_CONTROL] == true)
	{
		change.y -= 1.0f * speed;
	}

	mp_camera->setPosition((change * m_delta_time) + mp_camera->getPosition());
}

uint32_t Player::getLoadRadius()
{
	return m_load_radius;
}
