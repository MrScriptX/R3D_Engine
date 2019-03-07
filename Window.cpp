#include "Window.h"



Window::Window(std::unique_ptr<Config>& config, Camera& camera)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_handle = glfwCreateWindow(config->width, config->height, "Vulkan", nullptr, nullptr);

	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_handle, &camera);
	glfwSetKeyCallback(m_handle, key_callback);
	glfwSetCursorPosCallback(m_handle, mouse_callback);
}


Window::~Window()
{
	glfwDestroyWindow(m_handle);
	glfwTerminate();
}

void Window::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Camera* pCamera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	pCamera->setInput(static_cast<int32_t>(key), static_cast<int32_t>(scancode), static_cast<int32_t>(mods), static_cast<int32_t>(action));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void Window::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	Camera* pCamera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	pCamera->mouse(xpos, ypos);
}

GLFWwindow & Window::getHandle()
{
	return *m_handle;
}