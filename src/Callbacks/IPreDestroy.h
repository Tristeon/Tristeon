#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive a callback before the object is destroyed.
	 * This callback is only called on Actors and Behaviours.
	 */
	class IPreDestroy : ICallback<IPreDestroy>
	{
	public:
		/**
		 * preDestroy() is called on actors and behaviours when they're about to be destroyed.
		 *
		 * The actor and its behaviours will still be entirely valid at this point, although references to other actors may not be.
		 */
		virtual void preDestroy() = 0;
	};
}