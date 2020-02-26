#pragma once
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/GameView.h>

namespace Tristeon
{
	class Engine
	{
		friend GameView;
		friend Renderer;

	public:
		explicit Engine();
		~Engine();

		void run();
		void setWindow(GameView* window);
	private:
		GameView* view = nullptr;
		std::unique_ptr<Renderer> renderer;
	};
}