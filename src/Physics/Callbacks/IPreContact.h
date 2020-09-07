#pragma once
#include "Physics/Contact.h"

namespace Tristeon
{
	struct Contact;
	class Collider;

	/**
	 * Inherit from this class to receive the preContact callback.
	 * preContact is called before a collision is resolved, returning false on the callback causes the collision to be ignored.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as either one of the colliders (for example an EnemyActor with a collider)
	 */
	class IPreContact
	{
	public:
		virtual ~IPreContact() = default;
		
		/**
		 * preContact is called before a collision is resolved, returning false on the callback causes the collision to be ignored.
		 */
		virtual bool preContact(Contact const& contact) = 0;
	};
}