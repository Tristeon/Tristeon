#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive fixedUpdate callbacks.
	 * fixedUpdate is called a fixed amount of times a second, defined by the project settings.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class IFixedUpdate : ICallback<IFixedUpdate>
	{
	public:
		virtual void fixedUpdate() = 0;
	};
}