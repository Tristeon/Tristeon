#include "Engine.h"

#include <chrono>
#include <Scenes/Scene.h>
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

#include "Actors/Actor.h"
#include "Project.h"
#include "Utils/Time.h"
#include "AssetManagement/AssetDatabase.h"

namespace Tristeon
{
	void Engine::run()
	{
		Window::pollEvents();

#ifndef TRISTEON_EDITOR
		Project::loadBuild();
		AssetDatabase::load();
#endif

		_renderer = std::make_unique<Renderer>();
		_physics = std::make_unique<PhysicsWorld>();
		
		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::load(Project::firstSceneName());
		processDestroyedObjects();

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
			Time::m_deltaTime = deltaTime;
			lastTime = now;

			//FPS counter
			frames++;
			time += deltaTime;
			if (time >= 1000)
			{
				Time::m_fps = frames;
				frames = 0;
				time = fmod(time, 1000.0f);
			}

			if (inPlayMode)
			{
				if (playModeDirty)
				{
					for (auto* start : Collector<IStart>::all()) start->start();
					playModeDirty = false;
					processDestroyedObjects();
				}
				
				fixedUpdateTime += deltaTime;
				while (fixedUpdateTime > Project::Physics::fixedDeltaTime()) 
				{
					_physics->update();
					processDestroyedObjects();
					for (auto* fixed : Collector<IFixedUpdate>::all()) fixed->fixedUpdate();
					processDestroyedObjects();
					fixedUpdateTime -= Project::Physics::fixedDeltaTime();
				}

				for (auto* early : Collector<IEarlyUpdate>::all()) early->earlyUpdate();
				processDestroyedObjects();
				for (auto* update : Collector<IUpdate>::all()) update->update();
				processDestroyedObjects();
				for (auto* late : Collector<ILateUpdate>::all()) late->lateUpdate();
				processDestroyedObjects();
			}

			for (auto* gizmos : Collector<IDrawGizmos>::all()) gizmos->drawGizmos();

			processDestroyedObjects();
			
			Window::draw();

			Mouse::reset();
			Keyboard::reset();
			Gamepad::reset();

			SceneManager::processCachedLoad();
		}
	}

	void Engine::playMode(bool const& enabled)
	{
		instance()->inPlayMode = enabled;
		instance()->playModeDirty = true;
	}

	bool Engine::playMode()
	{
		return instance()->inPlayMode;
	}

	void Engine::destroyLater(Actor* actor)
	{
		instance()->destroyedActors.add(actor);
	}

	void Engine::destroyLater(Behaviour* behaviour)
	{
		instance()->destroyedBehaviours.add(behaviour);
	}

	void Engine::processDestroyedObjects()
	{
		for (auto const& behaviour : destroyedBehaviours) behaviour->actor()->removeBehaviour(behaviour);
		destroyedBehaviours.clear();
		for (auto const& actor : destroyedActors) SceneManager::destroyActor(actor);
		destroyedActors.clear();
	}
}
