#include "Engine.h"

int main()
{
	try
	{
		Engine engine;
		Logger::init();

		std::shared_ptr<Material> room_texture = std::make_shared<Material>();
		room_texture->loadTexture("assets/textures/viking_room.png", engine.getRenderEngine());

		std::shared_ptr<GameObject> room = std::make_shared<GameObject>(engine.getRenderEngine());
		room->loadMesh("assets/models/viking_room.obj");
		room->bindMatToMesh(0, room_texture);
		room->setPosition({ -3.0f, 0.0f, 0.0f });

		std::shared_ptr<Material> room2_texture = std::make_shared<Material>();
		room2_texture->loadTexture("assets/textures/viking_room.png", engine.getRenderEngine());

		std::shared_ptr<GameObject> room2 = std::make_shared<GameObject>(engine.getRenderEngine());

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->addGameObject(room);

		engine.setScene(scene);

		int init = 0;
		// running loop
		do
		{
			if (init == 30000) {
				room2->loadMesh("assets/models/viking_room.obj");
				room2->bindMatToMesh(0, room2_texture);
				room2->setPosition({ -13.0f, 0.0f, 0.0f });

				scene->addGameObject(room2);
			}

			init++;

			if (init == 60000) {
				scene->removeGameObject(room2);
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