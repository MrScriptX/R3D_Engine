#include "Application.h"

#include <assert.h>
#include <set>
#include <algorithm>

#define WIDTH 1260
#define HEIGHT 720


Application::Application()
{
}


Application::~Application()
{
}

void Application::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	clean();
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		updateUniformBuffer();
		drawFrame();
	}

	vkDeviceWaitIdle(m_device);
}

void Application::drawFrame()
{
	//update app state here

	vkQueueWaitIdle(m_presentQueue);//not needed if validation layer are disable

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(m_device, m_swapChain, std::numeric_limits<uint64_t>::max(), m_semaphore->getImageAvailable(), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire a swap chain image!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphore[] = { m_semaphore->getImageAvailable() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer.getCommandBuffer()[imageIndex];

	VkSemaphore signalSemaphore[] = { m_semaphore->getRenderFinished() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphore;

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphore;

	VkSwapchainKHR swapChains[] = { m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present a swap chain image!");
	}
}

void Application::clean()
{
	cleanSwapChain();

	m_sampler.reset();
	m_imageView.reset();

	m_textureImage.reset();

	m_descriptorPool.reset();
	m_descriptorSetLayout.reset();

	m_uniformBuffer.reset();
	m_buffer.reset();

	m_semaphore.reset();
	m_commandPool.reset();

	vkDestroyDevice(m_device, nullptr);
	destroyDebugReportCallbackEXT(m_instance, callback, nullptr);
	m_surface.reset();
	vkDestroyInstance(m_instance, nullptr);

	glfwDestroyWindow(m_window);

	glfwTerminate();
}





/**************************** Init  and clean fonctions ****************************************/
/***********************************************************************************************/

void Application::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetWindowSizeCallback(m_window, Application::onWindowResized);
}

void Application::initVulkan()
{
	createInstance();
	setupCallBack();
	m_surface = std::make_unique<VkSurface>(m_instance, *m_window);

	pickPhysicalDevice();
	createLogicalDevice();

	createSwapChain();
	createImageViews();

	m_renderPass = std::make_unique<RenderPass>(m_device, m_swapChainImageFormat, DepthRessources::findDepthFormat(m_physicalDevice));
	m_descriptorSetLayout = std::make_unique<DescriptorSetLayout>(m_device);
	m_pipeline = std::make_unique<Pipeline>(m_device, m_swapChainExtent, m_descriptorSetLayout->get(), m_renderPass->get());

	m_commandPool = std::make_unique<CommandPool>(m_device, m_physicalDevice, m_surface->getSurface());

	m_depthRessource = std::make_unique<DepthRessources>(m_swapChainExtent.width, m_swapChainExtent.height, m_device, m_physicalDevice, m_commandPool->get(), m_graphicsQueue);
	m_frameBuffer = std::make_unique<FrameBuffer>(m_device, m_renderPass->get(), m_swapChainImageViews, m_swapChainExtent, m_depthRessource->getImageView());

	m_textureImage = std::make_unique<Texture>(m_device, m_physicalDevice, m_graphicsQueue, m_commandPool->get());
	m_imageView = std::make_unique<TextureView>(m_device, m_textureImage->getTextureImage());
	m_sampler = std::make_unique<Sampler>(m_device);

	m_model.loadModel("model\\chalet.obj", "texture\\chalet.jpg");
	m_buffer = std::make_unique<Buffer>(m_device, m_commandPool->get(), m_graphicsQueue, m_physicalDevice, m_model.getVertex(), m_model.getIndex());
	m_uniformBuffer = std::make_unique<UniformBuffer>(m_device, m_physicalDevice);

	m_descriptorPool = std::make_unique<DescriptorPool>(m_device);
	m_descriptorSet = std::make_unique<DescriptorSet>(m_device, m_descriptorSetLayout->get(), m_descriptorPool->getDescriptor(), m_uniformBuffer->getBuffer(), m_imageView->getImageView(), m_sampler->getSampler());

	m_commandBuffer.allocateCommandBuffer(m_device, m_commandPool->get(), m_frameBuffer->getFrameBuffer());
	
	m_commandBuffer.beginCommandBuffer(m_renderPass->get(), m_pipeline->getPipeline(), m_buffer->getVertexBuffer(), m_buffer->getIndexBuffer(), m_pipeline->getPipelineLayout(), m_descriptorSet->get(), m_swapChainExtent, m_frameBuffer->getFrameBuffer(), m_model.getIndex());
	m_renderPass->beginRenderPass(m_commandBuffer.getCommandBuffer(), m_swapChainExtent, m_frameBuffer->getFrameBuffer());
	m_pipeline->bindPipeline(m_commandBuffer.getCommandBuffer());
	m_buffer->bindVertexBuffer(m_commandBuffer.getCommandBuffer());
	m_buffer->bindIndexBuffer(m_commandBuffer.getCommandBuffer());
	m_descriptorSet->bindDescriptorSet(m_commandBuffer.getCommandBuffer(), m_pipeline->getPipelineLayout());
	m_model.drawIndexed(m_commandBuffer.getCommandBuffer());
	m_renderPass->endRenderPass(m_commandBuffer.getCommandBuffer());
	m_commandBuffer.endCommandBuffer();

	m_semaphore = std::make_unique<Semaphore>(m_device);
}

void Application::cleanSwapChain()
{
	m_depthRessource.reset();

	m_frameBuffer.reset();

	m_commandBuffer.clean(m_device, m_commandPool->get());
	m_pipeline.reset();
	m_renderPass.reset();

	for (auto imageView : m_swapChainImageViews)
	{
		vkDestroyImageView(m_device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}





/**************************** Create fonctions ****************************************/
/**************************************************************************************/


void Application::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "App name";//name of app
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);//API version
	appInfo.pEngineName = "No engine";//name
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);//version of ???
	appInfo.apiVersion = VK_API_VERSION_1_0;//version of vulkan api

	VkInstanceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	info.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		info.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		info.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&info, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}

void Application::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface->getSurface());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentationFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t> (deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentationFamily, 0, &m_presentQueue);
}

void Application::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface->getSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice, m_surface->getSurface());
	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentationFamily };

	if (indices.graphicsFamily != indices.presentationFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain!");
	}
	
	vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;
}

void Application::recreateSwapChain()
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	if (width == 0 || height == 0)
	{
		return;
	}

	vkDeviceWaitIdle(m_device);

	cleanSwapChain();

	createSwapChain();
	createImageViews();

	m_renderPass = std::make_unique<RenderPass>(m_device, m_swapChainImageFormat, DepthRessources::findDepthFormat(m_physicalDevice));
	m_pipeline = std::make_unique<Pipeline>(m_device, m_swapChainExtent, m_descriptorSetLayout->get(), m_renderPass->get());
	m_depthRessource = std::make_unique<DepthRessources>(m_swapChainExtent.width, m_swapChainExtent.height, m_device, m_physicalDevice, m_commandPool->get(), m_graphicsQueue);
	m_frameBuffer = std::make_unique<FrameBuffer>(m_device, m_renderPass->get(), m_swapChainImageViews, m_swapChainExtent, m_depthRessource->getImageView());

	m_commandBuffer.allocateCommandBuffer(m_device, m_commandPool->get(), m_frameBuffer->getFrameBuffer());
	
	m_commandBuffer.beginCommandBuffer(m_renderPass->get(), m_pipeline->getPipeline(), m_buffer->getVertexBuffer(), m_buffer->getIndexBuffer(), m_pipeline->getPipelineLayout(), m_descriptorSet->get(), m_swapChainExtent, m_frameBuffer->getFrameBuffer(), m_model.getIndex());
	m_renderPass->beginRenderPass(m_commandBuffer.getCommandBuffer(), m_swapChainExtent, m_frameBuffer->getFrameBuffer());
	m_pipeline->bindPipeline(m_commandBuffer.getCommandBuffer());
	m_buffer->bindVertexBuffer(m_commandBuffer.getCommandBuffer());
	m_buffer->bindIndexBuffer(m_commandBuffer.getCommandBuffer());
	m_descriptorSet->bindDescriptorSet(m_commandBuffer.getCommandBuffer(), m_pipeline->getPipelineLayout());
	m_model.drawIndexed(m_commandBuffer.getCommandBuffer());
	m_renderPass->endRenderPass(m_commandBuffer.getCommandBuffer());
	m_commandBuffer.endCommandBuffer();
}

void Application::createImageViews()
{
	m_swapChainImageViews.resize(m_swapChainImages.size());

	for (size_t i = 0; i < m_swapChainImages.size(); i++)
	{
		m_swapChainImageViews[i] = TextureView::createImageView(m_device, m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}





/**************************** Init fonctions ****************************************/
/************************************************************************************/

void Application::setupCallBack()
{
	if (!enableValidationLayers) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	if (createDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug callback!");
	}
}

void Application::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	for (const VkPhysicalDevice& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

void Application::updateUniformBuffer()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));//(existing transform, rotation, axis to apply)
	ubo.view = glm::lookAt(glm::vec3(1.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));//(eye pos, center pos, up axis) 
	ubo.proj = glm::perspective(glm::radians(45.0f), (float)m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1f, 10.0f);//(view angle, apsect ratio, far plane, near plane)

	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(m_device, m_uniformBuffer->getBufferMemory(), 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(m_device, m_uniformBuffer->getBufferMemory());
}

bool Application::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool Application::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
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

bool Application::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device, m_surface->getSurface());

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

VkPresentModeKHR Application::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR Application::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

VkExtent2D Application::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

SwapChainSupportDetails Application::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface->getSurface(), &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->getSurface(), &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->getSurface(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->getSurface(), &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->getSurface(), &presentModeCount, details.presentModes.data());
	}

	return details;
}

std::vector<const char*> Application::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

void Application::onWindowResized(GLFWwindow* window, int width, int height)
{
	Application * app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->recreateSwapChain();
}

VkResult Application::createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
{
	PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Application::destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL Application::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}