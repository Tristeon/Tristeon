#include "Engine.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Window.h>

#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <QApplication>

#include "AssetDatabase.h"

namespace Tristeon
{
	void Engine::run()
	{
		QApplication::processEvents();
		AssetDatabase::load();
		
		_renderer = std::make_unique<Renderer>();

		//SceneManager must be loaded last because its components can rely on any of the previously created subsystems
		SceneManager::load("Scene");
		//SceneManager::testLoadScene();

		while (!QApplication::closingDown())
		{
			QApplication::processEvents();
			Window::instance()->pollEvents();

			Scene* scene = SceneManager::current();

			if (scene != nullptr)
				scene->update();

			_view->paintGL();

			Mouse::reset();
			Keyboard::reset();
			
			QApplication::sendPostedEvents();
		}
	}
}
