#include "Controller.h"


Controller::Controller(std::shared_ptr<Camera> p_camera)
{
	mp_camera = p_camera;

	// Set default controls
	std::function<void()> default_func = [this]() { mp_camera->MoveForward(2.0f); };
	SetKeyToFunc(GLFW_KEY_W, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveBackward(2.0f); };
	SetKeyToFunc(GLFW_KEY_S, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveLeft(2.0f); };
	SetKeyToFunc(GLFW_KEY_A, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveRight(2.0f); };
	SetKeyToFunc(GLFW_KEY_D, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveUp(2.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_SHIFT, default_func, ActionType::R3D_HOLD);

	default_func = [this]() { mp_camera->MoveDown(2.0f); };
	SetKeyToFunc(GLFW_KEY_LEFT_CONTROL, default_func, ActionType::R3D_HOLD);
}


Controller::~Controller()
{
}

void Controller::setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action)
{
	if (action == GLFW_PRESS)
	{
		m_keyboard_press.set(key, true);

		for (size_t i = 0; i < m_onpress_actions.size(); i++)
		{
			m_onpress_actions[i].func();
		}
	}
	else if (action == GLFW_RELEASE)
	{
		m_keyboard_press.set(key, false);

		for (size_t i = 0; i < m_onrelease_actions.size(); i++)
		{
			m_onrelease_actions[i].func();
		}
	}
}

void Controller::SetKeyToFunc(const int32_t& key, std::function<void()>& func, const ActionType& type)
{
	switch (type)
	{
	case ActionType::RED_RELEASE:
		m_onrelease_actions.push_back({ key, func });
		break;
	case ActionType::R3D_PRESS:
		m_onpress_actions.push_back({ key, func });
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
	const float sensibility = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(xpos, ypos);

	mp_camera->setYaw(mp_camera->getYaw() + (mouse_delta.x * sensibility));
	mp_camera->setPitch(mp_camera->getPitch() + (mouse_delta.y * sensibility));
}

void Controller::Update(const float& dt)
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
