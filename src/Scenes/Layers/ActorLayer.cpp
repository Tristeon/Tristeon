#include "ActorLayer.h"

#include "Scenes/Actors/Actor.h"

#include <Rendering/Renderer.h>
#include <Scenes/Scene.h>

#include <Rendering/Sprite.h>
#include <Callbacks/IPreDestroy.h>

namespace Tristeon
{
	ActorLayer::~ActorLayer()
	{
		for (auto const& actor : findActorsOfType<IPreDestroy>())
			actor->preDestroy();

		for (auto const& actor : _actors)
			actor->_destroyed = true;
	}

	json ActorLayer::serialize()
	{
		json j = Layer::serialize();
		j["typeID"] = Type<ActorLayer>::fullName();

		json serializedActors = json::array_t();
		for (auto& actor : _actors)
			serializedActors.push_back(actor->serialize());
		j["actors"] = serializedActors;

		return j;
	}

	void ActorLayer::deserialize(json j)
	{
		Layer::deserialize(j);

		for (const auto& a : _actors)
		{
			if (dynamic_cast<IPreDestroy*>(a.get()))
				dynamic_cast<IPreDestroy*>(a.get())->preDestroy();
			a->_destroyed = true;
		}
		_actors.clear();

		for (const auto& serializedActor : j["actors"])
		{
			//TODO: Potentially detect existing actors and simply re-deserialize as opposed to recreating them
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedActor["typeID"]);
			if (serializable == nullptr)
				continue;
			serializable->deserialize(serializedActor);
			_actors.add(Unique<Actor>((Actor*)serializable.release()));
		}
	}

	Actor* ActorLayer::actorAt(const unsigned long long& index) const
	{
		if (index >= _actors.size())
			throw std::invalid_argument("Index in ActorLayer::actorAt() must be less than actorCount()");

		return _actors[index].get();
	}

	unsigned long long ActorLayer::actorCount() const
	{
		return _actors.size();
	}

	bool ActorLayer::contains(Actor* actor) const
	{
		return _actors.any([=](const Unique<Actor>& unique) { return unique.get() == actor; });
	}

	Actor* ActorLayer::findActor(const String& name) const
	{
		for (auto& actor : _actors)
		{
			if (actor->name == name)
				return actor.get();
		}

		return nullptr;
	}

	Actor* ActorLayer::createActor(const String& type)
	{
		auto serializable = TypeRegister::createInstance(type);
		auto* actor = dynamic_cast<Actor*>(serializable.get());
		if (actor != nullptr)
		{
			actor = dynamic_cast<Actor*>(serializable.release());
			_actors.add(std::unique_ptr<Actor>(actor));
			return actor;
		}
		return nullptr;
	}

	void ActorLayer::destroyActor(Actor* actor)
	{
		_destroyedActors.add(actor);
	}

	void ActorLayer::destroyBehaviour(Behaviour* behaviour)
	{
		_destroyedBehaviours.add(behaviour);
	}

	void ActorLayer::render(const Framebuffer& framebuffer)
	{
		//Render each graphic
		for (auto& actor : _actors)
		{
			auto* graphic = dynamic_cast<Graphic*>(actor.get());
			if (graphic == nullptr || !graphic->display)
				continue;

			auto* shader = graphic->getShader();
			shader->bind();
			graphic->render();
		}
	}

	void ActorLayer::safeCleanup()
	{
		//Clear behaviours first
		for (auto* behaviour : _destroyedBehaviours)
		{
			Actor* actor = behaviour->actor();
			for (size_t i = 0; i < actor->_behaviours.size(); i++)
			{
				if (actor->_behaviours[i].get() == behaviour)
				{
					auto* pre = dynamic_cast<IPreDestroy*>(actor->_behaviours[i].get());
					if (pre != nullptr)
						pre->preDestroy();

					actor->_behaviours[i].reset();
					actor->_behaviours.removeAt(i);
					break;
				}
			}
		}
		_destroyedBehaviours.clear();

		//Then handle actors
		for (auto* actor : _destroyedActors)
		{
			for (size_t i = 0; i < _actors.size(); i++)
			{
				if (_actors[i].get() == actor)
				{
					auto* pre = dynamic_cast<IPreDestroy*>(_actors[i].get());
					if (pre != nullptr)
						pre->preDestroy();

					_actors[i].reset();
					_actors.removeAt(i);
					break;
				}
			}
		}
		_destroyedActors.clear();
	}
}