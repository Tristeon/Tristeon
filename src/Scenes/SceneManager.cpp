#include "SceneManager.h"

#include <Scenes/Scene.h>
#include <Serialization/JsonSerializer.h>

#include "AssetManagement/AssetDatabase.h"
#include <Actors/Collector.h>
#include <Callbacks/IStart.h>
#include <Scenes/Layers/ActorLayer.h>
#include "Engine.h"
#include "Project.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;
	Delegate<Scene*> SceneManager::sceneLoaded;
	String SceneManager::cachedSceneName;
	json SceneManager::cachedSceneData;

	Scene* SceneManager::current()
	{
		return currentScene.get();
	}

	void SceneManager::load(String const name)
	{
		cachedSceneName = name;
	}

	void SceneManager::load(json const data)
	{
		cachedSceneData = data;
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

		json const data = scene->serialize();
		JsonSerializer::save(Project::assetPath() + filepath, data);

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
		json toLoad;
		String path;
		
		if (!cachedSceneData.empty())
		{
			toLoad = cachedSceneData;
			path = "";
		}
		else if (!cachedSceneName.empty())
		{
			path = AssetDatabase::findByName(cachedSceneName, ".scene");
			if (path.empty())
			{
				std::cout << "Couldn't find scene: " << cachedSceneName << std::endl;
				currentScene.reset();
				currentScene = std::make_unique<Scene>(); // load empty scene
				sceneLoaded.invoke(currentScene.get());
				return;
			}
			toLoad = JsonSerializer::load(Project::assetPath() + path);
		}
		else //Don't load a scene if there's nothing to load
		{
			return;
		}

		currentScene = std::make_unique<Scene>();
		currentScene->deserialize(toLoad);
		currentScene->name = cachedSceneName;
		currentScene->path = path;

		if (Engine::playMode())
			for (auto* start : Collector<IStart>::all()) start->start();

		sceneLoaded.invoke(currentScene.get());

		cachedSceneName = "";
		cachedSceneData = json();
	}

	void SceneManager::reset()
	{
		currentScene.reset();
	}
}
