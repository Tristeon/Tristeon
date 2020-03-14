#include "ActorLayer.h"

#include <Rendering/Renderer.h>
#include <Scenes/Scene.h>

#include <Actors/Collector.h>
#include <Actors/Sprite.h>

#include <Rendering/GameView.h>

namespace Tristeon
{
	REGISTER_TYPE_CPP(ActorLayer);

	json ActorLayer::serialize()
	{
		json j = Layer::serialize();
		j["typeID"] = TRISTEON_TYPENAME(ActorLayer);

		json serializedActors = json::array_t();
		for (auto& actor : actors)
			serializedActors.push_back(actor->serialize());
		j["actors"] = serializedActors;

		return j;
	}

	void ActorLayer::deserialize(json j)
	{
		Layer::deserialize(j);
		
		actors.clear();

		for (auto serializedActor : j["actors"])
		{
			//TODO: Potentially detect existing actors and simply re-deserialize as opposed to recreating them
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedActor["typeID"]);
			serializable->deserialize(serializedActor);
			actors.push_back(Unique<Actor>((Actor*)serializable.release()));
		}
	}

	Actor* ActorLayer::getActor(unsigned int const& index) const
	{
		if (index < 0 || index > actors.size())
			throw std::invalid_argument("Index in ActorLayer::getActor() must be 0 or higher, and lower than getActorSize()");

		return actors[index].get();
	}

	unsigned ActorLayer::getActorCount() const
	{
		return actors.size();
	}

	Actor* ActorLayer::findActor(std::string const& name) const
	{
		for (auto& actor : actors)
		{
			if (actor->name == name)
				return actor.get();
		}

		return nullptr;
	}

	void ActorLayer::render(Renderer * renderer, Scene * scene)
	{
		//Render each graphic
		for (auto& actor : actors)
		{
			auto* graphic = dynamic_cast<Graphic*>(actor.get());
			if (graphic == nullptr)
				continue;

			Shader* shader = graphic->getShader();
			shader->bind();
			graphic->render(shader->getShaderProgram());
		}
	}

	void ActorLayer::update()
	{
		for (auto& actor : actors)
		{
			actor->update();
			for (auto& behaviour : actor->behaviours)
				behaviour->update();
		}
	}
}
