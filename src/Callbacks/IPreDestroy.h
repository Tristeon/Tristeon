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
		virtual void preDestroy() = 0;
	};
}