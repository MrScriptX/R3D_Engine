#include <Engine.h>

int main()
{
	try
	{
		Engine engine(1280, 720);
		Logger::init();

		std::shared_ptr<GameObject> cube = engine.CreateCube({ .0f, .0f, .0f }, 1.f, { 1.f, .0f, .0f });
		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		cube->bindMatToMesh(0, cube_texture);

		glm::vec3 light_pos = { 2.0f, .0f, .0f };
		std::shared_ptr<GameObject> light_cube = engine.CreateCube(light_pos, 0.2f, { 1.f, 1.f, 1.f });
		std::shared_ptr<Material> cube_texture2 = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		light_cube->bindMatToMesh(0, cube_texture2);

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		scene->AddGameObject(cube);
		scene->AddGameObject(light_cube);

		std::shared_ptr<LightObject> light = std::make_shared<LightObject>();
		scene->AddLight(light);

		engine.setScene(scene);

		std::chrono::time_point<std::chrono::high_resolution_clock> last = std::chrono::high_resolution_clock::now();
		float radius = 2.0f;
		float gpos = 0.f;
		do
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> current_time = std::chrono::high_resolution_clock::now();
			float delta_time = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last).count();
			last = current_time;

			float speed = 0.0005f * delta_time;
			gpos += speed;

			float x = glm::cos(gpos) * radius;
			float z = glm::sin(gpos) * radius;

			light_pos = glm::vec3(x, .0f, z);


			light_cube->setPosition(light_pos);
			Light transform;
			transform.position = light_pos;
			transform.rotation = glm::vec3(.0f, .0f, .0f);
			light->SetTransform(transform);

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