#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

struct Actions
{
	const int32_t key;
	const std::function<void()> func;
};

class Controller
{
public:
	Controller(std::shared_ptr<Camera> p_camera);
	~Controller();

	void setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action);
	void SetKeyToFunc(const int32_t& key, std::function<void()>& func);

	void Update(const float& dt);
	void updateRotation(const double& xpos, const double& ypos);

	uint32_t getLoadRadius();
private:
	std::vector<Actions> m_actions;
	std::bitset<348> m_keyboard_press;

	uint32_t m_load_radius = 4;

	std::shared_ptr<Camera> mp_camera;
};

#endif //!_CONTROLLER_H