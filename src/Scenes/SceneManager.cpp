#include "SceneManager.h"
#include <Scenes/Scene.h>

#include "Actors/AnimationSprite.h"
#include "Actors/Behaviours/TestBehaviour.h"
#include "Animations/AnimationClip.h"
#include "Layers/ActorLayer.h"
#include "Layers/TileLayer.h"
#include "Serialization/JsonSerializer.h"

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

		TileLayer* tileLayer = new TileLayer();
		tileLayer->tileSet = std::make_unique<TileSet>("Project/TilesetTest.png", 3, 5, nullptr);
		
		currentScene->layers.push_back(std::unique_ptr<TileLayer>(tileLayer));
		currentScene->camera = std::make_unique<Camera>();
		currentScene->camera->zoom = 0.3f;
		currentScene->camera->size = Vector2Int(800, 600);
		currentScene->camera->position = Vector2Int(978, 368);

		auto* layer = new ActorLayer();
		currentScene->layers.push_back(std::unique_ptr<ActorLayer>(layer));

		auto* actor = new Sprite();
		actor->addBehaviour<TestBehaviour>();
		actor->setTexture("Project/penguin.png", true);
		actor->width = 128;
		actor->height = 128;
		actor->position.x = 208;
		actor->position.y = 1042;
		layer->actors.push_back(std::unique_ptr<Actor>(actor));

		auto* actor2 = new AnimationSprite();
		actor2->position.x = 1478;
		actor2->position.y = 870;
		actor2->setTexture("Project/Tiny Platformer/Sprites/Character 1/Run.png", false);
		actor2->width = 256;
		actor2->height = 256;
		layer->actors.push_back(std::unique_ptr<Actor>(actor2));

		//Proof of creation using json, scene is reset and then loaded in through json data
		json data = currentScene->serialize();
		std::cout << "Scene: " << currentScene->serialize().dump(4) << std::endl;

		Unique<Serializable> serializable = TypeRegister::createInstance(data["typeID"]);
		currentScene.reset((Scene*)serializable.release()); //Cast and move ownership over
		currentScene->deserialize(data);
	}

	void SceneManager::reset()
	{
		//currentScene.reset();
	}
}
