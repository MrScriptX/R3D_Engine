#include "Player.h"


#include "Math.h"


Player::Player()
{
	m_pCamera = std::make_unique<Camera>();
	m_pUBO = std::make_unique<UniformBufferObject>();
}


Player::~Player()
{
}

void Player::update(float dt)
{
	m_pCamera->setDeltaTime(dt);
	m_pCamera->updatePos();
}

void Player::updateUBO(const float width, const float height)
{
	m_pUBO->view = updateView(m_pCamera->getPitch(), m_pCamera->getYaw(), m_pCamera->getPosition());
	m_pUBO->model = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), m_pCamera->getPosition());
	m_pUBO->proj = createProjMatrix(width, height);
	m_pUBO->proj[1][1] *= -1;
}

UniformBufferObject & Player::getUBO()
{
	return *m_pUBO;
}

uint32_t Player::getLoadRadius()
{
	return m_load_radius;
}

glm::vec3 Player::getPosition()
{
	return m_pCamera->getPosition();
}

float& Player::getPitch()
{
	return m_pCamera->getPitch();
}

float& Player::getYaw()
{
	return m_pCamera->getYaw();
}

Camera & Player::getCamera()
{
	return *m_pCamera.get();
}
