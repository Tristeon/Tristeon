#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class IUpdate : ICallback<IUpdate>
	{
	public:
		virtual void update() = 0;
	};
}