#pragma once
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/Window.h>

namespace Tristeon
{
	class Engine
	{
		friend Window;
		friend Renderer;

	public:
		explicit Engine();
		~Engine();

		void run();
		void update();
	private:
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<Window> window;
	};
}