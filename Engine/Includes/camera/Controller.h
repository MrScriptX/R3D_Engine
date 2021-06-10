#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"

struct Actions
{
	const int32_t key;
	const std::function<void()> func;
};

enum class ActionType
{
	RED_RELEASE,
	R3D_PRESS,
	R3D_HOLD
};

class Controller
{
  public:
	Controller(std::shared_ptr<Camera> p_camera);
	~Controller();

	void setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action);
	void SetKeyToFunc(const int32_t& key, std::function<void()>& func, const ActionType& type);

	void Update(const float& dt);
	void updateRotation(const double& xpos, const double& ypos);

	uint32_t getLoadRadius();

  private:
	std::vector<Actions> m_hold_actions;
	std::unique_ptr<std::array<std::function<void()>, 348>> m_onpress_actions;
	std::unique_ptr<std::array<std::function<void()>, 348>> m_onrelease_actions;

	uint32_t m_load_radius = 4;
	std::bitset<348> m_keyboard_press;

	std::shared_ptr<Camera> mp_camera;
};

#endif //!_CONTROLLER_H
