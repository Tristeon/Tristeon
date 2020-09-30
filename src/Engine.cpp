#include "Engine.h"

#include <chrono>
#include <Scenes/SceneManager.h>
#include <Window.h>

#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Input/Gamepad.h>

#include <Callbacks/IEarlyUpdate.h>
#include <Callbacks/IFixedUpdate.h>
#include <Callbacks/ILateUpdate.h>
#include <Callbacks/IDrawGizmos.h>
#include <Callbacks/IUpdate.h>

#include "Scenes/Actors/Actor.h"
#include "Project.h"
#include "AssetManagement/AssetDatabase.h"
#include "Scenes/Scene.h"
#include "Utils/Time.h"

namespace Tristeon
{
	void Engine::run()
	{
		Console::init();
		Window::pollEvents();

#ifndef TRISTEON_EDITOR
		Project::loadBuild();
		AssetDatabase::load();
#endif

		_renderer = std::make_unique<Renderer>();
		_physics = std::make_unique<PhysicsWorld>();
		
		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::sceneLoaded += [=](Scene* scene)
		{
			_playModeDirty = true;
		};

		SceneManager::load(Project::firstSceneName());
		SceneManager::processCachedLoad();
		
		if (SceneManager::current() == nullptr) //Create an empty scene with firstSceneName()
		{
			Scene empty;
			SceneManager::save(&empty, Project::firstSceneName() + ".scene");
			SceneManager::load(Project::firstSceneName());
		}
		
		SceneManager::current()->safeCleanup();

		auto lastTime = std::chrono::high_resolution_clock::now();
		unsigned int frames = 0;
		float time = 0;
		float fixedUpdateTime = 0;

		while (!Window::closingDown())
		{
			Window::pollEvents();

			//Keep track of elapsed time to calculate deltaTime (used in gameplay/physics systems to account for FPS differences)
			auto now = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime);
			float const deltaTime = duration.count() / 1000.0f; //Convert to ms
			Time::_deltaTime = deltaTime;
			lastTime = now;

			//FPS counter
			frames++;
			time += deltaTime;
			if (time >= 1000)
			{
				Time::_fps = frames;
				frames = 0;
				time = fmod(time, 1000.0f);
			}

			if (_playMode)
			{
				if (_playModeDirty)
				{
					for (auto* start : Collector<IStart>::all()) start->start();
					_playModeDirty = false;
					SceneManager::current()->safeCleanup();
				}
				
				fixedUpdateTime += deltaTime;
				while (fixedUpdateTime > Project::Physics::fixedDeltaTime()) 
				{
					_physics->update();
					SceneManager::current()->safeCleanup();
					for (auto* fixed : Collector<IFixedUpdate>::all()) fixed->fixedUpdate();
					SceneManager::current()->safeCleanup();
					fixedUpdateTime -= Project::Physics::fixedDeltaTime();
				}

				for (auto* early : Collector<IEarlyUpdate>::all()) early->earlyUpdate();
				SceneManager::current()->safeCleanup();
				for (auto* update : Collector<IUpdate>::all()) update->update();
				SceneManager::current()->safeCleanup();
				for (auto* late : Collector<ILateUpdate>::all()) late->lateUpdate();
				SceneManager::current()->safeCleanup();
			}

			for (auto* gizmos : Collector<IDrawGizmos>::all()) gizmos->drawGizmos();

			SceneManager::current()->safeCleanup();
			
			Window::draw();

			Mouse::reset();
			Keyboard::reset();
			Gamepad::reset();

			SceneManager::processCachedLoad();
		}
	}

	void Engine::setPlayMode(const bool& enabled)
	{
		instance()->_playMode = enabled;
		instance()->_playModeDirty = true;
	}

	bool Engine::playMode()
	{
		return instance()->_playMode;
	}
}