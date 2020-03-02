#include "Renderer.h"
#include <Scenes/Scene.h>
#include "Scenes/Layers/Layer.h"

namespace Tristeon
{
	Renderer::Renderer(Engine* engine) : engine(engine)
	{
		spriteShader = std::make_unique<Shader>("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
	}

	void Renderer::renderScene(Scene* scene)
	{
		if (scene == nullptr)
			return;

		for (unsigned int i = 0; i < scene->getLayerCount(); i++)
		{
			Layer* layer = scene->getLayer(i);
			layer->render(this, scene);
		}
	}

	void Renderer::renderHUD(HUD* hud)
	{
		//TODO: HUD rendering
	}
}
