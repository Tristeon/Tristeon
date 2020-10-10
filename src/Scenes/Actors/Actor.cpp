#include "Actor.h"

#include "Engine.h"
#include "Callbacks/IPreDestroy.h"
#include <InstanceCollector.h>
#include "Scenes/SceneManager.h"

#include "Scenes/Scene.h"
#include "Scenes/Layers/ActorLayer.h"
#include "Serialization/TypeRegister.h"

namespace Tristeon
{
	Actor::Actor()
	{
		Collector<Actor>::add(this);
	}

	Actor::Actor(const bool& registerSelf)
	{
		if (registerSelf)
			Collector<Actor>::add(this);
	}

	Actor::~Actor() noexcept
	{
		Collector<Actor>::remove(this);

		assert(_destroyed == true);

		for (auto& b : findBehaviours<IPreDestroy>()) { b->preDestroy(); }
		for (int i = _behaviours.size() - 1; i >= 0; --i)
		{
			_behaviours[i]->_destroyed = true;
			_behaviours[i].reset();
		}
		_behaviours.clear();
	}

	json Actor::serialize()
	{
		auto j = InstancedSerializable::serialize();
		j["typeID"] = Type<Actor>::fullName();
		j["position"] = position;
		j["scale"] = scale;
		j["rotation"] = rotation;
		j["name"] = name;

		json serializedBehaviours = json::array_t();
		for (const auto& behaviour : _behaviours)
			serializedBehaviours.push_back(behaviour->serialize());
		j["behaviours"] = serializedBehaviours;
		return j;
	}

	void Actor::deserialize(json j)
	{
		InstancedSerializable::deserialize(j);

		position = j.value("position", Vector2());
		scale = j.value("scale", Vector2::one());
		rotation = j.value("rotation", 0);
		name = j.value("name", "");

		//Clear previous behaviours
		for (auto& b : _behaviours)
			b->_destroyed = true;
		for (auto& b : findBehaviours<IPreDestroy>()) { b->preDestroy(); }
		_behaviours.clear();

		for (const auto& serializedBehaviour : j.value("behaviours", json::array_t()))
		{
			//Attempt to create behaviour from type ID
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedBehaviour["typeID"]);
			if (serializable == nullptr)
			{
				Console::warning("Attempted to create behaviour of type " + serializedBehaviour.value("typeID", "") + ", but failed. It will be ignored upon creation and removed from the save file.");
				continue;
			}
			//Attempt to cast serializable down to behaviour
			auto* behaviour = dynamic_cast<Behaviour*>(serializable.get());
			if (behaviour == nullptr)
			{
				Console::warning("Failed to cast type " + serializedBehaviour.value("typeID", "") + ", to Tristeon::Behaviour. The object will be ignored upon creation and removed from the save file.");
				serializable.reset();
				continue;
			}

			//Successfully created, move behaviour
			serializable.release();
			behaviour->_actor = this;
			_behaviours.add(Unique<Behaviour>(behaviour));
			behaviour->deserialize(serializedBehaviour);
		}
	}

	unsigned long long Actor::behaviourCount() const
	{
		return _behaviours.size();
	}

	Behaviour* Actor::behaviourAt(const unsigned long long& index)
	{
		if (index >= _behaviours.size())
			throw std::invalid_argument("Index in Actor::behaviourAt() must be less than behaviourCount()");
		
		return _behaviours[index].get();
	}

	Vector<Behaviour*> Actor::findBehaviours()
	{
		Vector<Behaviour*> result;
		for (const auto& behaviour : _behaviours)
			result.add(behaviour.get());
		return result;
	}

	Behaviour* Actor::createBehaviour(const std::string& type)
	{
		auto* result = Register<Behaviour>::createInstance(type).release();

		if (result == nullptr)
			return nullptr;

		result->_actor = this;
		_behaviours.add(Unique<Behaviour>(result));

		//Call start callback if available.
		auto* istart = dynamic_cast<IStart*>(result);
		if (istart != nullptr)
			istart->start();

		return result;
	}

	void Actor::destroyBehaviour(Behaviour* behaviour)
	{
		if (!behaviour || behaviour->actor() != this)
			return;
		
		behaviour->destroy();
	}

	void Actor::destroy()
	{
		_destroyed = true;

		if (SceneManager::current() != nullptr)
		{
			for (auto* layer : SceneManager::current()->findLayersOfType<ActorLayer>())
			{
				if (layer->contains(this))
				{
					layer->destroyActor(this);
					return;
				}
			}
		}
	}

	Actor* Actor::find(const String& name)
	{
		auto actors = Collector<Actor>::all();
		for (auto actor : actors)
		{
			if (actor->name == name)
				return actor;
		}
		return nullptr;
	}

	Actor* Actor::find(const unsigned int& id)
	{
		auto* instance = InstanceCollector::find(id);
		auto* actor = dynamic_cast<Actor*>(instance);
		return actor; //Returns nullptr if the dynamic_cast failed.
	}
}