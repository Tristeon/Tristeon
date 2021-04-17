#include "Light.h"

#include <InstanceCollector.h>
#include <Collector.h>

#include <Serialization/MetaWrappers/SerializedEnum.h>

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
		j["renderMask"] = SerializedEnum<RenderMask>(_renderMask);
		return j;
	}

	void Light::deserialize(json pJson)
	{
		Behaviour::deserialize(pJson);
		_intensity = pJson.value("intensity", 1.0f);
		_colour = pJson.value("colour", Colour());
		_renderMask = pJson.value("renderMask", SerializedEnum<RenderMask>(RenderMask::All)).value;
	}
}