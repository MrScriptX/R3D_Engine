#include <world/GameObject.h>
#include <Engine.h>

int main()
{
	try
	{
		vred::settings settings;
		settings.app_name = "VRED Engine - Normal Map";

		Engine engine(settings);
		Logger::init();

		// create shaders
		vred::renderer::shader_stages textured_shader_stages;
		textured_shader_stages.vertex = "../assets/shaders/vert.spv";
		textured_shader_stages.fragment = "../assets/shaders/texture_shader.spv";

		std::string textured_shader = engine.create_pipeline("main", textured_shader_stages);

		vred::renderer::shader_stages light_shader_stages;
		light_shader_stages.vertex = "../assets/shaders/vert.spv";
		light_shader_stages.fragment = "../assets/shaders/light_source.spv";

		std::string light_shader = engine.create_pipeline("light", light_shader_stages);

		// setup 3D models
		std::shared_ptr<Material> room_texture = engine.CreateMaterial(textured_shader, "../assets/textures/viking_room.png");
		std::shared_ptr<GameObject> room = engine.CreateGameObject();
		room->LoadMesh("../assets/models/viking_room.obj");
		room->bindMatToMesh(0, room_texture);
		room->setPosition({ 3.0f, 0.0f, 3.0f });

		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(light_shader);
		std::shared_ptr<GameObject> cube = engine.CreateCube({ 2.0f, .0f, .0f }, 1.f, { 1.f, .0f, .0f });
		cube->bindMatToMesh(0, cube_texture);

		// setup scene
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->AddGameObject(room);
		scene->AddGameObject(cube);

		// set scene and color mode
		engine.setScene(scene);
		engine.SetColorMode(ColorMode::NORMALMAP);

		// set keys to switch fill mode
		std::function<void()> wireframemode = [&engine]() { engine.SetWireframeMode(); };
		engine.BindKeyToFunc(GLFW_KEY_Q, wireframemode, ActionType::R3D_PRESS);

		std::function<void()> fillmode = [&engine]() { engine.SetFillMode(); };
		engine.BindKeyToFunc(GLFW_KEY_E, fillmode, ActionType::R3D_PRESS);

		std::function<void()> pointmode = [&engine]() { engine.SetPointMode(); };
		engine.BindKeyToFunc(GLFW_KEY_R, pointmode, ActionType::R3D_PRESS);

		do
		{
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
