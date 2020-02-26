#include "SceneManager.h"
#include <Scenes/Scene.h>

#include "Layers/TileLayer.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;
	
	Scene* SceneManager::getCurrentScene()
	{
		//TODO: SceneManager implementation
		return currentScene.get();
	}

	void SceneManager::initialize()
	{
		currentScene = std::make_unique<Scene>();
		currentScene->layers.push_back(std::make_unique<TileLayer>());
		currentScene->camera = std::make_unique<Camera>();
		currentScene->camera->zoom = 1.0f;
		currentScene->camera->size = Vector2Int(800, 600);
	}

	void SceneManager::reset()
	{
		currentScene.reset();
	}
}
