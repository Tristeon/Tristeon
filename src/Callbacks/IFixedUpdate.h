#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive fixedUpdate callbacks.
	 * fixedUpdate is called a fixed amount of times a second, defined by the physics settings.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class IFixedUpdate : ICallback<IFixedUpdate>
	{
	public:
		/**
		 * fixedUpdate is called a fixed amount of times per second, defined by the physics settings.
		 */
		virtual void fixedUpdate() = 0;
	};
}