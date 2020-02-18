#pragma once

namespace Tristeon
{
	class Engine;
	class Scene;
	class HUD;
	class Window;

	class Renderer
	{
		friend Engine;
		friend Window;
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