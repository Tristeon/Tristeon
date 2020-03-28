#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class IEarlyUpdate : ICallback<IEarlyUpdate>
	{
	public:
		virtual void earlyUpdate() = 0;
	};
}