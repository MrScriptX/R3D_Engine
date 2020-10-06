#include "Engine.h"

Engine::Engine()
{
	m_last_time = std::chrono::high_resolution_clock::now();

	mp_player = std::make_shared<Player>();
	mp_config = std::make_shared<Config>();

	mp_window = std::make_unique<Window>(mp_config, mp_player->getCamera());

	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), mp_config->width, mp_config->height);


	mp_renderer->createNewPipeline(base_pipeline);
	mp_renderer->createDepthResources();
	mp_renderer->createFramebuffer();

	mp_renderer->createDescriptorPool();
	mp_renderer->allocateCommandBuffers();
}

Engine::~Engine()
{
	mp_renderer->cleanSwapchain(std::make_shared<Pipeline>(base_pipeline));

	for (size_t i = 0; i < mp_scene->getObjects().size(); i++)
	{
		for (size_t t = 0; t < mp_scene->getObjects()[i]->getMeshesCount(); t++)
		{
			mp_scene->getObjects()[i]->getMesh(t).getMaterial()->destroyTexture();
			mp_scene->getObjects()[i]->getMesh(t).destroyMesh();
		}

		mp_scene->getObjects()[i]->destroy();
	}

	mp_scene.reset();

	mp_renderer.reset();
	mp_window.reset();
	mp_config.reset();
}

void Engine::run()
{
	do 
	{
		glfwPollEvents();
		input();
		update();
		mp_renderer->draw(base_pipeline);
	} while (!glfwWindowShouldClose(&mp_window->getHandle()));

	vkDeviceWaitIdle(mp_renderer->getDevice());
}

void Engine::setScene(std::shared_ptr<Scene> p_scene)
{
	mp_scene = p_scene;
}

void Engine::registerGameObject(std::shared_ptr<GameObject> gameobject)
{
	mp_scene->addGameObject(gameobject);
}

std::shared_ptr<Renderer> Engine::getRenderEngine()
{
	return mp_renderer;
}

void Engine::input()
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_last_time).count();

	mp_player->update(delta_time);

	m_last_time = currentTime;
}

void Engine::update()
{
	if (mp_scene->isUpdate())
	{
		for (uint16_t i = 0; i < mp_renderer->getGraphic().command_buffers.size(); i++)
		{
			mp_renderer->beginRecordCommandBuffers(mp_renderer->getGraphic().command_buffers[i], mp_renderer->getGraphic().framebuffers[i], base_pipeline);

			mp_scene->render(base_pipeline, mp_renderer->getGraphic().command_buffers[i]);

			mp_renderer->endRecordCommandBuffers(mp_renderer->getGraphic().command_buffers[i]);
		}
	}

	mp_player->updateUBO(static_cast<float>(mp_config->width), static_cast<float>(mp_config->height));
	mp_scene->updateUBO(mp_player, mp_renderer);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}
