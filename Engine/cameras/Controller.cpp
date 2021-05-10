#include "Controller.h"


Controller::Controller(std::shared_ptr<Camera> p_camera)
{
	mp_camera = p_camera;

	// Set default controls
	std::function<void()> default_func = [this]() { mp_camera->MoveForward(2.0f); };
	SetKeyToFunc(GLFW_KEY_W, default_func);

	default_func = [this]() { mp_camera->MoveBackward(2.0f); };
	SetKeyToFunc(GLFW_KEY_S, default_func);

	default_func = [this]() { mp_camera->MoveLeft(2.0f); };
	SetKeyToFunc(GLFW_KEY_A, default_func);

	default_func = [this]() { mp_camera->MoveRight(2.0f); };
	SetKeyToFunc(GLFW_KEY_D, default_func);

	default_func = [this]() { mp_camera->MoveUp(2.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_SHIFT, default_func);

	default_func = [this]() { mp_camera->MoveDown(2.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_CONTROL, default_func);
}


Controller::~Controller()
{
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

void Controller::SetKeyToFunc(const int32_t& key, std::function<void()>& func)
{
	m_actions.push_back({ key, func });
}

void Controller::updateRotation(const double& xpos, const double& ypos)
{
	const float sensibility = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(xpos, ypos);

	mp_camera->setYaw(mp_camera->getYaw() + (mouse_delta.x * sensibility));
	mp_camera->setPitch(mp_camera->getPitch() + (mouse_delta.y * sensibility));
}

void Controller::Update(const float& dt)
{
	for (size_t i = 0; i < m_actions.size(); i++)
	{
		if (m_keyboard_press[m_actions[i].key])
		{
			m_actions[i].func();
		}
	}

	mp_camera->UpdatePosition(dt);
}

uint32_t Controller::getLoadRadius()
{
	return m_load_radius;
}
