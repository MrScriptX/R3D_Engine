#include "Application.h"
#include "Engine.h"

int main()
{
	Application app;
	//Engine engine;

	try
	{
		Logger::init();
		app.run();

		/*std::shared_ptr<Material> gun_texture = std::make_shared<Material>();
		gun_texture->loadTexture("textures/texture.jpg", engine.getRenderEngine());

		std::shared_ptr<GameObject> gun = std::make_shared<GameObject>(engine.getRenderEngine());
		gun->loadMesh("models/ak-47.obj");

		gun->bindMatToMesh(0, gun_texture);
		gun->setPosition(glm::vec3({ 10, 0, 0 }));

		std::shared_ptr<Material> room_texture = std::make_shared<Material>();
		room_texture->loadTexture("textures/viking_room.png", engine.getRenderEngine());

		std::shared_ptr<GameObject> room = std::make_shared<GameObject>(engine.getRenderEngine());
		room->loadMesh("models/viking_room.obj");

		room->bindMatToMesh(0, room_texture);*/

		//engine.run();
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return 0;
}