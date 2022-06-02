#include "Engine.h"

Engine::Engine(uint32_t width, uint32_t height) : m_last_time(std::chrono::high_resolution_clock::now())
{
	mp_main_camera = std::make_shared<Camera>();
	
	mp_controller = std::make_shared<Controller>(mp_main_camera);

	ConsoleUI& console = m_console;

	std::function<void()> console_hide = [&console]() { 
		console.SetActive(!console.IsActive());
	};
	BindKeyToFunc(GLFW_KEY_GRAVE_ACCENT, console_hide, ActionType::R3D_PRESS);

	ObjectsViewer& viewer = m_viewer;
	std::function<void()> viewer_hide = [&viewer]() { 
		viewer.SetActive(!viewer.IsActive());
	};
	BindKeyToFunc(GLFW_KEY_F1, viewer_hide, ActionType::R3D_PRESS);

	mp_config = std::make_shared<Config>();
	mp_config->width = width;
	mp_config->height = height;

	mp_window = std::make_unique<Window>(mp_config, *mp_controller.get());

	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), mp_config->width, mp_config->height);
}

Engine::~Engine()
{
	vkDeviceWaitIdle(mp_renderer->GetDevice());

	mp_renderer->cleanSwapchain();

	mp_scene->CleanRessources(mp_renderer);
	mp_scene.reset();

	mp_renderer.reset();
	mp_window.reset();
	mp_config.reset();
}

void Engine::setScene(std::shared_ptr<Scene> p_scene)
{
	mp_scene = p_scene;
	mp_scene->Load(mp_renderer);
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

	Geometry geo;

	const float half_size = size / 2;

	// vertices
	uint32_t a = geo.addVertex({ -half_size, -half_size, -half_size }, vcolor, { .0f, .0f });
	uint32_t b = geo.addVertex({ half_size, -half_size, -half_size }, vcolor, { .0f, 2.f });
	uint32_t c = geo.addVertex({ -half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t d = geo.addVertex({ half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t e = geo.addVertex({ -half_size, -half_size, half_size }, vcolor, { .0f, .0f });
	uint32_t f = geo.addVertex({ half_size, -half_size, half_size }, vcolor, { .0f, 2.f });
	uint32_t g = geo.addVertex({ -half_size, half_size, half_size }, vcolor, { 2.f, .0f });
	uint32_t h = geo.addVertex({ half_size, half_size, half_size }, vcolor, { 2.f, 2.f });

	uint32_t a1 = geo.addVertex({ -half_size, -half_size, -half_size }, vcolor, { .0f, .0f });
	uint32_t b1 = geo.addVertex({ half_size, -half_size, -half_size }, vcolor, { .0f, 2.f });
	uint32_t c1 = geo.addVertex({ -half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t d1 = geo.addVertex({ half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t e1 = geo.addVertex({ -half_size, -half_size, half_size }, vcolor, { .0f, .0f });
	uint32_t f1 = geo.addVertex({ half_size, -half_size, half_size }, vcolor, { .0f, 2.f });
	uint32_t g1 = geo.addVertex({ -half_size, half_size, half_size }, vcolor, { 2.f, .0f });
	uint32_t h1 = geo.addVertex({ half_size, half_size, half_size }, vcolor, { 2.f, 2.f });

	uint32_t a2 = geo.addVertex({ -half_size, -half_size, -half_size }, vcolor, { .0f, .0f });
	uint32_t b2 = geo.addVertex({ half_size, -half_size, -half_size }, vcolor, { .0f, 2.f });
	uint32_t c2 = geo.addVertex({ -half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t d2 = geo.addVertex({ half_size, half_size, -half_size }, vcolor, { 2.f, .0f });
	uint32_t e2 = geo.addVertex({ -half_size, -half_size, half_size }, vcolor, { .0f, .0f });
	uint32_t f2 = geo.addVertex({ half_size, -half_size, half_size }, vcolor, { .0f, 2.f });
	uint32_t g2 = geo.addVertex({ -half_size, half_size, half_size }, vcolor, { 2.f, .0f });
	uint32_t h2 = geo.addVertex({ half_size, half_size, half_size }, vcolor, { 2.f, 2.f });

	// indices
	geo.addIndices(a, c, b);
	geo.addIndices(b, c, d);

	geo.addIndices(f, h, e);
	geo.addIndices(e, h, g);

	geo.addIndices(b1, d1, f1);
	geo.addIndices(f1, d1, h1);

	geo.addIndices(e1, g1, a1);
	geo.addIndices(a1, g1, c1);

	geo.addIndices(c2, g2, d2);
	geo.addIndices(d2, g2, h2);

	geo.addIndices(e2, a2, f2);
	geo.addIndices(f2, a2, b2);

	cube->LoadMesh(geo.vertices, geo.indices);
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

		mp_renderer->BeginRecordCommandBuffers(mp_renderer->GetCommandBuffer(frame), mp_renderer->GetFrameBuffer(frame));
		mp_scene->Render(mp_renderer->GetCommandBuffer(frame), frame);
		mp_renderer->EndRecordCommandBuffers(mp_renderer->GetCommandBuffer(frame));

		mp_scene->Clean(frame);
		mp_renderer->SetUpdated(frame);
	}

	mp_main_camera->UpdateUBO(static_cast<float>(mp_config->width), static_cast<float>(mp_config->height), frame);
	mp_scene->UpdateUBO(mp_main_camera, mp_renderer, frame);
	mp_scene->UpdateSceneUBO(mp_renderer);

	// Update UI
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_console.Update();
	m_viewer.Update();

	ImGui::Render();

	// std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Engine::draw()
{
	mp_renderer->UpdateUI();
	mp_renderer->draw();
}
