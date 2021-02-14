#include "Light.h"

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
		j["sourceType"] = _type;
		j["range"] = _range;
		return j;
	}

	void Light::deserialize(json j)
	{
		Behaviour::deserialize(j);

		_intensity = j.value("intensity", 1.0f);
		_type = j.value("sourceType", SourceType::Point);
		_range = j.value("range", 256.0f);
	}
}