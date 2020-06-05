#include "Engine.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Window.h>

#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Input/Gamepad.h>
#include <QApplication>

#include "AssetDatabase.h"
#include <Callbacks/IEarlyUpdate.h>
#include <Callbacks/IFixedUpdate.h>
#include <Callbacks/ILateUpdate.h>
#include <Callbacks/IDrawGizmos.h>
#include <Callbacks/IUpdate.h>

#include "Actors/Actor.h"
#include "Project.h"
#include "Utils/Time.h"

namespace Tristeon
{
	void Engine::run()
	{
		QApplication::processEvents();

#ifndef TRISTEON_EDITOR //In Build mode, we assume the .exe is in the same folder as a build.json file.
		Project::loadBuild();
#endif
		
		AssetDatabase::load();
		
		_renderer = std::make_unique<Renderer>();
		_physics = std::make_unique<PhysicsWorld>();
		
		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::load(Project::firstSceneName());

		auto lastTime = std::chrono::high_resolution_clock::now();
		uint frames = 0;
		float time = 0;
		float fixedUpdateTime = 0;

		while (!QApplication::closingDown())
		{
			QApplication::processEvents();
			Window::instance()->pollEvents();

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
					for (auto start : Collector<IStart>::all()) start->start();
					playModeDirty = false;
				}
				
				fixedUpdateTime += deltaTime;
				while (fixedUpdateTime > Project::Physics::fixedDeltaTime()) 
				{
					_physics->update();
					for (auto fixed : Collector<IFixedUpdate>::all()) fixed->fixedUpdate();
					fixedUpdateTime -= Project::Physics::fixedDeltaTime();
				}

				for (auto early : Collector<IEarlyUpdate>::all()) early->earlyUpdate();
				for (auto update : Collector<IUpdate>::all()) update->update();
				for (auto late : Collector<ILateUpdate>::all()) late->lateUpdate();
			}

			for (auto gizmos : Collector<IDrawGizmos>::all()) gizmos->drawGizmos();

			for (auto const& behaviour : destroyedBehaviours) behaviour->getOwner()->removeBehaviour(behaviour);
			destroyedBehaviours.clear();
			for (auto const& actor : destroyedActors) SceneManager::destroyActor(actor);
			destroyedActors.clear();
			
			_view->update();

			QApplication::sendPostedEvents();
			
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
		destroyedActors.add(actor);
	}

	void Engine::destroyLater(Behaviour* behaviour)
	{
		destroyedBehaviours.add(behaviour);
	}
}
