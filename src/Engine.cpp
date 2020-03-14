#include "Engine.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Window.h>

#include <Input/Keyboard.h>
#include <Input/Mouse.h>

namespace Tristeon
{
	Engine::Engine()
	{
		QTimer::singleShot(0, this, &Engine::initialize);

		timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &Engine::update);
		timer->start(0);
	}

	void Engine::initialize()
	{
		_renderer = std::make_unique<Renderer>();

		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::loadScene();
	}

	void Engine::update()
	{
		Window::instance()->pollEvents();

		Scene* scene = SceneManager::current();

		if (scene != nullptr)
			scene->update();

		_view->paintGL();

		Mouse::reset();
		Keyboard::reset();
	}
}