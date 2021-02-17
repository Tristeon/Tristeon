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
		j["sourceType"] = _type;
		j["intensity"] = _intensity;
		j["range"] = _range;
		j["colour"] = _colour;
		
		j["direction"] = _direction;
		j["cutoff"] = _cutoff;
		
		return j;
	}

	void Light::deserialize(json j)
	{
		Behaviour::deserialize(j);

		_intensity = j.value("intensity", 1.0f);
		_type = j.value("sourceType", SourceType::Point);
		_range = j.value("range", 256.0f);
		_colour = j.value("colour", Colour());

		_direction = j.value("direction", Vector{});
		_cutoff = j.value("cutoff", 360.0f);
	}
}