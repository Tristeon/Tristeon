#pragma once
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/GameView.h>

#include "TypeDefinitions.h"

namespace Tristeon
{
	class Engine
	{
		friend GameView;
		friend Renderer;

	public:
		Engine();
		~Engine();

		static Engine* instance() { return _instance; }
		void initialize();
		void setGameView(GameView* gameView);
		GameView* gameView() const { return view; }
	private:
		GameView* view = nullptr;
		Unique<Renderer> renderer;
		static Engine* _instance;
	};
}
