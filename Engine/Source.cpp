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

		const float half_size = 1.f / 2.f;
		Voxel voxel;

		//vertices
		uint32_t index_0 = voxel.addVertex({ 0.f - half_size, 0.f - half_size, -2.f - half_size }, { 0.f, 1.f, 0.f }, { .0f, .0f });
		uint32_t index_1 = voxel.addVertex({ 0.f + half_size, 0.f - half_size, -2.f - half_size }, { 0.f, 1.f, 0.f }, { .0f, 2.0f });
		uint32_t index_2 = voxel.addVertex({ 0.f - half_size, 0.f + half_size, -2.f - half_size }, { 0.f, 1.f, 0.f }, { 2.0f, .0f });
		uint32_t index_3 = voxel.addVertex({ 0.f + half_size, 0.f + half_size, -2.f - half_size }, { 0.f, 1.f, 0.f }, { 2.0f, .0f });
		uint32_t index_4 = voxel.addVertex({ 0.f - half_size, 0.f - half_size, -2.f + half_size }, { 0.f, 1.f, 0.f }, { .0f, .0f });
		uint32_t index_5 = voxel.addVertex({ 0.f + half_size, 0.f - half_size, -2.f + half_size }, { 0.f, 1.f, 0.f }, { .0f, 2.0f });
		uint32_t index_6 = voxel.addVertex({ 0.f - half_size, 0.f + half_size, -2.f + half_size }, { 0.f, 1.f, 0.f }, { 2.0f, .0f });
		uint32_t index_7 = voxel.addVertex({ 0.f + half_size, 0.f + half_size, -2.f + half_size }, { 0.f, 1.f, 0.f }, { 2.0f, 2.0f });

		//indices
		voxel.addIndices(index_0, index_2, index_1);
		voxel.addIndices(index_1, index_2, index_3);

		voxel.addIndices(index_5, index_7, index_4);
		voxel.addIndices(index_4, index_7, index_6);

		voxel.addIndices(index_1, index_3, index_5);
		voxel.addIndices(index_5, index_3, index_7);

		voxel.addIndices(index_4, index_6, index_0);
		voxel.addIndices(index_0, index_6, index_2);

		voxel.addIndices(index_2, index_6, index_3);
		voxel.addIndices(index_3, index_6, index_7);

		voxel.addIndices(index_4, index_0, index_5);
		voxel.addIndices(index_5, index_0, index_1);

		cube->LoadMesh(voxel.vertices, voxel.indices);

		cube->bindMatToMesh(1, cube_texture);

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
			//cube->setPosition(cube->getPosition() + glm::vec3{0.0001f, 0.f, 0.f});

			if (init++ == 10000)
			{
				Voxel v;
				v.vertices = cube->GetVertices(1);
				v.indices = cube->GetIndices(1);
				v.vertices[0].color = { .0f, .0f, 1.0f };

				cube->UpdateMesh(1, v.vertices, v.indices);
				scene->Update();
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