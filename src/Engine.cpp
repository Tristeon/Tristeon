#include "Engine.h"

#include <QApplication>
#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

namespace Tristeon
{
	Engine::Engine()
	{
		renderer = std::make_unique<Renderer>(this);
		SceneManager::initialize();
	}

	Engine::~Engine()
	{
		SceneManager::reset();
	}

	void Engine::run()
	{
		QApplication::exec();
	}

	void Engine::setWindow(GameView* window)
	{
		this->view = window;
	}
}
