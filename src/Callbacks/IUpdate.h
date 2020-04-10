#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive update callbacks.
	 * Update is called once every frame.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class IUpdate : ICallback<IUpdate>
	{
	public:
		virtual void update() = 0;
	};
}