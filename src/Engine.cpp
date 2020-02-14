#include "Engine.h"
#include <GLFW/glfw3.h>

namespace Tristeon
{
	Tristeon::Engine::Engine()
	{
		window = std::make_unique<Window>(this);
		renderer = std::make_unique<Renderer>(this);
	}

	void Engine::run()
	{
		do
		{
			glfwPollEvents();

			renderer->renderScene();
			renderer->renderHUD();
		} while (!glfwWindowShouldClose(window->getGLFWwindow()));
	}
}
