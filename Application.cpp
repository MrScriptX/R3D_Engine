#include "Application.h"


//#include "Math.h"


#include <thread>

Application::Application()
{
	m_pPlayer = std::make_shared<Player>();
	m_config = std::make_unique<Config>();
	m_window = std::make_unique<Window>(m_config, m_pPlayer->getCamera());
	m_pRenderer = std::make_shared<Renderer>(m_window->getHandle(), m_config->width, m_config->height);
	m_pChunkManager = std::make_unique<ChunkManager>(m_pRenderer, m_pPlayer);


	m_pRenderer->createNewPipeline(base_pipeline);
	m_pRenderer->createDepthResources();
	m_pRenderer->createFramebuffer();

	m_pRenderer->createTextureImage();
	m_pRenderer->createTextureImageView();
	m_pRenderer->createTextureSampler();

	m_pRenderer->createDescriptorPool();

	m_pChunkManager->createChunks();
	m_pChunkManager->loadChunks();
	m_pRenderer->createUBO();

	m_pRenderer->allocateCommandBuffers();
	
	m_pRenderer->createDescriptorSet();
}


Application::~Application()
{
	m_pChunkManager.reset();

	m_pRenderer->cleanSwapchain(std::make_shared<Pipeline>(base_pipeline));
	m_pRenderer->destroyTextures();
	m_pRenderer->destroyDescriptors();
	m_pRenderer->destroyUniformBuffer();

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

		if (m_pRenderer->draw(base_pipeline) != 0)
		{
			m_pChunkManager->setRebuild(true);
		}
	}

	vkDeviceWaitIdle(m_pRenderer->getDevice());
}

void Application::update()
{
	m_pChunkManager->update();

	if (m_pChunkManager->needRebuild())
	{
		for (uint16_t i = 0; i < m_pRenderer->getGraphic().command_buffers.size(); i++)
		{
			m_pRenderer->beginRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i], m_pRenderer->getGraphic().framebuffers[i], base_pipeline);
			m_pChunkManager->renderChunks(m_pRenderer->getGraphic().command_buffers[i], base_pipeline);
			m_pRenderer->endRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i]);
		}

		m_pChunkManager->setRebuild(false);
	}

	m_pPlayer->updateUBO(static_cast<float>(m_config->width), static_cast<float>(m_config->height));

	void* data;
	vkMapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory, 0, sizeof(m_pPlayer->getUBO()), 0, &data);
	memcpy(data, &m_pPlayer->getUBO(), sizeof(m_pPlayer->getUBO()));
	vkUnmapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Application::input()
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_last_time).count();

	m_pPlayer->update(delta_time);

	m_last_time = currentTime;
}


