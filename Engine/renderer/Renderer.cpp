#include "Renderer.h"

#include <set>

#ifndef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#endif // !STB_IMAGE_IMPLEMENTATION

// review createBuffer func
#include "parameters.h"
#include "vinstance.h"
#include "vdevices.h"
#include "vsurface.h"
#include "vsync_obj.h"
#include "vswapchain.h"
#include "vrenderpass.h"
#include "vimage.h"
#include "vframebuffer.h"
#include "vcommandbuffer.h"
#include "vdescriptorset.h"
#include "callback.h"
#include "vbuffer.h"

Renderer::Renderer(GLFWwindow& window, uint32_t width, uint32_t height) : m_is_updated({ false })
{
	// setupInstance(window);
	m_interface.instance = vred::renderer::create_instance();
	setup_debug_callback(m_interface.instance);
	m_interface.surface = vred::renderer::create_surface(window, m_interface.instance);

	// setupDevice();
	m_interface.physical_device = vred::renderer::choose_device(m_interface);
	vred::renderer::create_device_interface(m_interface);

	// createSyncObject();
	for (vred::renderer::frame& f: m_frames)
	{
		f.render_fence = vred::renderer::create_fence(m_interface.device);
		f.render_finished = vred::renderer::create_semaphore(m_interface.device);
		f.image_available = vred::renderer::create_semaphore(m_interface.device);
	}

	// setup swapchain
	create_swapchain({ width, height });

	std::vector<VkDescriptorSetLayoutBinding> main_descriptor_bindings(2);
	main_descriptor_bindings[0] = vred::renderer::create_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1, 0);
	main_descriptor_bindings[1] = vred::renderer::create_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 1);
	
	m_render_objects.main_descriptor_set_layout = vred::renderer::create_descriptor_set_layout(m_interface.device, main_descriptor_bindings);

	VkDescriptorSetLayoutBinding light_ubo_binding = vred::renderer::create_descriptor_set_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1, 0);
	m_render_objects.light_descriptor_set_layout = vred::renderer::create_descriptor_set_layout(m_interface.device, { light_ubo_binding });

	std::vector<VkDescriptorPoolSize> pool_sizes(2);
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = 10;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = 10;

	m_render_objects.main_pool = vred::renderer::create_descriptor_pool(m_interface.device, pool_sizes);

	pool_sizes.resize(11);
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_SAMPLER;
	pool_sizes[0].descriptorCount = 1000;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = 1000;
	pool_sizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	pool_sizes[2].descriptorCount = 1000;
	pool_sizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	pool_sizes[3].descriptorCount = 1000;
	pool_sizes[4].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	pool_sizes[4].descriptorCount = 1000;
	pool_sizes[5].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	pool_sizes[5].descriptorCount = 1000;
	pool_sizes[6].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[6].descriptorCount = 1000;
	pool_sizes[7].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	pool_sizes[7].descriptorCount = 1000;
	pool_sizes[8].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	pool_sizes[8].descriptorCount = 1000;
	pool_sizes[9].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	pool_sizes[9].descriptorCount = 1000;
	pool_sizes[10].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	pool_sizes[10].descriptorCount = 1000;

	m_render_objects.ui_pool = vred::renderer::create_descriptor_pool(m_interface.device, pool_sizes);

	initUI(window);

	mp_pipelines_manager = std::make_unique<VulkanPipeline>(m_interface, m_swapchain, m_render_objects);
	m_pBufferFactory = std::make_unique<VulkanBuffer>(m_interface);
}

Renderer::~Renderer()
{
	// destroy ImGui objects
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// clean main descriptor
	vkDestroyDescriptorPool(m_interface.device, m_render_objects.main_pool, nullptr);
	vkDestroyDescriptorSetLayout(m_interface.device, m_render_objects.main_descriptor_set_layout, nullptr);

	// clean ImGui descriptor
	vkDestroyDescriptorPool(m_interface.device, m_render_objects.ui_pool, nullptr);
	vkDestroyDescriptorSetLayout(m_interface.device, m_render_objects.light_descriptor_set_layout, nullptr);

	for (const vred::renderer::frame& f: m_frames)
	{
		vkDestroySemaphore(m_interface.device, f.image_available, nullptr);
		vkDestroySemaphore(m_interface.device, f.render_finished, nullptr);
		vkDestroyFence(m_interface.device, f.render_fence, nullptr);
	}

	vkDestroyDevice(m_interface.device, nullptr);

	if (vred::renderer::parameters::validation_layer_enable)
		vred::renderer::destroy_debug_report_callback(m_interface.instance, callback, nullptr);

	vkDestroySurfaceKHR(m_interface.instance, m_interface.surface, nullptr);
	vkDestroyInstance(m_interface.instance, nullptr);
}

void Renderer::begin_render(size_t frame)
{
	const VkCommandBuffer& current_command_buffer = m_frames[frame].main_command_buffer;

	// reset command buffer
	if (vkResetCommandBuffer(current_command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS)
		throw std::runtime_error("Failed to reset command buffer!");

	// begin command buffer
	VkCommandBufferBeginInfo begin_buffer_info = {};
	begin_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_buffer_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	begin_buffer_info.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(current_command_buffer, &begin_buffer_info) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

	// begin render pass
	VkRenderPassBeginInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = m_swapchain.main_render_pass;
	render_pass_info.framebuffer = m_frames[frame].main_framebuffer;

	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = m_swapchain.extent;

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(current_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::end_render(size_t frame)
{
	const VkCommandBuffer& current_command_buffer = m_frames[frame].main_command_buffer;

	vkCmdEndRenderPass(current_command_buffer);

	if (vkEndCommandBuffer(current_command_buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer!");
}

void Renderer::reset(VkExtent2D window_size)
{
	clean_swapchain();
	create_swapchain(window_size);

	m_is_updated.set();
}

int32_t Renderer::draw()
{
	if (vred::renderer::parameters::validation_layer_enable)
		vkQueueWaitIdle(m_interface.present_queue);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	const VkSemaphore wait_semaphores[] = { m_frames[m_last_image].image_available };
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;

	constexpr VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submit_info.pWaitDstStageMask = wait_stages;

	const std::array<VkCommandBuffer, 2> command_buffers = { m_frames[m_current_image].main_command_buffer, m_frames[m_current_image].ui_command_buffer };
	submit_info.commandBufferCount = command_buffers.size();
	submit_info.pCommandBuffers = command_buffers.data();

	const VkSemaphore signal_semaphores[] = { m_frames[m_last_image].render_finished };

	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphores;

	vkResetFences(m_interface.device, 1, &m_frames[m_current_image].render_fence);

	VkResult result = vkQueueSubmit(m_interface.graphics_queue, 1, &submit_info, m_frames[m_current_image].render_fence);
	if (result != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;

	const VkSwapchainKHR swapchains[] = { m_swapchain.handle };
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapchains;
	present_info.pImageIndices = &m_current_image;
	present_info.pResults = nullptr;

	result = vkQueuePresentKHR(m_interface.present_queue, &present_info);

	int return_code = 0;
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		return_code = 1;
	else if (result != VK_SUCCESS)
		throw std::runtime_error("failed to present swap chain image!");

	return return_code;
}

void Renderer::UpdateUI()
{
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {};
	cmd_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmd_buffer_begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(m_frames[m_current_image].ui_command_buffer, &cmd_buffer_begin_info) != VK_SUCCESS)
		throw std::runtime_error("Unable to start recording UI command buffer!");

	const VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	VkRenderPassBeginInfo render_pass_begin_info = {};
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.renderPass = m_swapchain.ui_render_pass;
	render_pass_begin_info.framebuffer = m_frames[m_current_image].ui_framebuffer;
	render_pass_begin_info.renderArea.extent.width = m_swapchain.extent.width;
	render_pass_begin_info.renderArea.extent.height = m_swapchain.extent.height;
	render_pass_begin_info.clearValueCount = 1;
	render_pass_begin_info.pClearValues = &clear_color;

	vkCmdBeginRenderPass(m_frames[m_current_image].ui_command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	// Grab and record the draw data for Dear Imgui
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_frames[m_current_image].ui_command_buffer);

	// End and submit render pass
	vkCmdEndRenderPass(m_frames[m_current_image].ui_command_buffer);

	if (vkEndCommandBuffer(m_frames[m_current_image].ui_command_buffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to record command buffers!");
}

int32_t Renderer::AcquireNextImage()
{
	m_last_image = m_current_image;

	 const VkResult result = vkAcquireNextImageKHR(m_interface.device, m_swapchain.handle, std::numeric_limits<uint64_t>::max(), m_frames[m_current_image].image_available, VK_NULL_HANDLE, &m_current_image);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
		return -1;
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to acquire swapchain image!");

	return m_current_image;
}

void Renderer::WaitForSwapchainImageFence()
{
	vkWaitForFences(m_interface.device, 1, &m_frames[m_current_image].render_fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
}

void Renderer::SetPolygonFillingMode(const VkPolygonMode& mode)
{
	m_render_objects.polygone_mode = mode;
}

void Renderer::SetColorMode(const ColorMode map)
{
	m_render_objects.color_map = map;
}

const VkDevice& Renderer::GetDevice()
{
	return m_interface.device;
}

std::vector<VkDescriptorSetLayout> Renderer::get_descriptor_set_layout() const
{
	return { m_render_objects.main_descriptor_set_layout, m_render_objects.light_descriptor_set_layout };
}

const vred::renderer::iswapchain& Renderer::get_swapchain() const
{
	return m_swapchain;
}

VkFramebuffer& Renderer::GetFrameBuffer(const size_t& i)
{
	return m_frames[i].main_framebuffer;
}

VkCommandBuffer& Renderer::GetCommandBuffer(const size_t& i)
{
	return m_frames[i].main_command_buffer;
}

std::unique_ptr<VulkanBuffer>& Renderer::getBufferFactory()
{
	return m_pBufferFactory;
}

std::unique_ptr<VulkanPipeline>& Renderer::GetPipelineFactory()
{
	return mp_pipelines_manager;
}

bool Renderer::IsUpdated() const
{
	return m_is_updated == false;
}

bool Renderer::NeedUpdate(const size_t& i) const
{
	return m_is_updated[i];
}

void Renderer::SetUpdated(const size_t& i)
{
	m_is_updated[i] = false;
}

void Renderer::update_all()
{
	m_is_updated.set();
}

void Renderer::destroyBuffers(Buffer& buffer)
{
	vkQueueWaitIdle(m_interface.graphics_queue);

	for (uint32_t i = 0; i < m_frames.size(); i++)
	{
		vkResetCommandBuffer(m_frames[i].main_command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

	vkDestroyBuffer(m_interface.device, buffer.index, nullptr);
	vkFreeMemory(m_interface.device, buffer.index_memory, nullptr);

	vkDestroyBuffer(m_interface.device, buffer.vertex, nullptr);
	vkFreeMemory(m_interface.device, buffer.vertex_memory, nullptr);
}

void Renderer::CreateVerticesBuffer(std::shared_ptr<std::vector<Vertex>> vertices, Buffer& buffer)
{
	VkDeviceSize buffer_size = sizeof(vertices->at(0)) * vertices->size();

	VkBuffer staging_buffer = vred::renderer::create_buffer(m_interface, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory staging_buffer_memory = vred::renderer::allocate_buffer(m_interface, staging_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(m_interface.device, staging_buffer_memory, 0, buffer_size, 0, &data);
	memcpy(data, vertices->data(), (size_t)buffer_size);
	vkUnmapMemory(m_interface.device, staging_buffer_memory);

	buffer.vertex = vred::renderer::create_buffer(m_interface, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	buffer.vertex_memory = vred::renderer::allocate_buffer(m_interface, buffer.vertex, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	copyBuffer(staging_buffer, buffer.vertex, buffer_size);

	vkDestroyBuffer(m_interface.device, staging_buffer, nullptr);
	vkFreeMemory(m_interface.device, staging_buffer_memory, nullptr);
}

void Renderer::CreateIndicesBuffer(std::shared_ptr<std::vector<uint32_t>> indices, Buffer& buffer)
{
	VkDeviceSize buffer_size = sizeof(indices->at(0)) * indices->size();

	VkBuffer staging_buffer;
	VkDeviceMemory staging_buffer_mem;
	m_pBufferFactory->createBuffer(staging_buffer, staging_buffer_mem, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(m_interface.device, staging_buffer_mem, 0, buffer_size, 0, &data);
	memcpy(data, indices->data(), (size_t)buffer_size);
	vkUnmapMemory(m_interface.device, staging_buffer_mem);

	m_pBufferFactory->createBuffer(buffer.index, buffer.index_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	copyBuffer(staging_buffer, buffer.index, buffer_size);

	vkDestroyBuffer(m_interface.device, staging_buffer, nullptr);
	vkFreeMemory(m_interface.device, staging_buffer_mem, nullptr);
}

void Renderer::CreateUniformBuffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size)
{
	m_pBufferFactory->createBuffer(buffer, memory, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void Renderer::setup_debug_callback(VkInstance& instance)
{
	if constexpr (!vred::renderer::parameters::validation_layer_enable)
		return;

	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	create_info.pfnCallback = vred::renderer::debug_callback;

	if (vred::renderer::create_debug_report_callback(instance, &create_info, nullptr, &callback) != VK_SUCCESS)
		throw std::runtime_error("Failed to setup debug callback");
}

void Renderer::create_swapchain(const VkExtent2D& extent)
{
	// create swapchain
	m_swapchain = vred::renderer::create_swapchain(m_interface, extent);
	m_swapchain.images = vred::renderer::create_swapchain_images(m_interface.device, m_swapchain.handle);
	m_swapchain.images_view = vred::renderer::create_swapchain_views(m_interface.device, m_swapchain);

	// create depth ressources
	VkExtent3D depth_extent = {};
	depth_extent.width = m_swapchain.extent.width;
	depth_extent.height = m_swapchain.extent.height;
	depth_extent.depth = 1;

	VkFormat depth_format = findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	m_swapchain.depth_image = vred::renderer::create_image(m_interface.device, depth_extent, depth_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
	m_swapchain.depth_memory = vred::renderer::create_image_memory(m_interface.device, m_interface.physical_device, m_swapchain.depth_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	m_swapchain.depth_image_view = vred::renderer::create_image_view(m_interface.device, m_swapchain.depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);

	// create renderpass
	m_swapchain.main_render_pass = vred::renderer::create_renderpass(m_swapchain.format, depth_format, m_interface.device);
	m_swapchain.ui_render_pass = vred::renderer::create_renderpass(m_swapchain.format, m_interface.device);

	// create framebuffer
	for (size_t i = 0; i < m_swapchain.images_view.size(); ++i)
	{
		const std::vector<VkImageView> attachements = { m_swapchain.images_view[i], m_swapchain.depth_image_view };
		m_frames[i].main_framebuffer = vred::renderer::create_framebuffer(m_interface.device, m_swapchain.main_render_pass, attachements, m_swapchain.extent);
		m_frames[i].ui_framebuffer = vred::renderer::create_framebuffer(m_interface.device, m_swapchain.ui_render_pass, { attachements[0] }, m_swapchain.extent);
	}

	// create commandpool
	m_swapchain.main_command_pool = vred::renderer::create_command_pool(m_interface.device, m_interface.queue_indices.graphic_family);
	m_swapchain.ui_command_pool = vred::renderer::create_command_pool(m_interface.device, m_interface.queue_indices.graphic_family);

	// allocate commandbuffers
	std::vector<VkCommandBuffer> main_command_buffers = vred::renderer::allocate_command_buffers(m_frames.size(), m_interface.device, m_swapchain.main_command_pool);
	std::vector<VkCommandBuffer> ui_command_buffers = vred::renderer::allocate_command_buffers(m_frames.size(), m_interface.device, m_swapchain.ui_command_pool);
	for (size_t i = 0; i < m_frames.size(); ++i)
	{
		m_frames[i].main_command_buffer = main_command_buffers[i];
		m_frames[i].ui_command_buffer = ui_command_buffers[i];
	}
}

void Renderer::allocateDescriptorSet(VkDescriptorSet& descriptor_set)
{
	VkDescriptorSetLayout layouts[] = { m_render_objects.main_descriptor_set_layout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_render_objects.main_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	VkResult result = vkAllocateDescriptorSets(m_interface.device, &allocInfo, &descriptor_set);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor set!");
	}
}

void Renderer::allocateDescriptorSetLight(VkDescriptorSet& descriptor_set)
{
	VkDescriptorSetLayout layouts[] = { m_render_objects.light_descriptor_set_layout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_render_objects.main_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	VkResult result = vkAllocateDescriptorSets(m_interface.device, &allocInfo, &descriptor_set);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor set!");
	}
}

void Renderer::updateDescriptorSet(const VkBuffer& ubo, const VkDescriptorSet& descriptor_set, VkDeviceSize size)
{
	std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = ubo;
	bufferInfo.offset = 0;
	bufferInfo.range = size;

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptor_set;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(m_interface.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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

	vkUpdateDescriptorSets(m_interface.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void Renderer::destroyUniformBuffer()
{
	vkDestroyBuffer(m_interface.device, m_render_objects.uniform_buffer, nullptr);
	vkFreeMemory(m_interface.device, m_render_objects.uniform_memory, nullptr);
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

	if (vkCreateImageView(m_interface.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void Renderer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
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

	if (vkCreateImage(m_interface.device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_interface.device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(m_interface.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(m_interface.device, image, imageMemory, 0);
}

void Renderer::initUI(GLFWwindow& window)
{
	std::cout << "Init ImGui for Vulkan..." << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan(&window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_interface.instance;
	init_info.PhysicalDevice = m_interface.physical_device;
	init_info.Device = m_interface.device;
	init_info.QueueFamily = m_interface.queue_indices.graphic_family;
	init_info.Queue = m_interface.graphics_queue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_render_objects.ui_pool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = m_swapchain.images.size();
	init_info.ImageCount = m_frames.size();
	init_info.CheckVkResultFn = nullptr;
	ImGui_ImplVulkan_Init(&init_info, m_swapchain.ui_render_pass);

	// upload textures to gpu
	VkCommandBuffer command_buffer = beginCommands();
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
	endCommands(command_buffer);
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void Renderer::clean_swapchain()
{
	vkDeviceWaitIdle(m_interface.device);
	vkQueueWaitIdle(m_interface.graphics_queue);
	vkQueueWaitIdle(m_interface.present_queue);

	vkDestroyImageView(m_interface.device, m_swapchain.depth_image_view, nullptr);
	vkDestroyImage(m_interface.device, m_swapchain.depth_image, nullptr);
	vkFreeMemory(m_interface.device, m_swapchain.depth_memory, nullptr);

	for (size_t i = 0; i < m_frames.size(); i++)
	{
		vkDestroyFramebuffer(m_interface.device, m_frames[i].main_framebuffer, nullptr);
		vkDestroyFramebuffer(m_interface.device, m_frames[i].ui_framebuffer, nullptr);
	}

	vkDestroyCommandPool(m_interface.device, m_swapchain.main_command_pool, nullptr);
	vkDestroyCommandPool(m_interface.device, m_swapchain.ui_command_pool, nullptr);

	vkDestroyRenderPass(m_interface.device, m_swapchain.main_render_pass, nullptr);
	vkDestroyRenderPass(m_interface.device, m_swapchain.ui_render_pass, nullptr);

	for (const auto& image_view : m_swapchain.images_view)
	{
		vkDestroyImageView(m_interface.device, image_view, nullptr);
	}

	vkDestroySwapchainKHR(m_interface.device, m_swapchain.handle, nullptr);
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_interface.physical_device, &memProperties);

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
	VkCommandBuffer commandBuffer = beginCommands(); // not optimized, should use command buffer of frame instead of new one

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
		vkGetPhysicalDeviceFormatProperties(m_interface.physical_device, format, &props);

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
	return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL,
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
	allocInfo.commandPool = m_swapchain.main_command_pool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_interface.device, &allocInfo, &commandBuffer);

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

	vkQueueSubmit(m_interface.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_interface.graphics_queue);

	vkFreeCommandBuffers(m_interface.device, m_swapchain.main_command_pool, 1, &commandBuffer);
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
