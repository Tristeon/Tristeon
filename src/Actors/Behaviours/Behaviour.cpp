#include "Behaviour.h"

namespace Tristeon
{
	Behaviour::Behaviour(Behaviour&& other) noexcept
	{
		owner = other.owner;
		other.owner = nullptr;
	}

	Behaviour& Behaviour::operator=(Behaviour&& other) noexcept
	{
		owner = other.owner;
		other.owner = nullptr;
		return *this;
	}
}
