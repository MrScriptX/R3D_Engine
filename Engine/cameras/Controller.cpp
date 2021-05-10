#include "Controller.h"


Controller::Controller(std::shared_ptr<Camera> p_camera)
{
	mp_camera = p_camera;
}


Controller::~Controller()
{
}

void Controller::setDeltaTime(const float& delta_time)
{
	m_delta_time = delta_time;
}

void Controller::setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action)
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

void Controller::updateRotation(const double& xpos, const double& ypos)
{
	const float sensibility = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(xpos, ypos);

	mp_camera->setYaw(mp_camera->getYaw() + (mouse_delta.x * sensibility));
	mp_camera->setPitch(mp_camera->getPitch() + (mouse_delta.y * sensibility));
}

void Controller::updatePosition()
{
	const float speed = 2.0f;

	if (m_keyboard_press[GLFW_KEY_W] == true)
	{
		mp_camera->MoveForward(speed);
	}

	if (m_keyboard_press[GLFW_KEY_S] == true)
	{
		mp_camera->MoveBackward(speed);
	}

	if (m_keyboard_press[GLFW_KEY_A] == true)
	{
		mp_camera->MoveLeft(speed);
	}

	if (m_keyboard_press[GLFW_KEY_D] == true)
	{
		mp_camera->MoveRight(speed);
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_SHIFT] == true)
	{
		mp_camera->MoveUp(speed);
	}

	if (m_keyboard_press[GLFW_KEY_LEFT_CONTROL] == true)
	{
		mp_camera->MoveDown(speed);
	}

	mp_camera->UpdatePosition(m_delta_time);
}

uint32_t Controller::getLoadRadius()
{
	return m_load_radius;
}
