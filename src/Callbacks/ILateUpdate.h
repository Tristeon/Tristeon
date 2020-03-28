#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class ILateUpdate : ICallback<ILateUpdate>
	{
	public:
		virtual void lateUpdate() = 0;
	};
}