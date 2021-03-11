#include "AmbientLight.h"

namespace Tristeon
{
	AmbientLight::AmbientLight()
	{
		Collector<AmbientLight>::add(this);
	}

	AmbientLight::~AmbientLight()
	{
		Collector<AmbientLight>::remove(this);
	}

	json AmbientLight::serialize()
	{
		json j = Light::serialize();
		j["typeID"] = Type<AmbientLight>::fullName();
		return j;
	}

	void AmbientLight::deserialize(json j)
	{
		Light::deserialize(j);
	}
}