#ifndef _PLAYER_H
#define _PLAYER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class Player
{
public:
	Player(std::shared_ptr<Camera> p_camera);
	~Player();

	void setDeltaTime(const float& delta_time);
	void setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action);

	void updateRotation(const double& xpos, const double& ypos);
	void updatePosition();

	uint32_t getLoadRadius();
private:
	std::bitset<348> m_keyboard_press;
	float m_delta_time;

	uint32_t m_load_radius = 4;

	std::shared_ptr<Camera> mp_camera;
};

#endif _PLAYER_H