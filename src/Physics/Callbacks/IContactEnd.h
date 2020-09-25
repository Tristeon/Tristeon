#pragma once
#include <Physics/Contact.h>

namespace Tristeon
{
	class Collider;

	/**
	 * Inherit from this class to receive contactEnd callbacks.
	 * contactEnd is called once a collision between two non-sensor colliders has come to an end.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as either one of the colliders (for example an EnemyActor with a collider)
	 */
	class IContactEnd
	{
	public:
		virtual ~IContactEnd() = default;

		/**
		 * contactEnd is called once a collision between two non-sensor colliders has come to an end.
		 */
		virtual void contactEnd(const Contact& contact) = 0;
	};
}