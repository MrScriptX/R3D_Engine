#include "Application.h"

#include <thread>



const std::string MODEL_PATH = "models/viking_room.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";

Application::Application()
{
	m_pPlayer = std::make_shared<Player>();
	m_config = std::make_unique<Config>();
	m_window = std::make_unique<Window>(m_config, m_pPlayer->getCamera());
	m_pRenderer = std::make_shared<Renderer>(m_window->getHandle(), m_config->width, m_config->height);


	m_pRenderer->createNewPipeline(base_pipeline);
	m_pRenderer->createDepthResources();
	m_pRenderer->createFramebuffer();

	m_pRenderer->createTextureImage("textures/viking_room.png");
	m_pRenderer->createTextureImageView();
	m_pRenderer->createTextureSampler();

	//------------------------------------------------------------------------------------------
	room_txt = std::make_unique<Texture>("textures/viking_room.png");
	room_txt->createTextureImage(m_pRenderer);
	room_txt->createTextureImageView(m_pRenderer);
	room_txt->createTextureSampler(m_pRenderer);

	gun_txt = std::make_unique<Texture>("textures/texture.jpg");
	gun_txt->createTextureImage(m_pRenderer);
	gun_txt->createTextureImageView(m_pRenderer);
	gun_txt->createTextureSampler(m_pRenderer);

	gun = std::make_unique<Mesh>("models/ak-47.obj");
	gun->loadModel();
	gun->createBuffer(m_pRenderer);

	room = std::make_unique<Mesh>("models/viking_room.obj");
	room->loadModel();
	room->createBuffer(m_pRenderer);

	//-------------------------------------------------------------------------------

	m_pRenderer->createDescriptorPool();

	m_pRenderer->createUBO();

	m_pRenderer->allocateCommandBuffers();
	
	m_pRenderer->createDescriptorSet();

	//m_pRenderer->allocateDescriptorSet(gun_txt->getDescriptorSet());
	//m_pRenderer->allocateDescriptorSet(room_txt->getDescriptorSet());

	//m_pRenderer->updateDescriptorSet(room_txt->getImageView(), room_txt->getSampler());
}


Application::~Application()
{
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
	for (uint16_t i = 0; i < m_pRenderer->getGraphic().command_buffers.size(); i++)
	{
		m_pRenderer->beginRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i], m_pRenderer->getGraphic().framebuffers[i], base_pipeline);

		//m_pRenderer->recordDrawCommands(m_pRenderer->getGraphic().command_buffers[i], base_pipeline, gun->get_buffer(), gun->get_indices().size());
		//m_pRenderer->recordDrawCommands(m_pRenderer->getGraphic().command_buffers[i], base_pipeline, room->get_buffer(), room->get_indices().size());

		m_pRenderer->updateDescriptorSet(m_pRenderer->getGraphic().descriptor_set, gun_txt->getImageView(), gun_txt->getSampler());
		gun->draw(m_pRenderer->getGraphic().command_buffers[i], base_pipeline, m_pRenderer->getGraphic().descriptor_set);

		m_pRenderer->updateDescriptorSet(m_pRenderer->getGraphic().descriptor_set, room_txt->getImageView(), room_txt->getSampler());
		room->draw(m_pRenderer->getGraphic().command_buffers[i], base_pipeline, m_pRenderer->getGraphic().descriptor_set);

		m_pRenderer->endRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i]);
	}
	
	std::cout << "here" << std::endl;

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


