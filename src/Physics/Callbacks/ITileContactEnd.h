#pragma once
#include "Callbacks/ICallback.h"
#include <Physics/TileContact.h>

namespace Tristeon
{
	/**
	 * Inherit from this class to receive contactEnd callbacks.
	 * contactEnd is called once a collision between a non-sensor collider and a tile has come to an end.
	 *
	 * The inherited class must be an actor or a behaviour, and must be part of the same object as the non-tile collider.
	 */
	class ITileContactEnd : public ICallback<ITileContactEnd>
	{
	public:
		/**
		 * contactEnd is called once a collision between a non-sensor collider and a tile has come to an end.
		 */
		virtual void tileContactEnd(TileContact const& contact) = 0;
	};
}