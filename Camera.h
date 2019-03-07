#ifndef _CAMERA_H
#define _CAMERA_H


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <bitset>
#include <iostream>

class Camera
{
public:
	Camera();
	~Camera();

	void setInput(int32_t key, int32_t scancode, int32_t mods, int32_t action);
	void mouse(double xpos, double ypos);

	void updatePos();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	float& getPitch();
	float& getYaw();

	void setDeltaTime(float dt);

private:

	std::bitset<348> m_keyboard_press;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	float m_yaw;
	float m_pitch;

	glm::vec2 last_mouse_pos;

	float delta_time;
};

#endif // !_CAMERA_H