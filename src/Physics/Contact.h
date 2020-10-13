#pragma once
#include "Collider.h"

namespace Tristeon
{
	/**
	 * Data structure containing information about a collision/contact between two colliders.
	 */
	struct Contact
	{
		/**
		 * The other collider.
		 */
		Collider* other = nullptr;
		/**
		 * The normal of the collision.
		 */
		Vector normal{};
	};
}