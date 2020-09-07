#include "Actor.h"

#include "Engine.h"
#include "Callbacks/IPreDestroy.h"
#include "Collectors/InstanceCollector.h"
#include "Scenes/SceneManager.h"
#include <Scenes/Layers/ActorLayer.h>

#include "Scenes/Scene.h"
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

		for (auto& b : behaviours<IPreDestroy>()) { b->preDestroy(); }
		for (int i = _behaviours.size() - 1; i >= 0; --i)
		{
			_behaviours[i]->_destroyed = true;
			_behaviours[i].reset();
			_behaviours.removeAt(i);
		}
	}

	json Actor::serialize()
	{
		auto j = Serializable::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Actor);
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
		Serializable::deserialize(j);

		position = j.value("position", Vector2());
		scale = j.value("scale", Vector2::one());
		rotation = j.value("rotation", 0);
		name = j.value("name", "");

		//Clear previous behaviours
		for (auto& b : _behaviours)
			b->_destroyed = true;
		for (auto& b : behaviours<IPreDestroy>()) { b->preDestroy(); }
		_behaviours.clear();

		for (auto serializedBehaviour : j.value("behaviours", json::array_t()))
		{
			//Attempt to create behaviour from type ID
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedBehaviour["typeID"]);
			if (serializable == nullptr)
			{
				std::cout << "Attempted to create behaviour of type " << serializedBehaviour["typeID"] << ", but failed. It will be ignored upon creation and removed from the save file. \n";
				continue;
			}
			//Attempt to cast serializable down to behaviour
			auto* behaviour = dynamic_cast<Behaviour*>(serializable.get());
			if (behaviour == nullptr)
			{
				std::cout << "Failed to cast type " << serializedBehaviour["typeID"] << ", to Tristeon::Behaviour. The object will be ignored upon creation and removed from the save file. \n";
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

	void Actor::removeBehaviour(Behaviour* behaviour)
	{
		for (size_t i = 0; i < _behaviours.size(); i++)
		{
			if (_behaviours[i].get() == behaviour)
			{
				auto* pre = dynamic_cast<IPreDestroy*>(_behaviours[i].get());
				if (pre != nullptr)
					pre->preDestroy();

				_behaviours[i].reset();
				_behaviours.removeAt(i);
				break;
			}
		}
	}

	Vector<Behaviour*> Actor::behaviours()
	{
		Vector<Behaviour*> result;
		for (const auto& behaviour : _behaviours)
			result.add(behaviour.get());
		return result;
	}

	Behaviour* Actor::addBehaviour(const std::string& type)
	{
		auto* result = Register<Behaviour>::createInstance(type).release();

		if (result == nullptr)
			return nullptr;

		result->_actor = this;
		_behaviours.add(Unique<Behaviour>(result));

		//Call start callback if available.
		IStart* istart = dynamic_cast<IStart*>(result);
		if (istart != nullptr)
			istart->start();

		return result;
	}

	void Actor::destroy()
	{
		_destroyed = true;
		Engine::destroyLater(this);
	}

	Actor* Actor::find(String const& name)
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