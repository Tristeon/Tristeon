#pragma once
#include <Physics/Contact.h>

namespace Tristeon
{
	class Collider;

	/**
	 * Inherit from this class to receive contactBegin callbacks.
	 * contactBegin is called on the first moment that a collision existed between two non-sensor colliders.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as either one of the colliders (for example an EnemyActor with a collider)
	 */
	class IContactBegin
	{
	public:
		virtual ~IContactBegin() = default;
		/**
		 * contactBegin is called on the first moment that a collision existed between two non-sensor colliders.
		 */
		virtual void contactBegin(const Contact& contact) = 0;
	};
}