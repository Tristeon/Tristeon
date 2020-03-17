#include "Actor.h"

namespace Tristeon
{
	REGISTER_TYPE_CPP(Actor)
	
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
		for (size_t i = 0; i < behaviours.size(); i++)
			serializedBehaviours.push_back(behaviours[i]->serialize());
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

		behaviours.clear();
		for (auto serializedBehaviour : j["behaviours"])
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedBehaviour["typeID"]);
			auto* behaviour = dynamic_cast<Behaviour*>(serializable.release());
			behaviour->_owner = this;
			behaviours.push_back(Unique<Behaviour>(behaviour));

			behaviour->deserialize(serializedBehaviour);
		}
	}
}
