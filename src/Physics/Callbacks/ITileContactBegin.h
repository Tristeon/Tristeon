#pragma once
#include "Callbacks/ICallback.h"
#include <Physics/TileContact.h>

namespace Tristeon
{
	/**
	 * Inherit from this class to receive contactBegin callbacks.
	 * contactBegin is called on the first moment that a collision existed between a non-sensor collider and a tile.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as the non-tile collider.
	 */
	class ITileContactBegin : public ICallback<ITileContactBegin>
	{
	public:
		/**
		 * contactBegin is called on the first moment that a collision existed between a non-sensor collider and a tile.
		 */
		virtual void tileContactBegin(TileContact const& contact) = 0;
	};
}