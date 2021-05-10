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
		room->setPosition({ 3.0f, 0.0f, 0.0f });

		std::shared_ptr<Material> room2_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> room2 = engine.CreateGameObject();
		room2->loadMesh("assets/models/viking_room.obj");
		room2->bindMatToMesh(0, room2_texture);
		room2->setPosition({ 6.0f, 0.0f, 0.0f });

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->addGameObject(room);

		engine.setScene(scene);

		std::function<void()> my_func = []() {std::clog << "Test d'un key bind" << std::endl; };
		engine.BindKeyToFunc(GLFW_KEY_Q, my_func, ActionType::R3D_PRESS);

		int init = 0;
		// running loop
		do
		{
			if (init++ == 30000)
			{
				scene->addGameObject(room2);
			}

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