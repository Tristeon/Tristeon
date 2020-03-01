#include "SceneManager.h"
#include <Scenes/Scene.h>


#include "Layers/ActorLayer.h"
#include "Layers/TileLayer.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;

	Scene* SceneManager::getCurrentScene()
	{
		//TODO: SceneManager implementation
		return currentScene.get();
	}

	void SceneManager::loadScene()
	{
		currentScene = std::make_unique<Scene>();
		currentScene->layers.push_back(std::make_unique<TileLayer>());
		currentScene->camera = std::make_unique<Camera>();
		currentScene->camera->zoom = 0.3f;
		currentScene->camera->size = Vector2Int(800, 600);
		currentScene->camera->position = Vector2Int(978, 368);

		auto* layer = new ActorLayer();
		currentScene->layers.push_back(std::unique_ptr<ActorLayer>(layer));

		auto* actor = new Actor();
		SpriteBehaviour* sprite = actor->addBehaviour<SpriteBehaviour>();
		sprite->setTexture("Project/penguin.png", true);
		sprite->width = 128;
		sprite->height = 128;
		actor->position.x = 208;
		actor->position.y = 1042;
		layer->actors.push_back(std::unique_ptr<Actor>(actor));
	}

	void SceneManager::reset()
	{
		//currentScene.reset();
	}
}
