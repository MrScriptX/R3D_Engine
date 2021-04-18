#include "Engine.h"

Engine::Engine()
{
	m_last_time = std::chrono::high_resolution_clock::now();

	mp_camera = std::make_shared<Camera>();
	mp_player = std::make_shared<Player>(mp_camera);
	mp_config = std::make_shared<Config>();

	mp_window = std::make_unique<Window>(mp_config, *mp_player.get());

	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), mp_config->width, mp_config->height);


	mp_renderer->createNewPipeline(base_pipeline);
	mp_renderer->createDepthResources();
	mp_renderer->createFramebuffer();

	mp_renderer->createDescriptorPool();
	mp_renderer->allocateCommandBuffers();
}

Engine::~Engine()
{
	vkDeviceWaitIdle(mp_renderer->getDevice());

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

	vkDestroyDescriptorPool(mp_renderer->getDevice(), mp_renderer->getDescriptorPool(), nullptr);
	vkDestroyDescriptorSetLayout(mp_renderer->getDevice(), mp_renderer->getDescriptorSetLayout(), nullptr);

	mp_renderer.reset();
	mp_window.reset();
	mp_config.reset();
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

const bool& Engine::shouldClose()
{
	return glfwWindowShouldClose(&mp_window->getHandle());
}

void Engine::update()
{
	glfwPollEvents();

	std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - m_last_time).count();

	mp_player->setDeltaTime(delta_time);
	mp_player->updatePosition();

	m_last_time = current_time;

	const int frame = mp_renderer->getFrameIndex();
	if (mp_scene->isUpdate(frame))
	{
		mp_renderer->beginRecordCommandBuffers(mp_renderer->getCommandBuffer(frame), mp_renderer->getFrameBuffer(frame), base_pipeline);

		mp_scene->render(base_pipeline, mp_renderer->getCommandBuffer(frame), frame);

		mp_renderer->endRecordCommandBuffers(mp_renderer->getCommandBuffer(frame));
	}

	mp_camera->updateUBO(static_cast<float>(mp_config->width), static_cast<float>(mp_config->height));
	mp_scene->updateUBO(mp_camera, mp_renderer);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Engine::draw()
{
	mp_renderer->draw(base_pipeline);
}
