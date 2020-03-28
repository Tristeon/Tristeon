#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class IStart : ICallback<IStart>
	{
	public:
		virtual void start() = 0;
	};
}