#pragma once
#include <Physics/Contact.h>

namespace Tristeon
{
	class Collider;
	
	/**
	 * Inherit from this class to receive sensorBegin callbacks.
	 * sensorBegin is called on the first moment that a collision existed between a sensor and another collider.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as either one of the colliders (for example a CoinActor with a collider)
	 */
	class ISensorBegin
	{
	public:
		virtual void sensorBegin(Contact const& contact) = 0;
	};
}