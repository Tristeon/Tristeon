#include "SceneManager.h"
#include <Scenes/Scene.h>

#include "TileLayer.h"

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
	}

	void SceneManager::reset()
	{
		currentScene.reset();
	}
}
