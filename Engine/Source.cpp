#include "Engine.h"

int main()
{
	try
	{
		Engine engine;
		Logger::init();

		std::shared_ptr<Material> room_texture = engine.CreateMaterial(TSHADER::TEXTURE);
		room_texture->LoadTexture("assets/textures/viking_room.png");

		std::shared_ptr<GameObject> room = engine.CreateGameObject();
		room->loadMesh("assets/models/viking_room.obj");
		room->bindMatToMesh(0, room_texture);
		room->setPosition({ 3.0f, 0.0f, 3.0f });

		std::shared_ptr<Material> room2_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> room2 = engine.CreateGameObject();
		room2->loadMesh("assets/models/viking_room.obj");
		room2->bindMatToMesh(0, room2_texture);
		room2->setPosition({ 6.0f, 0.0f, 0.0f });

		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> cube = engine.CreateCube({ 2.0f, .0f, .0f }, 1.f, {1.f, .0f, .0f});
		cube->bindMatToMesh(0, cube_texture);

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->addGameObject(room);
		scene->addGameObject(room2);
		scene->addGameObject(cube);

		engine.setScene(scene);

		std::function<void()> wireframemode = [&engine]() {
			engine.SetWireframeMode();
		};
		engine.BindKeyToFunc(GLFW_KEY_Q, wireframemode, ActionType::R3D_PRESS);

		std::function<void()> fillmode = [&engine]() {
			engine.SetFillMode();
		};
		engine.BindKeyToFunc(GLFW_KEY_E, fillmode, ActionType::R3D_PRESS);

		std::function<void()> pointmode = [&engine]() {
			engine.SetPointMode();
		};
		engine.BindKeyToFunc(GLFW_KEY_R, pointmode, ActionType::R3D_PRESS);

		int init = 0;
		// running loop
		do
		{
			cube->setPosition(cube->getPosition() + glm::vec3{0.0001f, 0.f, 0.f});

			engine.update();
			engine.draw();
		} while (!engine.shouldClose());
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		system("pause");//no system pause | try catch should be done more localy depending on where to end 
		return EXIT_FAILURE;
	}

	return 0;
}