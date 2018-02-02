#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>


class HelloTriangleApplication 
{
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void createInstance()
	{
		VkApplicationInfo info = {};//caract de l'app
		info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;//type of the app
		info.pApplicationName = "Hello Triangle";//name of app
		info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);//version of ???
		info.pEngineName = "No engine";//name of what???
		info.engineVersion = VK_MAKE_VERSION(1, 0, 0);//version of ???
		info.apiVersion = VK_API_VERSION_1_0;//version of vulkan api

		VkInstanceCreateInfo createInfo = {};//carac de l'instance Vk
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &info;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
	}

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	}
	void initVulkan() 
	{
		createInstance();
	}

	void mainLoop() 
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		vkDestroyInstance(m_instance, nullptr);

		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	VkInstance m_instance;
	GLFWwindow * m_window;
};

int main()
{
	HelloTriangleApplication app;
	
	app.run();

	return 0;
}