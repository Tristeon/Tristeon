#include "Light.h"
#include <Rendering/Gizmos.h>

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

		j["innerRadius"] = _innerRadius;
		j["outerRadius"] = _outerRadius;

		j["innerCutoff"] = _innerCutoff;
		j["outerCutoff"] = _outerCutoff;

		j["colour"] = _colour;
		
		j["distance"] = _distance;
		j["direction"] = _direction;
		
		return j;
	}

	void Light::deserialize(json j)
	{
		Behaviour::deserialize(j);

		_intensity = j.value("intensity", 1.0f);
		_type = j.value("sourceType", SourceType::Point);

		_innerRadius = j.value("innerRadius", 256.0f);
		_outerRadius = j.value("outerRadius", 1028.0f);

		_innerCutoff = j.value("innerCutoff", 360.0f);
		_outerCutoff = j.value("outerCutoff", 360.0f);

		_colour = j.value("colour", Colour());

		_distance = j.value("distance", 1);
		_direction = j.value("direction", Vector{});
	}
	
	void Light::drawGizmos()
	{
		Gizmos::drawCircle(actor()->position, _innerRadius, Colour::green());
		Gizmos::drawCircle(actor()->position, _outerRadius, Colour::red());
		Gizmos::drawLine(actor()->position, actor()->position + _direction.normalize() * _outerRadius, Colour::blue());
	}
}