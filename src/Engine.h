#pragma once
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/Window.h>

namespace Tristeon
{
	class Engine final
	{
	public:
		explicit Engine();
		void run();

	private:
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<Window> window;
	};
}