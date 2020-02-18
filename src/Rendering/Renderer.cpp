#include "Renderer.h"
#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>
#include <Scenes/Layer.h>

namespace Tristeon
{
	Renderer::Renderer(Engine* engine) : engine(engine)
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::initialize()
	{

	}

	void Renderer::renderScene(Scene* scene)
	{
		if (scene == nullptr)
			return;

		for (int i = 0; i < scene->getLayerCount(); i++)
		{
			Layer* layer = scene->getLayer(i);
			layer->render();
		}
	}

	void Renderer::renderHUD(HUD* hud)
	{
		//TODO: HUD rendering
	}
}