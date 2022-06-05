#pragma once

#ifndef _GLFW3_
#define _GLFW3_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#endif // !_GLFW3_

#include <memory>
#include <imgui.h>

#include "cameras/Controller.h"
#include "graphics/Config.h"

class Window
{
  public:
	Window(std::shared_ptr<Config>& config, Controller& controller);
	~Window();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	GLFWwindow& getHandle();

  private:
	GLFWwindow* m_handle;
};
