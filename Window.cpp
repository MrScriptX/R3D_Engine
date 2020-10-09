#include "Window.h"



Window::Window(std::shared_ptr<Config>& config, Player& player)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_handle = glfwCreateWindow(config->width, config->height, "Vulkan", nullptr, nullptr);

	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_handle, &player);
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
	Player* pPlayer = static_cast<Player*>(glfwGetWindowUserPointer(window));

	pPlayer->setInput(static_cast<int32_t>(key), static_cast<int32_t>(scancode), static_cast<int32_t>(mods), static_cast<int32_t>(action));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void Window::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	Player* pPlayer = static_cast<Player*>(glfwGetWindowUserPointer(window));

	pPlayer->updateRotation(xpos, ypos);
}

GLFWwindow & Window::getHandle()
{
	return *m_handle;
}