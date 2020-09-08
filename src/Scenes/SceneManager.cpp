#include "SceneManager.h"

#include <Scenes/Scene.h>
#include <Serialization/JsonSerializer.h>

#include "AssetManagement/AssetDatabase.h"
#include <Collectors/Collector.h>
#include <Callbacks/IStart.h>
#include <Scenes/Layers/ActorLayer.h>
#include "Engine.h"
#include "Project.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::_current = nullptr;
	Delegate<Scene*> SceneManager::sceneLoaded;
	String SceneManager::_cachedName;
	json SceneManager::_cachedData;

	Scene* SceneManager::current()
	{
		return _current.get();
	}

	void SceneManager::load(const String& name)
	{
		_cachedName = name;
	}

	void SceneManager::load(const json& data, const String& path)
	{
		_cachedData = data;
		_cachedData["path"] = path;
	}

	void SceneManager::reload()
	{
		if (_current == nullptr)
			return;

		load(_current->_name);
	}

	void SceneManager::save(Scene* scene, const String& filepath)
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

		if (current()->path().empty())
		{
			std::cout << "Current scene doesn't have a path therefore it can't be saved automatically. Use save(current(), filePath) instead" << std::endl;
			return;
		}
		save(current(), current()->path());
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
		
		if (!_cachedData.empty())
		{
			toLoad = _cachedData;
			path = _cachedData.value("path", "");
		}
		else if (!_cachedName.empty())
		{
			path = AssetDatabase::findByName(_cachedName, ".scene");
			if (path.empty())
			{
				std::cout << "Couldn't find scene: " << _cachedName << std::endl;
				_current.reset();
				_current = std::make_unique<Scene>(); // load empty scene
				sceneLoaded.invoke(_current.get());
				return;
			}
			toLoad = JsonSerializer::load(Project::assetPath() + path);
		}
		else //Don't load a scene if there's nothing to load
		{
			return;
		}

		_current = std::make_unique<Scene>();
		_current->deserialize(toLoad);
		_current->_name = _cachedName;
		_current->_path = path;

		if (Engine::playMode())
			for (auto* start : Collector<IStart>::all()) start->start();

		sceneLoaded.invoke(_current.get());

		_cachedName = "";
		_cachedData = json();
	}

	void SceneManager::reset()
	{
		_current.reset();
	}
}