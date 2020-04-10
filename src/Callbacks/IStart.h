#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive a start callback.
	 *
	 * Start is called at 2 potential moments:
	 * - Once a scene is fully loaded
	 * - When a new actor or behaviour is added to the scene / existing actor.
	 *
	 * Any class can inherit from IStart to receive a start callback after the scene is initialized,
	 * however the 2nd potential moment is only available to Actors or Behaviours.
	 */
	class IStart : ICallback<IStart>
	{
	public:
		virtual void start() = 0;
	};
}