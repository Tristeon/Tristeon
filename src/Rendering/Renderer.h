#pragma once
#include "Shader.h"

namespace Tristeon
{
	class Engine;
	class Scene;
	class HUD;
	class GameView;

	class Renderer final
	{
		friend Engine;
		friend GameView;
	public:
		explicit Renderer(Engine* engine);

		Shader* getSpriteShader() const { return spriteShader.get(); }
	private:
		void renderScene(Scene* scene);
		void renderHUD(HUD* hud);

		Engine* engine = nullptr;
		std::unique_ptr<Shader> spriteShader = nullptr;
	};
}
