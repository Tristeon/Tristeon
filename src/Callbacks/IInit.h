#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive an init callback.
	 *
	 * Init is called at 2 potential moments:
	 * - Once a scene is fully loaded
	 * - When a new actor or behaviour is added to the scene / existing actor.
	 * Either way, Init is called when every object in the Scene exists and has been deserialized.
	 *
	 * Any class can inherit from IInit to receive an init callback after the scene is initialized,
	 * however the 2nd potential moment is reserved to Actors or Behaviours.
	 */
	class IInit : ICallback<IInit>
	{
	public:
		/**
		 * Init is called at 2 potential moments:
		 * - Once a scene is fully loaded (any object may receive this callback)
		 * - When a new actor or behaviour is added to the scene / existing actor (only actors or behaviours can receive this callback).
		 */
		virtual void init() = 0;
	};
}