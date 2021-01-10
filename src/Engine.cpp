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
#include "Settings.h"
#include "AssetManagement/AssetDatabase.h"
#include "Scenes/Scene.h"
#include "Utils/Time.h"

#include <thread>

using namespace std::chrono;

namespace Tristeon
{
	void Engine::run()
	{
		Window::pollEvents();

		_renderer = std::make_unique<Renderer>();
		_physics = std::make_unique<PhysicsWorld>();
		_audio = std::make_unique<Audio>();
		
		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::sceneLoaded += [=](Scene* scene)
		{
			_playModeDirty = true;
		};

		SceneManager::load(Settings::firstSceneName());
		SceneManager::processCachedLoad();
		
		if (SceneManager::current() == nullptr) //Create an empty scene with firstSceneName()
		{
			Scene empty;
			SceneManager::save(&empty, Settings::firstSceneName() + ".scene");
			SceneManager::load(Settings::firstSceneName());
		}
		
		SceneManager::current()->safeCleanup();

		auto lastTime = high_resolution_clock::now();
		unsigned int frames = 0;
		float time = 0;
		float fixedUpdateTime = 0;

		while (!Window::closingDown())
		{
			//Keep track of elapsed time to calculate deltaTime (used in gameplay/physics systems to account for FPS differences)
			auto now = high_resolution_clock::now();
			microseconds duration = std::chrono::duration_cast<microseconds>(now - lastTime);
			lastTime = now;
			float deltaTime = (float)duration.count() / 1000.0f; //Convert to ms

			//Account for fps limit
			if (Settings::Graphics::maxFPS() != 0)
			{
				microseconds targetMicroSeconds = 1000000us / Settings::Graphics::maxFPS();
				if (duration < targetMicroSeconds)
				{
					//Sleep if we're running too fast
					microseconds sleepTime{ (targetMicroSeconds - duration) };
					std::this_thread::sleep_for(sleepTime);

					deltaTime = 1000.0f / (float)Settings::Graphics::maxFPS();
					lastTime = high_resolution_clock::now();
				}
			}

			//FPS counter
			frames++;
			time += deltaTime;
			Time::_deltaTime = deltaTime;
			if (time >= 1000)
			{
				Time::_fps = frames;
				frames = 0;
				time = fmod(time, 1000.0f);
			}

			//Poll input & window changes
			Window::pollEvents();
			
			if (_playMode)
			{
				if (_playModeDirty)
				{
					for (auto* start : Collector<IInit>::all()) start->init();
					_playModeDirty = false;
					SceneManager::current()->safeCleanup();
				}
				
				fixedUpdateTime += deltaTime;
				while (fixedUpdateTime > Settings::Physics::fixedDeltaTime()) 
				{
					_physics->update();
					SceneManager::current()->safeCleanup();
					for (auto* fixed : Collector<IFixedUpdate>::all()) fixed->fixedUpdate();
					SceneManager::current()->safeCleanup();
					fixedUpdateTime -= Settings::Physics::fixedDeltaTime();
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