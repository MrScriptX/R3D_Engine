#include "Application.h"

void Application::Start()
{
	vred::settings setting;
	setting.app_name = "REDVoxel";
	setting.window_width = 1920;
	setting.window_height = 1080;

	try
	{
		mp_engine = std::make_unique<Engine>(setting);
		Logger::init();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	vred::renderer::shader_stages shaders_files;
	shaders_files.vertex = "../assets/shaders/vert.spv";
	shaders_files.fragment = "../assets/shaders/no_texture_shader.spv";

	const std::string shader = mp_engine->create_pipeline("shader", shaders_files);

	// init world
	std::shared_ptr<Material> world_mat = mp_engine->CreateMaterial(shader);
	std::shared_ptr<GameObject> world = mp_engine->CreateGameObject();

	mp_engine->GetMainCamera()->SetPosition({ 0.f, 10.f, 0.f });

	ChunkManager chunk_manager(world, world_mat, *mp_engine->GetMainCamera());
	chunk_manager.CreateWorld();

	glm::vec3 lposition = { 50.0f, 10.0f, 0.0f };

	// create global lighting
	std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();
	light->ambient_strength = 0.2f;
	light->diffuse_strength = 0.7f;
	light->specular_strength = 0.2f;
	light->color = glm::vec3(1.0f, 1.0f, 1.0f);
	light->position = lposition;
	light->direction = glm::normalize(light->position - glm::vec3(.0f, .0f, .0f));

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	mp_engine->setScene(scene);
	scene->AddGameObject(world);
	scene->AddLight(light);

	std::function<void()> wireframemode = [this]() {
		mp_engine->SetWireframeMode();
	};
	mp_engine->BindKeyToFunc(GLFW_KEY_Q, wireframemode, ActionType::R3D_PRESS);

	std::function<void()> fillmode = [this]() {
		mp_engine->SetFillMode();
	};
	mp_engine->BindKeyToFunc(GLFW_KEY_E, fillmode, ActionType::R3D_PRESS);

	// std::function<void()> hide_worldmanager = [this]() { mp_engine->SetFillMode(); };
	// mp_engine->BindKeyToFunc(GLFW_KEY_F2, hide_worldmanager, ActionType::R3D_PRESS);

	mp_engine->RenderUI(chunk_manager.GetMenu());

	do
	{
		const auto update_x = chunk_manager.compute_world_update_x(*mp_engine->GetMainCamera());
		if (update_x.updated)
			chunk_manager.update_world_x(update_x.created, update_x.update_plus, update_x.update_min);

		const auto update_z = chunk_manager.compute_world_update_z(*mp_engine->GetMainCamera());
		if (update_z.updated)
			chunk_manager.update_world_z(update_z.created, update_z.update_plus, update_z.update_min);

		if (update_x.updated || update_z.updated)
			scene->ToUpdate();

		Watcher::WatchPosition("camera", mp_engine->GetMainCamera()->GetPosition());

		mp_engine->update();
		mp_engine->draw();
	} while (!mp_engine->shouldClose());
}
