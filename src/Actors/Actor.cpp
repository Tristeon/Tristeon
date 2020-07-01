#include "Actor.h"

#include "Callbacks/IPreDestroy.h"
#include "Engine.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Actor)

	Actor::~Actor()
	{
		assert(destroyed == true);
		
		for (auto& b : getBehaviours<IPreDestroy>()) { b->preDestroy(); }
		for (int i = _behaviours.size() - 1; i >= 0; --i)
		{
			_behaviours[i]->destroyed = true;
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
		position = j.value("position", Vector2());
		scale = j.value("scale", Vector2::one());
		rotation = j.value("rotation", 0);
		name = j.value("name", "");

		for (auto& b : _behaviours)
			b->destroyed = true;
		for (auto& b : getBehaviours<IPreDestroy>()) { b->preDestroy(); }
		_behaviours.clear();

		for (auto serializedBehaviour : j.value("behaviours", json::array_t()))
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedBehaviour["typeID"]);
			if (serializable == nullptr)
			{
				std::cout << "Attempted to create behaviour of type " << serializedBehaviour["typeID"] << ", but failed. It will be ignored upon creation and removed from the save file. \n";
				continue;
			}
			auto* behaviour = dynamic_cast<Behaviour*>(serializable.get());
			if (behaviour == nullptr)
			{
				std::cout << "Failed to cast type " << serializedBehaviour["typeID"] << ", to Tristeon::Behaviour. The object will be ignored upon creation and removed from the save file. \n";
				serializable.reset();
				continue;
			}
			serializable.release();
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
		destroyed = true;
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
