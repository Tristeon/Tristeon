#include "Actor.h"

#include "Callbacks/IPreDestroy.h"
#include "Engine.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Actor)

	Actor::~Actor()
	{
		for (auto& b : getBehaviours<IPreDestroy>()) { b->preDestroy(); }
		for (int i = _behaviours.size() - 1; i >= 0; --i)
		{
			_behaviours[i].reset();
			_behaviours.removeAt(i);
		}
	}

	json Actor::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Actor);
		j["position"] = position;
		j["scale"] = scale;
		j["rotation"] = rotation;
		j["name"] = name;

		json serializedBehaviours = json::array_t();
		for (size_t i = 0; i < _behaviours.size(); i++)
			serializedBehaviours.push_back(_behaviours[i]->serialize());
		j["behaviours"] = serializedBehaviours;
		return j;
	}

	void Actor::deserialize(json j)
	{
		position = j["position"];
		scale = j["scale"];
		rotation = j["rotation"];
		name = j["name"].get<std::string>();

		for (auto& b : getBehaviours<IPreDestroy>()) { b->preDestroy(); }
		_behaviours.clear();

		for (auto serializedBehaviour : j["behaviours"])
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedBehaviour["typeID"]);
			auto* behaviour = dynamic_cast<Behaviour*>(serializable.release());
			behaviour->_owner = this;
			_behaviours.push_back(Unique<Behaviour>(behaviour));

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

	Vector<Behaviour*> Actor::getBehaviours()
	{
		Vector<Behaviour*> result;
		for (size_t i = 0; i < _behaviours.size(); i++)
			result.add(_behaviours[i].get());
		return result;
	}

	Behaviour* Actor::addBehaviour(std::string type)
	{
		Behaviour* result = BehaviourRegister::createInstance(type).release();

		if (result == nullptr)
			return nullptr;
		
		result->_owner = this;
		_behaviours.push_back(Unique<Behaviour>(result));

		//Call start callback if available.
		IStart* istart = dynamic_cast<IStart*>(result);
		if (istart != nullptr)
			istart->start();

		return result;
	}

	void Actor::destroy()
	{
		Engine::instance()->destroyLater(this);
	}

	Actor* Actor::find(String const& name)
	{
		auto layers = SceneManager::current()->findLayersOfType<ActorLayer>();
		for (auto layer : layers)
		{
			auto* actor = layer->findActor(name);
			if (actor)
				return actor;
		}
		return nullptr;
	}
}
