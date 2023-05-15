#include <ui/Watcher.h>
#include <Engine.h>

#include "LightMenu.h"
#include <Logger.h>

int main()
{
	try
	{
		vred::settings settings;
		settings.app_name = "VRED Engine - Moving Light";
		settings.window_width = 1920;
		settings.window_height = 1080;

		Engine engine(settings);
		Logger::init();
		vred::Log::init();

		vred::Log::message(LOG_TYPE::INFO, "Moving Light Example Project");
		
		// build pipelines
		vred::renderer::shader_stages cube_shader_stages;
		cube_shader_stages.vertex = "../assets/shaders/vert.spv";
		cube_shader_stages.fragment = "../assets/shaders/no_texture_shader.spv";

		std::string cube_shader = engine.create_pipeline("main", cube_shader_stages);

		vred::renderer::shader_stages light_shader_stages;
		light_shader_stages.vertex = "../assets/shaders/vert.spv";
		light_shader_stages.fragment = "../assets/shaders/light_source.spv";

		std::string light_shader = engine.create_pipeline("light", light_shader_stages);

		// create object
		std::shared_ptr<GameObject> cube = engine.CreateCube({ .0f, .0f, .0f }, 1.f, { 1.f, .0f, .0f });
		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(cube_shader);
		cube->bindMatToMesh(0, cube_texture);

		ConsoleUI::Log("Create cube 1 at pos {} {} {}", .0f, .0f, .0f);

		std::shared_ptr<GameObject> cube2 = engine.CreateCube({ .0f, .0f, 3.0f }, 1.f, { .0f, 1.0f, .0f });
		std::shared_ptr<Material> cube_texture2 = engine.CreateMaterial(cube_shader);
		cube2->bindMatToMesh(0, cube_texture2);

		ConsoleUI::Log("Create cube 2 at pos {} {} {}", .0f, .0f, 3.0f);

		std::shared_ptr<GameObject> cube3 = engine.CreateCube({ 3.0f, .0f, .0f }, 1.f, { .0f, .0f, 1.0f });
		std::shared_ptr<Material> cube_texture3 = engine.CreateMaterial(cube_shader);
		cube3->bindMatToMesh(0, cube_texture3);

		ConsoleUI::Log("Create cube 3 at pos {} {} {}", -3.0f, .0f, .0f);

		std::shared_ptr<GameObject> cube4 = engine.CreateCube({ -3.0f, .0f, .0f }, 1.f, { 1.0f, 1.0f, 1.0f });
		std::shared_ptr<Material> cube_texture4 = engine.CreateMaterial(cube_shader);
		cube4->bindMatToMesh(0, cube_texture4);

		ConsoleUI::Log("Create cube 4 at pos {} {} {}", -3.0f, .0f, .0f);

		std::shared_ptr<GameObject> cube5 = engine.CreateCube({ .0f, .0f, -3.0f }, 1.f, { 1.0f, 1.0f, .0f });
		std::shared_ptr<Material> cube_texture5 = engine.CreateMaterial(cube_shader);
		cube5->bindMatToMesh(0, cube_texture5);

		ConsoleUI::Log("Create cube 5 at pos {} {} {}", .0f, .0f, -3.0f);

		glm::vec3 light_pos = { 2.0f, .0f, .0f };
		std::shared_ptr<GameObject> light_cube = engine.CreateCube(light_pos, 0.2f, { 1.f, 1.f, 1.f });
		std::shared_ptr<Material> cube_light_mat = engine.CreateMaterial(light_shader);
		light_cube->bindMatToMesh(0, cube_light_mat);

		ConsoleUI::Log("Create light at pos {} {} {}", 2.0f, .0f, .0f);

		// create light
		std::shared_ptr<PointLight> light = std::make_shared<PointLight>();
		light->color = { 1.0f, 1.0f, 1.0f };
		light->ambient_strength = 0.15f;
		light->diffuse_strength = 1.0f;
		light->specular_strength = 1.0f;
		light->constant = 1.0f;
		light->linear = 1.0f;
		light->quadratic = 0.2f;

		LightMenu menu(light->color, light->ambient_strength, light->specular_strength, light->diffuse_strength);
		engine.RenderUI(menu);

		std::function<void()> key_light_menu = [&menu]() { menu.SetActive(!menu.IsActive()); };
		engine.BindKeyToFunc(GLFW_KEY_F2, key_light_menu, ActionType::R3D_PRESS);

		// setup scene
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->AddGameObject(cube);
		scene->AddGameObject(cube2);
		scene->AddGameObject(cube3);
		scene->AddGameObject(cube4);
		scene->AddGameObject(cube5);
		scene->AddGameObject(light_cube);
		scene->AddLight(light);

		engine.setScene(scene);

		engine.GetMainCamera()->SetPosition({ -5.f, 2.f, 7.f });

		std::chrono::time_point<std::chrono::high_resolution_clock> last = std::chrono::high_resolution_clock::now();
		const float radius = 5.0f;
		float gpos = 0.f;
		do
		{
			// compute light next position
			std::chrono::time_point<std::chrono::high_resolution_clock> current_time = std::chrono::high_resolution_clock::now();
			float delta_time = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last).count();
			last = current_time;

			float speed = 0.0005f * delta_time;
			gpos += speed;

			float x = glm::cos(gpos) * radius;
			float z = glm::sin(gpos) * radius;

			light_pos = glm::vec3(x, .0f, z);

			// update properties
			light_cube->setPosition(light_pos);
			light->position = light_pos;
			// light->direction = glm::normalize(light_pos - glm::vec3(.0f, .0f, .0f));

			Watcher::WatchPosition("camera", engine.GetMainCamera()->GetPosition());
			Watcher::WatchPosition("light", light->position);

			Watcher::WatchVariable("yaw", static_cast<double>(engine.GetMainCamera()->getYaw()));
			Watcher::WatchVariable("pitch", static_cast<double>(engine.GetMainCamera()->getPitch()));

			// update & draw
			engine.update();
			engine.draw();
		} while (!engine.shouldClose());
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}
