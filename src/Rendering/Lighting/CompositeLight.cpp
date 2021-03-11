#include "CompositeLight.h"

namespace Tristeon
{
	CompositeLight::CompositeLight()
	{
		Collector<CompositeLight>::add(this);
	}

	CompositeLight::~CompositeLight()
	{
		Collector<CompositeLight>::remove(this);
	}
}