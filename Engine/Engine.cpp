#include "Includes/Engine.h"

Engine::Engine(uint32_t width, uint32_t height)
{
	m_last_time = std::chrono::high_resolution_clock::now();

	mp_main_camera = std::make_shared<Camera>();
	mp_controller = std::make_shared<Controller>(mp_main_camera);
	mp_config = std::make_shared<Config>();
	mp_config->width = width;
	mp_config->height = height;

	mp_window = std::make_unique<Window>(mp_config, *mp_controller.get());

	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), mp_config->width, mp_config->height);

	mp_renderer->createDepthResources();
	mp_renderer->createFramebuffer();

	mp_renderer->createDescriptorPool();
	mp_renderer->allocateCommandBuffers();

	Transform transform;
	transform.position = { .0f, .0f, .0f };
	transform.rotation = { .0f, .0f, .0f };
	m_lightobject.SetTransform(transform);

	// create buffer
	mp_renderer->CreateUniformBuffer(m_lightobject_buffer, m_lightobject_memory, sizeof(Transform));
	mp_renderer->allocateDescriptorSet(m_descritorset);
	mp_renderer->updateDescriptorSet(m_lightobject_buffer, m_descritorset);
}

Engine::~Engine()
{
	vkDeviceWaitIdle(mp_renderer->getDevice());

	mp_renderer->cleanSwapchain();

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
	mp_scene->AddGameObject(gameobject);
}

const std::shared_ptr<Material> Engine::CreateMaterial(const TSHADER shader)
{
	return std::make_shared<Material>(shader, mp_renderer);
}

const std::shared_ptr<Material> Engine::CreateMaterial(const TSHADER shader, const std::string& texture_file)
{
	std::shared_ptr<Material> mat = std::make_shared<Material>(shader, mp_renderer);
	mat->LoadTexture(texture_file);

	return mat;
}

const std::shared_ptr<GameObject> Engine::CreateGameObject()
{
	return std::make_shared<GameObject>(mp_renderer);
}

const std::shared_ptr<GameObject> Engine::CreateGameObject(const std::string& object_file)
{
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(mp_renderer);
	go->LoadMesh(object_file);

	return go;
}

const std::shared_ptr<GameObject> Engine::CreateCube(const glm::vec3& position, const float& size, const glm::vec3& vcolor)
{
	std::shared_ptr<GameObject> cube = std::make_shared<GameObject>(mp_renderer);

	Geometry g;

	const float half_size = size / 2;

	// vertices
	g.addVertex({ -half_size, -half_size, -half_size }, vcolor, { .0f, .0f });
	g.addVertex({ half_size, -half_size, -half_size }, vcolor, { .0f, 2.f });
	g.addVertex({ -half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	g.addVertex({ half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	g.addVertex({ -half_size, -half_size, half_size }, vcolor, { .0f, .0f });
	g.addVertex({ half_size, -half_size, half_size }, vcolor, { .0f, 2.f });
	g.addVertex({ -half_size, half_size, half_size }, vcolor, { 2.f, .0f });
	g.addVertex({ half_size, half_size, half_size }, vcolor, { 2.f, 2.f });

	// indices
	g.addIndices(0, 2, 1);
	g.addIndices(1, 2, 3);

	g.addIndices(5, 7, 4);
	g.addIndices(4, 7, 6);

	g.addIndices(1, 3, 5);
	g.addIndices(5, 3, 7);

	g.addIndices(4, 6, 0);
	g.addIndices(0, 6, 2);

	g.addIndices(2, 6, 3);
	g.addIndices(3, 6, 7);

	g.addIndices(4, 0, 5);
	g.addIndices(5, 0, 1);

	cube->LoadMesh(g.vertices, g.indices);
	cube->setPosition(position);

	return cube;
}

void Engine::BindKeyToFunc(const int& key, std::function<void()>& func, const ActionType& type)
{
	mp_controller->SetKeyToFunc(key, func, type);
}

const std::shared_ptr<Camera> Engine::GetMainCamera()
{
	return mp_main_camera;
}

void Engine::SetWireframeMode()
{
	mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_LINE);
}

void Engine::SetPointMode()
{
	mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_POINT);
}

void Engine::SetFillMode()
{
	mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_FILL);
}

void Engine::SetColorMode(const ColorMode color_map)
{
	mp_renderer->SetColorMode(color_map);
}

const bool& Engine::shouldClose()
{
	return glfwWindowShouldClose(&mp_window->getHandle());
}

void Engine::update()
{
	glfwPollEvents();

	std::chrono::time_point<std::chrono::high_resolution_clock> current_time = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - m_last_time).count();

	mp_controller->Update(delta_time);

	m_last_time = current_time;

	const int32_t frame = mp_renderer->AcquireNextImage();
	if (frame == -1)
		return;

	if (mp_scene->isUpdate(frame) || mp_renderer->NeedUpdate(frame))
	{
		mp_renderer->WaitForSwapchainImageFence();

		mp_scene->Update(frame);

		mp_renderer->beginRecordCommandBuffers(mp_renderer->getCommandBuffer(frame), mp_renderer->getFrameBuffer(frame));
		mp_scene->Render(mp_renderer->getCommandBuffer(frame), m_descritorset, frame);
		mp_renderer->endRecordCommandBuffers(mp_renderer->getCommandBuffer(frame));

		mp_scene->Clean(frame);
		mp_renderer->SetUpdated(frame);
	}

	mp_main_camera->UpdateUBO(static_cast<float>(mp_config->width), static_cast<float>(mp_config->height), frame);
	mp_scene->UpdateUBO(mp_main_camera, mp_renderer, frame);

	void* data;
	vkMapMemory(mp_renderer->getDevice(), m_lightobject_memory, 0, sizeof(m_lightobject.GetTransform()), 0, &data);
	memcpy(data, &m_lightobject.GetTransform(), sizeof(m_lightobject.GetTransform()));
	vkUnmapMemory(mp_renderer->getDevice(), m_lightobject_memory);

	// std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Engine::draw()
{
	mp_renderer->draw();
}
