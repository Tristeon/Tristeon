#include "Light.h"
#include <InstanceCollector.h>

namespace Tristeon
{
	Light::Light()
	{
		Collector<Light>::add(this);
	}

	Light::~Light()
	{
		Collector<Light>::remove(this);
	}

	json Light::serialize()
	{
		json j = Behaviour::serialize();
		j["typeID"] = Type<Light>::fullName();
		j["intensity"] = _intensity;
		j["colour"] = _colour;
		j["ignoreLayers"] = _ignoreLayers;		
		return j;
	}

	void Light::deserialize(json j)
	{
		Behaviour::deserialize(j);
		_intensity = j.value("intensity", 1.0f);
		_colour = j.value("colour", Colour());
		_ignoreLayers = j.value("ignoreLayers", std::set<uint32_t>());
	}

	void Light::ignore(Layer* layer)
	{
		_ignoreLayers.insert(layer->instanceID());
	}

	void Light::unignore(Layer* layer)
	{
		_ignoreLayers.erase(layer->instanceID());
	}

	std::set<Layer*> Light::ignored()
	{
		std::set<Layer*> result;
		for (auto id : _ignoreLayers)
		{
			auto* layer = InstanceCollector::find(id);
			if (dynamic_cast<Layer*>(layer) != nullptr)
				result.insert(dynamic_cast<Layer*>(layer));
		}
		return result;
	}
}