#include "Window.h"

#include "Engine.h"
#include <GLFW/glfw3.h>

namespace Tristeon
{
	Window::Window(Engine* engine) : engine(engine)
	{
		glfwSetErrorCallback([](int const error, const char* description) { 
			/* TODO: implement glfw error callback */
		});

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, true);

		//Get requested window size
		GLFWmonitor* monitor = nullptr;
		monitor = glfwGetPrimaryMonitor();
		glfwGetMonitorPhysicalSize(monitor, &width, &height);

		window = glfwCreateWindow(width, height, "Tristeon", nullptr, nullptr);

		if (window == nullptr)
			throw std::exception("Failed to open GLFW Window");

		glfwMaximizeWindow(window);
		glfwSetWindowUserPointer(window, this);
		glfwMakeContextCurrent(window);

		glfwSetWindowSizeCallback(window, [](GLFWwindow * w, int const width, int const height) { 
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->onResize(width, height); 
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::onResize(int w, int h)
	{
		width = w;
		height = h;
	}
}