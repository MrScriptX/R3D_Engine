#include "../Includes/renderer/Renderer.h"


#include <set>

#ifndef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#endif // !STB_IMAGE_IMPLEMENTATION

//review createBuffer func

Renderer::Renderer(GLFWwindow& window, uint32_t width, uint32_t height)
{
	WIDTH = std::make_unique<uint32_t>(width);
	HEIGHT = std::make_unique<uint32_t>(height);
	m_is_updated = { false };

	m_pBufferFactory = std::make_unique<VulkanBuffer>(m_graphic);

	setupInstance(window);
	setupDevice();
	setupSwapchain();
	setupRenderPass();
	setupDescriptorSetLayout();
	setupCommandPool();

	mp_pipelines_manager = std::make_unique<VulkanPipeline>(m_graphic);
}


Renderer::~Renderer()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(m_graphic.device, m_graphic.semaphores_image_available[i], nullptr);
		vkDestroySemaphore(m_graphic.device, m_graphic.semaphores_render_finished[i], nullptr);
		vkDestroyFence(m_graphic.device, m_graphic.fences_in_flight[i], nullptr);
	}

	vkDestroyCommandPool(m_graphic.device, m_graphic.command_pool, nullptr);

	vkDestroyDevice(m_graphic.device, nullptr);

	if (m_graphic.validation_layer_enable)
	{
		DestroyDebugReportCallbackEXT(m_graphic.instance, callback, nullptr);
	}

	vkDestroySurfaceKHR(m_graphic.instance, m_graphic.surface, nullptr);
	vkDestroyInstance(m_graphic.instance, nullptr);
}

int32_t Renderer::draw()
{
	if (m_graphic.validation_layer_enable)
	{
		vkQueueWaitIdle(m_graphic.present_queue);
	}

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_graphic.semaphores_image_available[m_last_image] };

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = waitSemaphores;
	submit_info.pWaitDstStageMask = waitStages;

	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_graphic.command_buffers[m_current_image]; 

	VkSemaphore signalSemaphores[] = { m_graphic.semaphores_render_finished[m_last_image] };

	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signalSemaphores;

	vkResetFences(m_graphic.device, 1, &m_graphic.fences_in_flight[m_current_image]);

	VkResult result;
	result = vkQueueSubmit(m_graphic.graphics_queue, 1, &submit_info, m_graphic.fences_in_flight[m_current_image]);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapchains[] = { m_graphic.swapchain };
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapchains;
	present_info.pImageIndices = &m_current_image;
	present_info.pResults = nullptr;

	result = vkQueuePresentKHR(m_graphic.present_queue, &present_info);

	int return_code = 0;
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		recreateSwapchain();
		return_code = 1;
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	return return_code;
}

int32_t Renderer::AcquireNextImage()
{
	m_last_image = m_current_image;

	VkResult result;
	result = vkAcquireNextImageKHR(m_graphic.device, m_graphic.swapchain, std::numeric_limits<uint64_t>::max(), m_graphic.semaphores_image_available[m_current_image], VK_NULL_HANDLE, &m_current_image);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapchain();
		return -1;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swapchain image!");
	}

	return m_current_image;
}

void Renderer::WaitForSwapchainImageFence()
{
	vkWaitForFences(m_graphic.device, 1, &m_graphic.fences_in_flight[m_current_image], VK_TRUE, std::numeric_limits<uint64_t>::max());
}

void Renderer::setupInstance(GLFWwindow& window)
{
	m_instance = std::make_unique<VulkanInstance>(m_graphic);
	setupCallback();
	createSurface(window);
}

void Renderer::setupDevice()
{
	m_device = std::make_unique<VulkanDevice>(m_graphic);
	createSyncObject();
}

void Renderer::setupSwapchain()
{
	m_swapchain = std::make_unique<VulkanSwapchain>(m_graphic, *WIDTH.get(), *HEIGHT.get());
}

void Renderer::setupRenderPass()
{
	VkFormat format = findSupportedFormat( { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	m_pRenderpass = std::make_unique<VulkanRenderPass>(m_graphic, format);
}

void Renderer::setupDescriptorSetLayout()
{
	m_descriptor = std::make_unique<VulkanDescriptor>(m_graphic);
	m_descriptor->createDescriptorSetLayout();
}

void Renderer::setupCommandPool()
{
	m_commandPool = std::make_unique<VulkanCommandPool>(m_graphic);
}

void Renderer::SetPolygonFillingMode(const VkPolygonMode& mode)
{
	m_graphic.polygone_mode = mode;

	recreateSwapchain();
}

VkDevice& Renderer::getDevice()
{
	return m_graphic.device;
}

VkDescriptorPool& Renderer::getDescriptorPool()
{
	return m_graphic.descriptor_pool;
}

VkDescriptorSetLayout& Renderer::getDescriptorSetLayout()
{
	return m_graphic.descriptor_set_layout;
}

const size_t Renderer::getNumberCommandBuffer()
{
	return m_graphic.command_buffers.size();
}

VkFramebuffer& Renderer::getFrameBuffer(const size_t& i)
{
	return m_graphic.framebuffers[i];
}

VkCommandBuffer& Renderer::getCommandBuffer(const size_t& i)
{
	return m_graphic.command_buffers[i];
}

std::unique_ptr<VulkanBuffer>& Renderer::getBufferFactory()
{
	return m_pBufferFactory;
}

std::unique_ptr<VulkanPipeline>& Renderer::GetPipelineFactory()
{
	return mp_pipelines_manager;
}

const int Renderer::getFrameIndex()
{
	return m_current_image;
}

const bool Renderer::IsUpdated()
{
	return m_is_updated == false;
}

const bool Renderer::NeedUpdate(const size_t& i)
{
	return m_is_updated[i];
}

void Renderer::SetUpdated(const size_t& i)
{
	m_is_updated[i] = false;
}

const uint32_t& Renderer::GetHeight()
{
	return m_swapchain->GetHeigth();
}

void Renderer::destroyBuffers(Buffer & buffer)
{
	vkQueueWaitIdle(m_graphic.graphics_queue);

	for (uint32_t i = 0; i < m_graphic.command_buffers.size(); i++)
	{
		vkResetCommandBuffer(m_graphic.command_buffers[i], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	vkDestroyBuffer(m_graphic.device, buffer.index, nullptr);
	vkFreeMemory(m_graphic.device, buffer.index_memory, nullptr);

	vkDestroyBuffer(m_graphic.device, buffer.vertex, nullptr);
	vkFreeMemory(m_graphic.device, buffer.vertex_memory, nullptr);
}

void Renderer::createVerticesBuffer(std::shared_ptr<std::vector<Vertex>> vertices, Buffer& buffer)
{
	VkDeviceSize buffer_size = sizeof(vertices->at(0)) * vertices->size();

	VkBuffer staging_buffer;
	VkDeviceMemory staging_buffer_memory;
	m_pBufferFactory->createBuffer(staging_buffer, staging_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(m_graphic.device, staging_buffer_memory, 0, buffer_size, 0, &data);
	memcpy(data, vertices->data(), (size_t)buffer_size);
	vkUnmapMemory(m_graphic.device, staging_buffer_memory);

	m_pBufferFactory->createBuffer(buffer.vertex, buffer.vertex_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	copyBuffer(staging_buffer, buffer.vertex, buffer_size);

	vkDestroyBuffer(m_graphic.device, staging_buffer, nullptr);
	vkFreeMemory(m_graphic.device, staging_buffer_memory, nullptr);
}

void Renderer::createIndicesBuffer(std::shared_ptr<std::vector<uint32_t>> indices, Buffer& buffer)
{
	VkDeviceSize buffer_size = sizeof(indices->at(0)) * indices->size();

	VkBuffer staging_buffer;
	VkDeviceMemory staging_buffer_mem;
	m_pBufferFactory->createBuffer(staging_buffer, staging_buffer_mem, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(m_graphic.device, staging_buffer_mem, 0, buffer_size, 0, &data);
	memcpy(data, indices->data(), (size_t)buffer_size);
	vkUnmapMemory(m_graphic.device, staging_buffer_mem);

	m_pBufferFactory->createBuffer(buffer.index, buffer.index_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	copyBuffer(staging_buffer, buffer.index, buffer_size);

	vkDestroyBuffer(m_graphic.device, staging_buffer, nullptr);
	vkFreeMemory(m_graphic.device, staging_buffer_mem, nullptr);
}

void Renderer::createUBO()
{
	VkDeviceSize buffer_size = sizeof(UniformBufferObject);
	m_pBufferFactory->createBuffer(m_graphic.uniform_buffer, m_graphic.uniform_memory, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Renderer::createUBO(VkBuffer& uniform_buffer, VkDeviceMemory& uniform_memory)
{
	VkDeviceSize buffer_size = sizeof(UniformBufferObject);
	m_pBufferFactory->createBuffer(uniform_buffer, uniform_memory, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Renderer::allocateCommandBuffers()
{
	m_graphic.command_buffers.resize(m_graphic.framebuffers.size());

	VkCommandBufferAllocateInfo alloc_buffers_info = {};
	alloc_buffers_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_buffers_info.commandPool = m_graphic.command_pool;
	alloc_buffers_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_buffers_info.commandBufferCount = static_cast<uint32_t>(m_graphic.command_buffers.size());

	if (vkAllocateCommandBuffers(m_graphic.device, &alloc_buffers_info, m_graphic.command_buffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void Renderer::beginRecordCommandBuffers(VkCommandBuffer & commandBuffer, VkFramebuffer& frameBuffer)
{
	// reset command buffer
	if (vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to reset command buffer!");
	}

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	VkCommandBufferBeginInfo begin_buffer_info = {};
	begin_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_buffer_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	begin_buffer_info.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &begin_buffer_info) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_graphic.render_pass;
	renderPassInfo.framebuffer = frameBuffer;

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_graphic.swapchain_details.extent;

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
	renderPassInfo.pClearValues = clear_values.data();

	m_pRenderpass->beginRenderPass(commandBuffer, renderPassInfo);
}

void Renderer::endRecordCommandBuffers(VkCommandBuffer & commandBuffer)
{
	m_pRenderpass->endRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void Renderer::setupCallback()
{
	if (!m_graphic.validation_layer_enable)
	{
		return;
	}

	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	create_info.pfnCallback = debugCallback;
	
	if (CreateDebugReportCallbackEXT(m_graphic.instance, &create_info, nullptr, &callback) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to setup callback");
	}
}

void Renderer::createSurface(GLFWwindow& window)
{
	if (glfwCreateWindowSurface(m_graphic.instance, &window, nullptr, &m_graphic.surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}
}

void Renderer::createFramebuffer()
{
	m_graphic.framebuffers.resize(m_graphic.images_view.size());

	for (size_t i = 0; i < m_graphic.images_view.size(); i++)
	{
		std::array<VkImageView, 2> attachments = { m_graphic.images_view[i], m_graphic.depth_view };

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_graphic.render_pass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_graphic.swapchain_details.extent.width;
		framebufferInfo.height = m_graphic.swapchain_details.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_graphic.device, &framebufferInfo, nullptr, &m_graphic.framebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void Renderer::createCommandPool()
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.pNext = nullptr;
	pool_info.queueFamilyIndex = m_graphic.queue_indices.graphic_family;
	pool_info.flags = 0;

	if (vkCreateCommandPool(m_graphic.device, &pool_info, nullptr, &m_graphic.command_pool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void Renderer::createSyncObject()
{
	m_graphic.semaphores_image_available.resize(MAX_FRAMES_IN_FLIGHT);
	m_graphic.semaphores_render_finished.resize(MAX_FRAMES_IN_FLIGHT);
	m_graphic.fences_in_flight.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_info.pNext = nullptr;
	semaphore_info.flags = 0;

	VkFenceCreateInfo fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.pNext = nullptr;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(m_graphic.device, &semaphore_info, nullptr, &m_graphic.semaphores_image_available[i]) != VK_SUCCESS ||
			vkCreateSemaphore(m_graphic.device, &semaphore_info, nullptr, &m_graphic.semaphores_render_finished[i]) != VK_SUCCESS ||
			vkCreateFence(m_graphic.device, &fence_info, nullptr, &m_graphic.fences_in_flight[i]) != VK_SUCCESS)
		{
			Logger::registerError("failed to create semaphores for a frame!");
			throw std::runtime_error("failed to create semaphores for a frame!");
		}
	}
}

void Renderer::createDescriptorLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(m_graphic.device, &layoutInfo, nullptr, &m_graphic.descriptor_set_layout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void Renderer::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 1;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 4;

	if (vkCreateDescriptorPool(m_graphic.device, &poolInfo, nullptr, &m_graphic.descriptor_pool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Renderer::allocateDescriptorSet(VkDescriptorSet& descriptor_set)
{
	VkDescriptorSetLayout layouts[] = { m_graphic.descriptor_set_layout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_graphic.descriptor_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	VkResult result = vkAllocateDescriptorSets(m_graphic.device, &allocInfo, &descriptor_set);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor set!");
	}
}

void Renderer::updateDescriptorSet(const VkBuffer& ubo, const VkDescriptorSet& descriptor_set)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = ubo;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptor_set;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(m_graphic.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void Renderer::updateDescriptorSet(const VkBuffer& ubo, const VkDescriptorSet& descriptor_set, const VkImageView& image_view, const VkSampler& image_sampler)
{
	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = ubo;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptor_set;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = image_view;
	imageInfo.sampler = image_sampler;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = descriptor_set;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(m_graphic.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void Renderer::createDepthResources()
{
	VkFormat depthFormat = findDepthFormat();

	createImage(m_graphic.swapchain_details.extent.width, m_graphic.swapchain_details.extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_graphic.depth_image, m_graphic.depth_memory);
	m_graphic.depth_view = createImageView(m_graphic.depth_image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

	transitionImageLayout(m_graphic.depth_image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void Renderer::destroyUniformBuffer()
{
	vkDestroyBuffer(m_graphic.device, m_graphic.uniform_buffer, nullptr);
	vkFreeMemory(m_graphic.device, m_graphic.uniform_memory, nullptr);
}

VkImageView Renderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageView imageView;

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(m_graphic.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void Renderer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;

	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	if (vkCreateImage(m_graphic.device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}


	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_graphic.device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(m_graphic.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(m_graphic.device, image, imageMemory, 0);
}

void Renderer::recreateSwapchain()
{
	cleanSwapchain();

	m_swapchain->createSwapchain();

	VkFormat format = findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	m_pRenderpass->createRenderPass(format);

	mp_pipelines_manager->CreatePipelines();
	createDepthResources();
	createFramebuffer();
	allocateCommandBuffers();

	m_is_updated.set();
}

void Renderer::cleanSwapchain()
{
	vkDeviceWaitIdle(m_graphic.device);
	vkQueueWaitIdle(m_graphic.graphics_queue);
	vkQueueWaitIdle(m_graphic.present_queue);

	vkDestroyImageView(m_graphic.device, m_graphic.depth_view, nullptr);
	vkDestroyImage(m_graphic.device, m_graphic.depth_image, nullptr);
	vkFreeMemory(m_graphic.device, m_graphic.depth_memory, nullptr);

	for (size_t i = 0; i < m_graphic.framebuffers.size(); i++)
	{
		vkDestroyFramebuffer(m_graphic.device, m_graphic.framebuffers[i], nullptr);
	}

	
	vkFreeCommandBuffers(m_graphic.device, m_graphic.command_pool, static_cast<uint32_t>(m_graphic.command_buffers.size()), m_graphic.command_buffers.data());

	mp_pipelines_manager->DestroyPipelines();

	vkDestroyRenderPass(m_graphic.device, m_graphic.render_pass, nullptr);

	for (auto imageView : m_graphic.images_view)
	{
		vkDestroyImageView(m_graphic.device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_graphic.device, m_graphic.swapchain, nullptr);
}

bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, availableExtensions.data());

	std::set<std::string> requiredExtensions(m_graphic.extensions.device_extensions.begin(), m_graphic.extensions.device_extensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool Renderer::checkDeviceSuitability(VkPhysicalDevice device)
{
	bool swapChainAdequate = false;

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (!(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader))
	{
		return false;
	}

	QueueFamilyIndices indices = findQueueFamily(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	if (extensionsSupported)
	{
		SwapchainDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool Renderer::checkValidationLayerSupport()
{
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties> available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	/*std::cout << "available layer:" << std::endl;
	for (const auto& extension : available_layers)
	{
		std::cout << "\t" << extension.layerName << std::endl;
	}*/
	
	for (const char* layerName : m_graphic.extensions.validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : available_layers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_graphic.physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Renderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endCommands(commandBuffer);
}

void Renderer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = beginCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endCommands(commandBuffer);
}

VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_graphic.physical_device, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat Renderer::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool Renderer::hasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkCommandBuffer Renderer::beginCommands()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_graphic.command_pool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_graphic.device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Renderer::endCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_graphic.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphic.graphics_queue);

	vkFreeCommandBuffers(m_graphic.device, m_graphic.command_pool, 1, &commandBuffer);
}

void Renderer::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = beginCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (hasStencilComponent(format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else 
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;


	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	endCommands(commandBuffer);
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = { *WIDTH.get(), *HEIGHT.get() };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

SwapchainDetails Renderer::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapchainDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_graphic.surface, &details.capabilities);


	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, details.formats.data());
	}


	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.presentModes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, details.presentModes.data());
	}

	return details;
}

QueueFamilyIndices Renderer::findQueueFamily(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	int i = 0;
	for (const auto& queueFamily : queue_families)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphic_family = i;
		}

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_graphic.surface, &present_support);

		if (queueFamily.queueCount > 0 && present_support)
		{
			indices.present_family = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

std::vector<const char*> Renderer::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (m_graphic.validation_layer_enable)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj,
	size_t location, int32_t code, const char* layerPrefix, const char* msg,void* userData)
{

	std::cerr << "\nvalidation layer: " << msg << std::endl;
	Logger::registerError("\nvalidation layer: " + std::string(msg));

	return VK_FALSE;
}

VkResult Renderer::CreateDebugReportCallbackEXT(VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Renderer::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}
