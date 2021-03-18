#include "Light.h"

#include <InstanceCollector.h>
#include <Collector.h>

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
		j["renderMask"] = (uint8_t)_mask;
		return j;
	}

	void Light::deserialize(json pJson)
	{
		Behaviour::deserialize(pJson);
		_intensity = pJson.value("intensity", 1.0f);
		_colour = pJson.value("colour", Colour());
		_mask = pJson.value("renderMask", RenderMask::All);
	}
}