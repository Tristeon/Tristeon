#pragma once

namespace Tristeon
{
	class Engine;
	class Scene;
	class HUD;
	class GameView;

	class Renderer
	{
		friend Engine;
		friend GameView;
	public:
		Renderer(Engine* engine);
		~Renderer();

	private:
		void renderScene(Scene* scene);
		void renderHUD(HUD* hud);
		void initialize();

		Engine* engine = nullptr;
	};
}