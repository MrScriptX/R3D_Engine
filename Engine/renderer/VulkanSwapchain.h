#ifndef R3DENGINE_VULKAN_SWAPCHAIN_H_
#define R3DENGINE_VULKAN_SWAPCHAIN_H_

#include <algorithm>
#include <stdexcept>

#include "../graphics/Config.h"
#include "../graphics/Graphics.h"

class VulkanSwapchain
{
  public:
	VulkanSwapchain(Graphics& m_graphic, uint32_t width, uint32_t height);
	~VulkanSwapchain();

	void createSwapchain();
	const uint32_t& GetHeigth();

  private:
	void createImagesView();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);

	uint32_t WIDTH;
	uint32_t HEIGHT;

	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_SWAPCHAIN_H_
