#include <Engine.h>

int main()
{
	try
	{
		Engine engine(1280, 720);
		Logger::init();

		std::shared_ptr<Material> room_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> room = engine.CreateGameObject();
		room->LoadMesh("../assets/models/viking_room.obj");
		room->bindMatToMesh(0, room_texture);
		room->setPosition({ 3.0f, 0.0f, 3.0f });

		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> cube = engine.CreateCube({ 2.0f, .0f, .0f }, 1.f, { 1.f, .0f, .0f });
		cube->bindMatToMesh(0, cube_texture);

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->AddGameObject(room);
		scene->AddGameObject(cube);

		engine.setScene(scene);
		engine.SetColorMode(ColorMode::NORMALMAP);


		std::function<void()> wireframemode = [&engine]() { engine.SetWireframeMode(); };
		engine.BindKeyToFunc(GLFW_KEY_Q, wireframemode, ActionType::R3D_PRESS);

		std::function<void()> fillmode = [&engine]() { engine.SetFillMode(); };
		engine.BindKeyToFunc(GLFW_KEY_E, fillmode, ActionType::R3D_PRESS);

		std::function<void()> pointmode = [&engine]() { engine.SetPointMode(); };
		engine.BindKeyToFunc(GLFW_KEY_R, pointmode, ActionType::R3D_PRESS);

		int init = 0;
		// running loop
		do
		{
			// cube->setPosition(cube->getPosition() + glm::vec3{0.0001f, 0.f, 0.f});

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