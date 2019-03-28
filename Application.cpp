#include "Application.h"


#include <thread>
#include <unordered_map>

#ifndef TINYOBJLOADER_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#endif // !TINYOBJLOADER_IMPLEMENTATION

#ifndef ASSIMP_BUILD_BOOST_WORKAROUND
#define ASSIMP_BUILD_BOOST_WORKAROUND
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif // !ASSIMP_BUILD_BOOST_WORKAROUND




Buffer buffers;
const std::string MODEL_PATH = "models/chalet.obj";
const std::string TEXTURE_PATH = "textures/chalet.jpg";

std::vector<Vertex> vertices;
std::vector<uint32_t> indices;

Application::Application()
{
	m_pPlayer = std::make_shared<Player>();
	m_config = std::make_unique<Config>();
	m_window = std::make_unique<Window>(m_config, m_pPlayer->getCamera());
	m_pRenderer = std::make_shared<Renderer>(m_window->getHandle(), m_config->width, m_config->height);


	m_pRenderer->createNewPipeline(base_pipeline);
	m_pRenderer->createDepthResources();
	m_pRenderer->createFramebuffer();

	m_pRenderer->createTextureImage();
	m_pRenderer->createTextureImageView();
	m_pRenderer->createTextureSampler();

	//------------------------------------------------------------------------------------------
	/*Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(MODEL_PATH, aiProcessPreset_TargetRealtime_Quality);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		for (int t = 0; t < mesh->mNumVertices; t++)
		{
			Vertex vertex;
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertex.texCoord.x = mesh->mTextureCoords[i]->x;
			vertex.texCoord.y = mesh->mTextureCoords[i]->y;

			vertices.push_back(vertex);
		}

		for (int x = 0; x < mesh->mFaces->mNumIndices; x++)
		{
			indices.push_back(mesh->mFaces->mIndices[x]);
		}
	}*/

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	m_pRenderer->createVerticesBuffer(std::make_shared<std::vector<Vertex>>(vertices), buffers);
	m_pRenderer->createIndicesBuffer(std::make_shared<std::vector<uint32_t>>(indices), buffers);
	//-------------------------------------------------------------------------------

	m_pRenderer->createDescriptorPool();

	m_pRenderer->createUBO();

	m_pRenderer->allocateCommandBuffers();
	
	m_pRenderer->createDescriptorSet();
}


Application::~Application()
{
	m_pRenderer->cleanSwapchain(std::make_shared<Pipeline>(base_pipeline));
	m_pRenderer->destroyTextures();
	m_pRenderer->destroyDescriptors();
	m_pRenderer->destroyUniformBuffer();

	m_pRenderer.reset();
	m_window.reset();
	m_config.reset();
}

void Application::run()
{
	for (uint16_t i = 0; i < m_pRenderer->getGraphic().command_buffers.size(); i++)
	{
		m_pRenderer->beginRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i], m_pRenderer->getGraphic().framebuffers[i], base_pipeline);

		m_pRenderer->recordDrawCommands(m_pRenderer->getGraphic().command_buffers[i], base_pipeline, buffers, indices.size());

		m_pRenderer->endRecordCommandBuffers(m_pRenderer->getGraphic().command_buffers[i]);
	}
	
	while (!glfwWindowShouldClose(&m_window->getHandle()))
	{
		glfwPollEvents();
		input();
		update();
		m_pRenderer->draw(base_pipeline);
	}

	vkDeviceWaitIdle(m_pRenderer->getDevice());
}

void Application::update()
{
	/*m_pPlayer->updateUBO(static_cast<float>(m_config->width), static_cast<float>(m_config->height));

	void* data;
	vkMapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory, 0, sizeof(m_pPlayer->getUBO()), 0, &data);
	memcpy(data, &m_pPlayer->getUBO(), sizeof(m_pPlayer->getUBO()));
	vkUnmapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory);*/

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f),  (float)m_config->width/ (float)m_config->height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(m_pRenderer->getDevice(), m_pRenderer->getGraphic().uniform_memory);

	//std::this_thread::sleep_for(std::chrono::nanoseconds(500));//delete when not streaming
}

void Application::input()
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_last_time).count();

	m_pPlayer->update(delta_time);

	m_last_time = currentTime;
}


