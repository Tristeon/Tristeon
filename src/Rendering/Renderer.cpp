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

	void Renderer::renderScene()
	{
		Scene* scene = SceneManager::getCurrentScene();

		if (scene == nullptr)
			return;

		for (int i = 0; i < scene->getLayerCount(); i++)
		{
			Layer* layer = scene->getLayer(i);
			layer->render();
		}
	}

	void Renderer::renderHUD()
	{

	}
}