#include "SceneManager.h"

#include <Scenes/Scene.h>
#include <Serialization/JsonSerializer.h>

#include <AssetDatabase.h>

#include <Actors/Behaviours/TestBehaviour.h>

#include <Actors/AnimationSprite.h>
#include <Animations/AnimationClip.h>

#include <Scenes/Layers/ActorLayer.h>
#include <Scenes/Layers/TileLayer.h>

#include <Physics/BoxCollider.h>
#include <Physics/PhysicsBody.h>

#include "Physics/CircleCollider.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;

	Scene* SceneManager::current()
	{
		return currentScene.get();
	}

	void SceneManager::load(String const& name)
	{
		String const path = findPath(name);
		if (path.empty())
		{
			std::cout << "Couldn't find scene: " << name << std::endl;
			currentScene.reset();
			return;
		}
		
		currentScene = std::unique_ptr<Scene>(JsonSerializer::deserialize<Scene>(path));

		for (auto start : Collector<IStart>::all()) start->start();
	}

	void SceneManager::save(Scene* scene, String const& filepath)
	{
		if (filepath.empty())
			throw std::invalid_argument("Filepath can't be empty!");
		
		json data = scene->serialize();
		JsonSerializer::save(filepath, data);
	}

	String SceneManager::findPath(String const& name)
	{
		Vector<String> scenes = AssetDatabase::get("scene");
		for (String const& scene : scenes)
		{
			QFile const file{ QString(scene.c_str()) };
			QFileInfo info{ file };

			if (info.baseName().toStdString() == name)
				return scene;
		}

		return "";
	}

	void SceneManager::saveTestScene()
	{
		//TODO: SceneManager implementation
		
		currentScene = std::make_unique<Scene>();

		auto* tileLayer = new TileLayer();
		tileLayer->w = 10;
		tileLayer->h = 6;
		//->
		//tileLayer->tileSet = std::make_unique<TileSet>("Project/TilesetTest.png", 3, 5, nullptr, 256, 256);
		//tileLayer->data = Unique<int[]>(new int[60] {
		//		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		//		4, 4, -1, -1, -1, -1, -1, -1, -1, -1,
		//		9, 10, -1, -1, -1, -1, 4, 4, 14, 14,
		//		-1, -1, -1, -1, -1, 4, 10, 9, 4, 14,
		//		0, 1, 2, 0, 8, 3, 6, 3, 6, 8,
		//		3, 6, 3, 6, 3, 6, 3, 6, 3, 6
		//	});

		tileLayer->tileSet = std::make_unique<TileSet>("Project/TilesetTest5.png", 2, 2, nullptr, 256, 256, 1, 1, 1, 1, 1, 1);
		tileLayer->data = Unique<int[]>(new int[60]{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				4, 4, -1, -1, -1, -1, -1, -1, -1, -1,
				9, 10, -1, -1, -1, -1, 4, 4, 14, 14,
				-1, -1, -1, -1, -1, 4, 10, 9, 4, 14,
				0, 1, 2, 0, 8, 3, 6, 3, 6, 8,
				3, 6, 3, 6, 3, 6, 3, 6, 3, 6
			});

		currentScene->layers.push_back(std::unique_ptr<TileLayer>(tileLayer));

		currentScene->camera = std::make_unique<Camera>();
		currentScene->camera->zoom = 0.3;
		currentScene->camera->size = Vector2Int(800, 800);
		currentScene->camera->position = Vector2Int(0, 0);

		auto* layer = new ActorLayer();
		currentScene->layers.push_back(std::unique_ptr<ActorLayer>(layer));

		auto* actor = new Sprite();
		actor->addBehaviour<TestBehaviour>();
		actor->setTexture("Project/penguin.png", true);
		actor->width = 256;
		actor->height = 256;
		actor->position.x = 208;
		actor->position.y = 1042;
		actor->name = "Penguin";
		PhysicsBody* body = actor->addBehaviour<PhysicsBody>();
		body->type = PhysicsBody::Dynamic;
		body->continuous(true);
		CircleCollider* col = actor->addBehaviour<CircleCollider>();
		col->radius(128);
		col->density(1.0f);
		col->friction(0.3f);
		col->restitution(0);
		layer->actors.push_back(std::unique_ptr<Actor>(actor));

		auto* actor2 = new AnimationSprite();
		actor2->position.x = 2000;
		actor2->position.y = 1250;
		actor2->name = "Animated numbers";
		actor2->setTexture("Project/SpriteSheet.png", false);
		actor2->width = 256;
		actor2->height = 256;
		layer->actors.push_back(std::unique_ptr<Actor>(actor2));

		auto* actor3 = new Sprite();
		actor3->setTexture("", true);
		actor3->name = "Ground";
		actor3->width = 2048;
		actor3->height = 256;
		actor3->position.x = 0;
		actor3->position.y = 0;
		BoxCollider* floor = actor3->addBehaviour<BoxCollider>();
		floor->width(2048);
		floor->height(256);
		floor->density(0.0f);
		floor->restitution(0);
		layer->actors.push_back(std::unique_ptr<Actor>(actor3));

		//Proof of creation using json, scene is reset and then loaded in through json data
		json data = currentScene->serialize();
		std::cout << "Scene: " << currentScene->serialize().dump(4) << std::endl;
		JsonSerializer::save("Project/Scene.scene", data);
		currentScene.reset();
	}

	void SceneManager::destroyActor(Actor* actor)
	{
		Vector<ActorLayer*> actorLayers = current()->findLayersOfType<ActorLayer>();
		for (const auto& layer : actorLayers)
			layer->destroyActor(actor);
	}

	void SceneManager::reset()
	{
		currentScene.reset();
	}
}
