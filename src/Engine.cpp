#include "Engine.h"

#include <QApplication>
#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

namespace Tristeon
{
	Engine::Engine()
	{
		window = std::make_unique<Window>(this);
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

	void Engine::update()
	{
		Scene* scene = SceneManager::getCurrentScene();
		
		if (scene != nullptr)
			scene->update();

		window->preRender();
		
		if (scene != nullptr)
		{
			renderer->renderScene(scene);
			renderer->renderHUD(scene->getHUD());
		}

		window->postRender();
	}
}
