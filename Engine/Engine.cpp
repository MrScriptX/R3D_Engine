#include "Engine.h"

Engine::Engine()
{
	m_last_time = std::chrono::high_resolution_clock::now();

	mp_main_camera = std::make_shared<Camera>();
	mp_controller = std::make_shared<Controller>(mp_main_camera);
	mp_config = std::make_shared<Config>();

	mp_window = std::make_unique<Window>(mp_config, *mp_controller.get());

	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), mp_config->width, mp_config->height);

	mp_renderer->createDepthResources();
	mp_renderer->createFramebuffer();

	mp_renderer->createDescriptorPool();
	mp_renderer->allocateCommandBuffers();
}

Engine::~Engine()
{
	vkDeviceWaitIdle(mp_renderer->getDevice());

	mp_renderer->cleanSwapchain();

	for (size_t i = 0; i < mp_scene->getObjects().size(); i++)
	{
		for (size_t t = 0; t < mp_scene->getObjects()[i]->getMeshesCount(); t++)
		{
			mp_scene->getObjects()[i]->getMesh(t).getMaterial()->DestroyTexture();
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
	go->loadMesh(object_file);
	return go;
}

const std::shared_ptr<GameObject> Engine::CreateCube(const glm::vec3& position, const float& size, const glm::vec3& vcolor)
{
	std::shared_ptr<GameObject> cube = std::make_shared<GameObject>(mp_renderer);

	Voxel voxel;

	const float half_size = size / 2;

	//vertices
	voxel.addVertex({ -half_size, -half_size, -half_size }, vcolor, { .0f, .0f });
	voxel.addVertex({ half_size, -half_size, -half_size }, vcolor, { .0f, 2.0f });
	voxel.addVertex({ -half_size, half_size, -half_size }, vcolor, { 2.0f, .0f });
	voxel.addVertex({ half_size, half_size, -half_size }, vcolor, { 2.0f, .0f });
	voxel.addVertex({ -half_size, -half_size, half_size }, vcolor, { .0f, .0f });
	voxel.addVertex({ half_size, -half_size, half_size }, vcolor, { .0f, 2.0f });
	voxel.addVertex({ -half_size, half_size, half_size }, vcolor, { 2.0f, .0f });
	voxel.addVertex({ half_size, half_size, half_size }, vcolor, { 2.0f, 2.0f });

	//indices
	voxel.addIndices(0, 2, 1);
	voxel.addIndices(1, 2, 3);

	voxel.addIndices(5, 7, 4);
	voxel.addIndices(4, 7, 6);

	voxel.addIndices(1, 3, 5);
	voxel.addIndices(5, 3, 7);

	voxel.addIndices(4, 6, 0);
	voxel.addIndices(0, 6, 2);

	voxel.addIndices(2, 6, 3);
	voxel.addIndices(3, 6, 7);

	voxel.addIndices(4, 0, 5);
	voxel.addIndices(5, 0, 1);

	cube->LoadMesh(voxel.vertices, voxel.indices);
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

const bool& Engine::shouldClose()
{
	return glfwWindowShouldClose(&mp_window->getHandle());
}

void Engine::update()
{
	glfwPollEvents();

	std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - m_last_time).count();

	mp_controller->Update(delta_time);

	m_last_time = current_time;

	const int32_t frame = mp_renderer->AcquireNextImage();
	if (frame != -1 && (mp_scene->isUpdate(frame) || mp_renderer->IsUpdated(frame)))
	{
		mp_renderer->beginRecordCommandBuffers(mp_renderer->getCommandBuffer(frame), mp_renderer->getFrameBuffer(frame));
		mp_scene->render(mp_renderer->getCommandBuffer(frame), frame);
		mp_renderer->endRecordCommandBuffers(mp_renderer->getCommandBuffer(frame));

		mp_renderer->SetUpdate(frame);
	}

	mp_main_camera->UpdateUBO(static_cast<float>(mp_config->width), static_cast<float>(mp_config->height));
	mp_scene->updateUBO(mp_main_camera, mp_renderer);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Engine::draw()
{
	mp_renderer->draw();
}
