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

		std::shared_ptr<Material> cube_texture = engine.CreateMaterial(TSHADER::NO_TEXTURE);
		std::shared_ptr<GameObject> cube = engine.CreateGameObject();
		
		std::vector<Vertex> cube_vertices;
		std::vector<uint32_t> cube_indices;

		Vertex p1;
		p1.pos = { -0.5f, -0.5f, -0.5f };
		p1.color = { 1, 0, 0 };
		p1.texCoord = { 0, 0 };

		Vertex p2;
		p2.pos = { 0.5f, -0.5f, -0.5f };
		p2.color = { 0, 1, 0 };
		p2.texCoord = { 0, 2 };

		Vertex p3;
		p3.pos = { -0.5f, 0.5f, -0.5f };
		p3.color = { 0, 0, 1 };
		p3.texCoord = { 2, 0 };

		Vertex p4;
		p4.pos = { 0.5f, 0.5f, -0.5f };
		p4.color = { 1, 1, 0 };
		p4.texCoord = { 2, 0 };

		Vertex p5;
		p5.pos = { -0.5f, -0.5f, 0.5f };
		p5.color = { 1, 0, 0 };
		p5.texCoord = { 0, 0 };

		Vertex p6;
		p6.pos = { 0.5f, -0.5f, 0.5f };
		p6.color = { 0, 1, 0 };
		p6.texCoord = { 0, 2 };

		Vertex p7;
		p7.pos = { -0.5f, 0.5f, 0.5f };
		p7.color = { 0, 0, 1 };
		p7.texCoord = { 2, 0 };

		Vertex p8;
		p8.pos = { 0.5f, 0.5f, 0.5f };
		p8.color = { 0, 1, 1 };
		p8.texCoord = { 2, 2 };

		cube_vertices.push_back(p1);
		cube_vertices.push_back(p2);
		cube_vertices.push_back(p3);
		cube_vertices.push_back(p4);
		cube_vertices.push_back(p5);
		cube_vertices.push_back(p6);
		cube_vertices.push_back(p7);
		cube_vertices.push_back(p8);

		//check indices
		//front
		cube_indices.push_back(0);
		cube_indices.push_back(2);
		cube_indices.push_back(1);
		cube_indices.push_back(1);
		cube_indices.push_back(2);
		cube_indices.push_back(3);

		//back
		cube_indices.push_back(5);
		cube_indices.push_back(7);
		cube_indices.push_back(4);
		cube_indices.push_back(4);
		cube_indices.push_back(7);
		cube_indices.push_back(6);

		//right
		cube_indices.push_back(1);
		cube_indices.push_back(3);
		cube_indices.push_back(5);
		cube_indices.push_back(5);
		cube_indices.push_back(3);
		cube_indices.push_back(7);

		//left
		cube_indices.push_back(4);
		cube_indices.push_back(6);
		cube_indices.push_back(0);
		cube_indices.push_back(0);
		cube_indices.push_back(6);
		cube_indices.push_back(2);

		//top
		cube_indices.push_back(2);
		cube_indices.push_back(6);
		cube_indices.push_back(3);
		cube_indices.push_back(3);
		cube_indices.push_back(6);
		cube_indices.push_back(7);

		//bot
		cube_indices.push_back(4);
		cube_indices.push_back(0);
		cube_indices.push_back(5);
		cube_indices.push_back(5);
		cube_indices.push_back(0);
		cube_indices.push_back(1);

		cube->loadMesh(cube_vertices, cube_indices);
		cube->bindMatToMesh(0, cube_texture);
		cube->setPosition({ 2.0f, 0.f, 0.f });


		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		//scene->addGameObject(room);
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