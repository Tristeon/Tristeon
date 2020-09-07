#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive earlyUpdate callbacks.
	 * earlyUpdate is called once every frame, before all update calls.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class IEarlyUpdate : ICallback<IEarlyUpdate>
	{
	public:
		/**
		 * earlyUpdate is called once every frame, before all other update calls.
		 */
		virtual void earlyUpdate() = 0;
	};
}