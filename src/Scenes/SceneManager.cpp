#include "SceneManager.h"

#include <Scenes/Scene.h>
#include <Serialization/JsonSerializer.h>

#include <AssetDatabase.h>

#include <Actors/Behaviours/TestBehaviour.h>

#include <Actors/AnimationSprite.h>
#include <Animations/AnimationClip.h>

#include <Scenes/Layers/ActorLayer.h>
#include <Scenes/Layers/TileLayer.h>

#include <Physics/PhysicsBody.h>

#include "Physics/CircleCollider.h"
#include "Physics/PhysicsWorld.h"

namespace Tristeon
{
	std::unique_ptr<Scene> SceneManager::currentScene = nullptr;
	Delegate<Scene*> SceneManager::sceneLoaded;
	
	Scene* SceneManager::current()
	{
		return currentScene.get();
	}

	void SceneManager::load(String name)
	{
		String const path = findPath(name);
		if (path.empty())
		{
			std::cout << "Couldn't find scene: " << name << std::endl;
			currentScene.reset();
			sceneLoaded.invoke(nullptr);
			return;
		}
		
		currentScene = std::unique_ptr<Scene>(JsonSerializer::deserialize<Scene>(path));
		currentScene->name = name;
		currentScene->path = path;
		
		for (auto start : Collector<IStart>::all()) start->start();

		sceneLoaded.invoke(currentScene.get());
	}

	void SceneManager::reload()
	{
		if (currentScene == nullptr)
			return;
		
		load(currentScene->name);
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

		//Layer 1
		auto* tileLayer = new TileLayer();
		tileLayer->name = "TileLayer";
		tileLayer->w = 10;
		tileLayer->h = 6;

		Tile tileInfo[48];
		tileInfo[13].hasCollider = true;
		
		tileLayer->tileSet = std::make_unique<TileSet>("Project/TilesetTest.png", 3, 5, tileInfo, 256, 256);

		tileLayer->data = Unique<int[]>(new int[60]{
				13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
				4, 2, 3, 1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, 13, 13, 13, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			});

		currentScene->layers.push_back(std::unique_ptr<TileLayer>(tileLayer));

		currentScene->camera = std::make_unique<Camera>();
		currentScene->camera->zoom = 0.3;
		currentScene->camera->size = Vector2Int(1920, 1080);
		currentScene->camera->position = Vector2Int(0, 0);

		//Layer 2
		auto* layer = new ActorLayer();
		layer->name = "ActorLayer";
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

		//Layer 3
		auto* actorLayer2 = new ActorLayer();
		actorLayer2->name = "Decoration";
		currentScene->layers.push_back(std::unique_ptr<ActorLayer>(actorLayer2));

		auto* actor3 = new Sprite();
		actor3->position.x = 2000;
		actor3->position.y = 1250;
		actor3->name = "Penguin 2";
		actor3->setTexture("Project/Penguin.png", false);
		actor3->width = 256;
		actor3->height = 256;
		actorLayer2->actors.push_back(std::unique_ptr<Actor>(actor3));
		
		//Proof of creation using json, scene is reset and then loaded in through json data
		json data = currentScene->serialize();
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
