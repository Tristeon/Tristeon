#include "SceneManager.h"
#include <Scenes/Scene.h>

#include "Actors/Behaviours/AnimationSprite.h"
#include "Actors/Behaviours/TestBehaviour.h"
#include "Animations/AnimationClip.h"
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
		actor->addBehaviour<TestBehaviour>();
		Sprite* sprite = actor->addBehaviour<Sprite>();
		sprite->setTexture("Project/penguin.png", true);
		sprite->width = 128;
		sprite->height = 128;
		actor->position.x = 208;
		actor->position.y = 1042;
		layer->actors.push_back(std::unique_ptr<Actor>(actor));

		auto* actor2 = new Actor();
		actor2->position.x = 1478;
		actor2->position.y = 870;

		AnimationSprite* animation = actor2->addBehaviour<AnimationSprite>();
		animation->setTexture("Project/Tiny Platformer/Sprites/Character 1/Run.png", false);
		animation->width = 256;
		animation->height = 256;
		AnimationClip* idle = new AnimationClip("Project/Tiny Platformer/Sprites/Character 1/Run.png", 6, 1, true); //TODO: Memory leak, animation clips should be owned by some kinda manager
		animation->setAnimationClip(idle);
		
		layer->actors.push_back(std::unique_ptr<Actor>(actor2));
	}

	void SceneManager::reset()
	{
		//currentScene.reset();
	}
}
