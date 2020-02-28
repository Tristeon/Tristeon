#pragma once
#include "Shader.h"

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

		Shader* getSpriteShader() const { return spriteShader; }
	private:
		void renderScene(Scene* scene);
		void renderHUD(HUD* hud);

		Engine* engine = nullptr;
		Shader* spriteShader = nullptr;
	};
}
