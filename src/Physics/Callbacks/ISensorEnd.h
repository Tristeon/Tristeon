#pragma once
#include <Physics/Contact.h>

namespace Tristeon
{
	class Collider;

	/**
	 * Inherit from this class to receive sensorEnd callbacks.
	 * sensorEnd is called once a collision with a sensor and another collider has come to an end.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as either one of the colliders (for example a CoinActor with a collider)
	 */
	class ISensorEnd
	{
	public:
		virtual void sensorEnd(Contact const& contact) = 0;
	};
}