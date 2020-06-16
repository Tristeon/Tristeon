#pragma once
#include "Collider.h"

namespace Tristeon
{
	struct Contact
	{
		Collider* other = nullptr;
		Vector2 normal{};
	};
}
