#include "Controller.h"

Controller::Controller(std::shared_ptr<Camera> p_camera) : m_mouse_lock(true)
{
	mp_camera = p_camera;

	// Set default controls
	std::function<void()> default_func = [this]() { mp_camera->MoveForward(4.0f); };
	SetKeyToFunc(GLFW_KEY_W, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveBackward(4.0f); };
	SetKeyToFunc(GLFW_KEY_S, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveLeft(4.0f); };
	SetKeyToFunc(GLFW_KEY_A, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveRight(4.0f); };
	SetKeyToFunc(GLFW_KEY_D, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveUp(4.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_SHIFT, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveDown(4.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_CONTROL, default_func, ActionType::R3D_HOLD);

	m_onpress_actions = std::make_unique<std::array<std::function<void()>, 348>>();
	m_onrelease_actions = std::make_unique<std::array<std::function<void()>, 348>>();
}

void Controller::setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action)
{
	if (action == GLFW_PRESS)
	{
		m_keyboard_press.set(key, true);

		if (m_onpress_actions->at(key))
			m_onpress_actions->at(key)();
	}
	else if (action == GLFW_RELEASE)
	{
		m_keyboard_press.set(key, false);

		if (m_onrelease_actions->at(key))
			m_onrelease_actions->at(key)();
	}
}

void Controller::SetKeyToFunc(const int32_t& key, std::function<void()>& func, const ActionType& type)
{
	switch (type)
	{
	case ActionType::RED_RELEASE:
		m_onrelease_actions->at(key) = func;
		break;
	case ActionType::R3D_PRESS:
		m_onpress_actions->at(key) = func;
		break;
	case ActionType::R3D_HOLD:
		m_hold_actions.push_back({ key, func });
		break;
	default:
		m_hold_actions.push_back({ key, func });
		break;
	}
}

void Controller::updateRotation(const double& xpos, const double& ypos)
{
	const float sensibility = 0.003f;

	glm::vec2 mouse_delta(std::abs(xpos) - m_last_mouse_pos_x, std::abs(ypos) - m_last_mouse_pos_y);

	mp_camera->setYaw(xpos * sensibility);
	mp_camera->setPitch(ypos * sensibility);

	m_last_mouse_pos_x = std::abs(xpos);
	m_last_mouse_pos_y = std::abs(ypos);
}

void Controller::Update(const float& dt) const
{
	for (size_t i = 0; i < m_hold_actions.size(); i++)
	{
		if (m_keyboard_press[m_hold_actions[i].key])
		{
			m_hold_actions[i].func();
		}
	}

	mp_camera->UpdatePosition(dt);
}

uint32_t Controller::getLoadRadius()
{
	return m_load_radius;
}

bool Controller::IsMouseLock() const
{
	return m_mouse_lock;
}

void Controller::SetMouseState(const bool lock)
{
	m_mouse_lock = lock;
}

double Controller::GetLastMousePosX() const
{
	return m_last_mouse_pos_x;
}

double Controller::GetLastMousePosY() const
{
	return m_last_mouse_pos_y;
}
