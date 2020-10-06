#include "Application.h"

#include <thread>



bool draw_room = false;
bool draw_gun = false;

Application::Application()
{
	m_pPlayer = std::make_shared<Player>();
	m_config = std::make_shared<Config>();
	m_window = std::make_unique<Window>(m_config, m_pPlayer->getCamera());
	m_pRenderer = std::make_shared<Renderer>(m_window->getHandle(), m_config->width, m_config->height);


	m_pRenderer->createNewPipeline(base_pipeline);
	m_pRenderer->createDepthResources();
	m_pRenderer->createFramebuffer();

	m_pRenderer->createDescriptorPool();
	m_pRenderer->allocateCommandBuffers();

	//------------------------------------------------------------------------------------------

	gun_texture = std::make_shared<Material>();
	gun_texture->loadTexture("textures/texture.jpg", m_pRenderer);

	gun = std::make_shared<GameObject>(m_pRenderer);
	gun->loadMesh("models/ak-47.obj");
	
	gun->bindMatToMesh(0, gun_texture);
	gun->setPosition(glm::vec3({ 5, 0, 0 }));

	room_texture = std::make_shared<Material>();
	room_texture->loadTexture("textures/viking_room.png", m_pRenderer);

	room = std::make_shared<GameObject>(m_pRenderer);
	room->loadMesh("models/viking_room.obj");

	room->bindMatToMesh(0, room_texture);

	scene.addGameObject(gun);
	scene.addGameObject(room);
}


Application::~Application()
{
	m_pRenderer->cleanSwapchain(std::make_shared<Pipeline>(base_pipeline));

	//m_pRenderer->destroyTextures();
	//m_pRenderer->destroyDescriptors();
	//m_pRenderer->destroyUniformBuffer();

	m_pRenderer.reset();
	m_window.reset();
	m_config.reset();
}

void Application::run()
{
	while (!glfwWindowShouldClose(&m_window->getHandle()))
	{
		glfwPollEvents();
		input();
		update();
		m_pRenderer->draw(base_pipeline);
	}

	vkDeviceWaitIdle(m_pRenderer->getDevice());
}

void Application::update()
{
	if (scene.isUpdate())
	{
		for (uint16_t i = 0; i < m_pRenderer->getGraphic().command_buffers.size(); i++)
		{
			m_pRenderer->beginRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i], m_pRenderer->getGraphic().framebuffers[i], base_pipeline);

			scene.render(base_pipeline, m_pRenderer->getGraphic().command_buffers[i]);

			m_pRenderer->endRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i]);
		}
	}

	m_pPlayer->updateUBO(static_cast<float>(m_config->width), static_cast<float>(m_config->height));
	scene.updateUBO(m_pPlayer, m_pRenderer);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Application::input()
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_last_time).count();

	m_pPlayer->update(delta_time);

	m_last_time = currentTime;
}


