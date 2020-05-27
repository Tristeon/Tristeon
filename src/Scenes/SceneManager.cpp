#include "SceneManager.h"

#include <Scenes/Scene.h>
#include <Serialization/JsonSerializer.h>

#include <AssetDatabase.h>
#include <Actors/Collector.h>
#include <Callbacks/IStart.h>
#include <Scenes/Layers/ActorLayer.h>
#include "Engine.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;
	Delegate<Scene*> SceneManager::sceneLoaded;
	String SceneManager::cachedSceneName;
	
	Scene* SceneManager::current()
	{
		return currentScene.get();
	}

	void SceneManager::load(String const name)
	{
		cachedSceneName = name;
	}

	void SceneManager::reload()
	{
		if (currentScene == nullptr)
			return;
		
		load(currentScene->name);
	}

	void SceneManager::save(Scene* scene, String const& filepath)
	{
		if (filepath.empty())
			throw std::invalid_argument("Filepath can't be empty!");
		
		json data = scene->serialize();
		JsonSerializer::save(filepath, data);

		AssetDatabase::add(filepath);
	}

	void SceneManager::saveCurrent()
	{
		if (current() == nullptr)
			return;

		if (current()->getPath().empty())
		{
			std::cout << "Current scene doesn't have a path therefore it can't be saved automatically. Use save(current(), filePath) instead" << std::endl;
			return;
		}
		save(current(), current()->getPath());
	}

	void SceneManager::destroyActor(Actor* actor)
	{
		Vector<ActorLayer*> actorLayers = current()->findLayersOfType<ActorLayer>();
		for (const auto& layer : actorLayers)
			layer->destroyActor(actor);
	}

	void SceneManager::processCachedLoad()
	{
		if (cachedSceneName.empty())
			return;
		
		String const path = AssetDatabase::findByName(cachedSceneName, "scene");
		if (path.empty())
		{
			std::cout << "Couldn't find scene: " << cachedSceneName << std::endl;
			currentScene.reset();
			currentScene = std::make_unique<Scene>(); // load empty scene
			sceneLoaded.invoke(currentScene.get());
			return;
		}

		currentScene = std::unique_ptr<Scene>(JsonSerializer::deserialize<Scene>(path));
		currentScene->name = cachedSceneName;
		currentScene->path = path;

		if (Engine::playMode())
			for (auto start : Collector<IStart>::all()) start->start();

		sceneLoaded.invoke(currentScene.get());

		cachedSceneName = "";
	}

	void SceneManager::reset()
	{
		currentScene.reset();
	}
}
