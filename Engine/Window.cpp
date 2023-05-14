#include "Window.h"

Window::Window(const vred::settings& setting, Controller& controller)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_handle = glfwCreateWindow(setting.window_width, setting.window_height, setting.app_name.c_str(), nullptr, nullptr);

	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(m_handle, &controller);
	glfwSetKeyCallback(m_handle, key_callback);
	glfwSetCursorPosCallback(m_handle, mouse_callback);
	glfwSetCursorPos(m_handle, 0.0, 0.0);
}

Window::~Window()
{
	glfwDestroyWindow(m_handle);
	glfwTerminate();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Controller* pController = static_cast<Controller*>(glfwGetWindowUserPointer(window));

	pController->setInput(static_cast<int32_t>(key), static_cast<int32_t>(scancode), static_cast<int32_t>(mods), static_cast<int32_t>(action));

	if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (pController->IsMouseLock())
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			pController->SetMouseState(false);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			pController->SetMouseState(true);
			glfwSetCursorPos(window, pController->GetLastMousePosX(), pController->GetLastMousePosY());
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Controller* pController = static_cast<Controller*>(glfwGetWindowUserPointer(window));

	if (pController->IsMouseLock())
	{
		pController->updateRotation(xpos, ypos);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(xpos, ypos);
	}
}

GLFWwindow& Window::getHandle()
{
	return *m_handle;
}
