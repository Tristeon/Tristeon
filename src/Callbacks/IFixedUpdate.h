#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class IFixedUpdate : ICallback<IFixedUpdate>
	{
	public:
		virtual void fixedUpdate() = 0;
	};
}