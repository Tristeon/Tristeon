#include "Renderer.h"
#include <Scenes/Scene.h>
#include "Scenes/Layers/Layer.h"

namespace Tristeon
{
	Renderer::Renderer(Engine* engine) : engine(engine)
	{
		spriteShader = new Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/Sprite.frag");
	}

	Renderer::~Renderer()
	{

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
