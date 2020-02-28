#include "Engine.h"
#include <Scenes/SceneManager.h>

namespace Tristeon
{
	Engine* Engine::_instance = nullptr;

	Engine::Engine()
	{
		_instance = this;
	}

	Engine::~Engine()
	{
		SceneManager::reset();
		_instance = nullptr;
	}

	void Engine::initialize()
	{
		renderer = std::make_unique<Renderer>(this);
		SceneManager::loadScene();
	}

	void Engine::setGameView(GameView* gameView)
	{
		view = gameView;
	}
}