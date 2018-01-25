#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <iostream>
#include <stdexcept>

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
	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	}
	void initVulkan() 
	{

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
		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	GLFWwindow * m_window;
};

int main()
{
	HelloTriangleApplication app;

	app.run();

	return 0;
}