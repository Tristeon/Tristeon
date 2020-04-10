#pragma once
#include "ICallback.h"

namespace Tristeon
{
	/**
	 * Inherit from this class to receive lateUpdate callbacks.
	 * lateUpdate is called once every frame, after all update calls.
	 *
	 * The inherited class does NOT have to be an actor or behaviour, any class will work.
	 */
	class ILateUpdate : ICallback<ILateUpdate>
	{
	public:
		virtual void lateUpdate() = 0;
	};
}