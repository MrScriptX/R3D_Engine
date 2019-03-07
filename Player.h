#ifndef _PLAYER_H
#define _PLAYER_H

#include "Camera.h"
#include "Vertex.h"

class Player
{
public:
	Player();
	~Player();

	void update(float dt);
	void updateUBO(const float width, const float height);

	UniformBufferObject& getUBO();
	uint32_t getLoadRadius();
	glm::vec3 getPosition();
	float& getPitch();
	float& getYaw();

	Camera& getCamera();
private:

	uint32_t m_load_radius = 4;

	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<UniformBufferObject> m_pUBO;
};

#endif _PLAYER_H