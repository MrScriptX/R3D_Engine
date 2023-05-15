#include "Engine.h"

Engine::Engine(const vred::settings& config) : m_extent({ config.window_width, config.window_height }), m_last_time(std::chrono::high_resolution_clock::now())
{
	mp_main_camera = std::make_shared<Camera>();
	mp_controller = std::make_shared<Controller>(mp_main_camera);

	// setup default UI
	ConsoleUI* console = &ConsoleUI::Get();
	std::function<void()> console_hide = [console]() { console->SetActive(!console->IsActive()); };
	BindKeyToFunc(GLFW_KEY_GRAVE_ACCENT, console_hide, ActionType::R3D_PRESS);

	m_UIs.push_back(console);

	Watcher* watcher = &Watcher::Get();
	std::function<void()> watcher_hide = [watcher]() { watcher->SetActive(!watcher->IsActive()); };
	BindKeyToFunc(GLFW_KEY_F1, watcher_hide, ActionType::R3D_PRESS);

	m_UIs.push_back(watcher);

	// start the interface
	mp_window = std::make_unique<Window>(config, *mp_controller.get());
	mp_renderer = std::make_shared<Renderer>(mp_window->getHandle(), m_extent.width, m_extent.height);
}

Engine::~Engine()
{
	mp_renderer->clean_swapchain();
	destroy_pipelines();

	mp_scene->CleanRessources(mp_renderer);
	mp_scene.reset();

	mp_renderer.reset();
	mp_window.reset();
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

std::string Engine::create_pipeline(const std::string& name, const vred::renderer::shader_stages& shaders)
{
	vred::renderer::ipipeline pipeline;
	pipeline.layout = vred::renderer::create_pipeline_layout(mp_renderer->GetDevice(), mp_renderer->get_descriptor_set_layout());
	pipeline.handle = vred::renderer::create_pipeline(shaders, pipeline.layout, mp_renderer->GetDevice(), mp_renderer->get_swapchain());

	if (m_pipelines.find(name) != m_pipelines.end())
		return "";

	m_pipelines.insert({ name, pipeline });
	m_shaders.insert({ name, shaders });
	
	return name;
}

void Engine::destroy_pipeline(const std::string& name)
{
	const auto& pipeline = m_pipelines.find(name);
	vred::renderer::destroy_pipeline(pipeline->second, mp_renderer->GetDevice());

	m_pipelines.erase(name);
	m_shaders.erase(name);
}

const std::shared_ptr<Material> Engine::CreateMaterial(const std::string& shader)
{
	return std::make_shared<Material>(shader, mp_renderer);
}

const std::shared_ptr<Material> Engine::CreateMaterial(const std::string& shader, const std::string& texture_file)
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

std::shared_ptr<Camera> Engine::GetMainCamera() const
{
	return mp_main_camera;
}

void Engine::SetWireframeMode()
{
	// mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_LINE);

	for (auto& shader : m_shaders)
		shader.second.polygon_mode = VK_POLYGON_MODE_LINE;

	destroy_pipelines();
	create_pipelines();

	mp_scene->ToUpdate();
}

void Engine::SetPointMode()
{
	// mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_POINT);

	for (auto& shader : m_shaders)
		shader.second.polygon_mode = VK_POLYGON_MODE_POINT;

	destroy_pipelines();
	create_pipelines();

	mp_scene->ToUpdate();
}

void Engine::SetFillMode()
{
	// mp_renderer->SetPolygonFillingMode(VK_POLYGON_MODE_FILL);

	for (auto& shader : m_shaders)
		shader.second.polygon_mode = VK_POLYGON_MODE_FILL;

	destroy_pipelines();
	create_pipelines();

	mp_scene->ToUpdate();
}

void Engine::SetColorMode(const ColorMode color_map)
{
	mp_renderer->SetColorMode(color_map);

	destroy_pipelines();
	create_pipelines();

	mp_scene->ToUpdate();
}

void Engine::RenderUI(UI& ui)
{
	m_UIs.push_back(&ui);
}

void Engine::RemoveUI(const UI& ui)
{
	for (auto it = m_UIs.begin(); it != m_UIs.end(); ++it)
	{
		if (*it == &ui)
		{
			m_UIs.erase(it);
			break;
		}
	}
}

bool Engine::shouldClose() const
{
	return glfwWindowShouldClose(&mp_window->getHandle());
}

void Engine::update_window_size()
{
	int width, height = 0;
	glfwGetWindowSize(&mp_window->getHandle(), &width, &height);

	m_extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

void Engine::update()
{
	glfwPollEvents();

	const std::chrono::time_point<std::chrono::high_resolution_clock> current_time = std::chrono::high_resolution_clock::now();
	const float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - m_last_time).count();

	mp_controller->Update(delta_time);

	m_last_time = current_time;

	const int32_t frame = mp_renderer->AcquireNextImage();
	if (frame == -1)
	{
		update_window_size();
		mp_renderer->reset(m_extent);

		destroy_pipelines();
		create_pipelines();

		return;
	}

	if (mp_scene->IsUpdate(frame) || mp_renderer->NeedUpdate(frame))
	{
		mp_renderer->WaitForSwapchainImageFence();

		mp_scene->Update(frame);

		mp_renderer->begin_render(frame);
		// mp_scene->Render(mp_renderer->GetCommandBuffer(frame), frame, m_pipelines);

		// test new render loop
		auto objects = mp_scene->get_objects();
		for (auto& object : objects)
		{
			for (size_t i = 0; i < object->getMeshesCount(); ++i)
			{
				Buffer& buffer = object->getMesh(i)->GetBuffer(frame);
				std::shared_ptr<Material> material = object->getMesh(i)->getMaterial();

				vred::renderer::ipipeline pipeline = m_pipelines.find(material->pipeline())->second;

				vkCmdBindPipeline(mp_renderer->GetCommandBuffer(frame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

				const VkBuffer vertex_buffer[] = { buffer.vertex };
				const VkDeviceSize offsets[] = { 0 };

				vkCmdBindVertexBuffers(mp_renderer->GetCommandBuffer(frame), 0, 1, vertex_buffer, offsets);
				vkCmdBindIndexBuffer(mp_renderer->GetCommandBuffer(frame), buffer.index, 0, VK_INDEX_TYPE_UINT32);

				const VkDescriptorSet sets[] = { material->getDescriptorSet(), mp_scene->GetDescriptorSet() };

				vkCmdBindDescriptorSets(mp_renderer->GetCommandBuffer(frame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 2, sets, 0, nullptr);

				vkCmdDrawIndexed(mp_renderer->GetCommandBuffer(frame), static_cast<uint32_t>(object->getMesh(i)->get_indices().size()), 1, 0, 0, 0);
			}
		}
		mp_scene->SetUpdated(frame);
		//
		mp_renderer->end_render(frame);
		
		mp_scene->Clean(frame);
		mp_renderer->SetUpdated(frame);
	}

	mp_main_camera->UpdateUBO(m_extent.width, m_extent.height, frame);
	mp_scene->UpdateUBO(mp_main_camera, mp_renderer, frame);
	mp_scene->UpdateSceneUBO(mp_renderer);

	// Update UI
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (size_t i = 0; i < m_UIs.size(); i++)
		m_UIs[i]->Update(m_extent.width, m_extent.height);

	ImGui::Render();

	// std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Engine::draw()
{
	mp_renderer->UpdateUI();
	if (mp_renderer->draw() != 0)
	{
		update_window_size();

		mp_renderer->reset(m_extent);

		destroy_pipelines();
		create_pipelines();
	}
}

void Engine::create_pipelines()
{
	for (const auto& shader : m_shaders)
	{
		vred::renderer::ipipeline pipeline;
		pipeline.layout = vred::renderer::create_pipeline_layout(mp_renderer->GetDevice(), mp_renderer->get_descriptor_set_layout());
		pipeline.handle = vred::renderer::create_pipeline(shader.second, pipeline.layout, mp_renderer->GetDevice(), mp_renderer->get_swapchain());

		m_pipelines.insert({ shader.first, pipeline });
	}
}

void Engine::destroy_pipelines()
{
	for (const auto& pipeline : m_pipelines)
		vred::renderer::destroy_pipeline(pipeline.second, mp_renderer->GetDevice());

	m_pipelines.clear();
}
