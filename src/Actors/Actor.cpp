#include "Actor.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Actor)

	Actor::~Actor()
	{
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
		j["tag"] = tag;

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
		tag = j["tag"].get<std::string>();

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
				_behaviours[i].reset();
				_behaviours.removeAt(i);
				break;
			}
		}
	}
}