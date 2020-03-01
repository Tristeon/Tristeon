#include "Behaviour.h"

namespace Tristeon
{
	Behaviour::Behaviour(Behaviour&& other) noexcept
	{
		_owner = other._owner;
		other._owner = nullptr;
	}

	Behaviour& Behaviour::operator=(Behaviour&& other) noexcept
	{
		_owner = other._owner;
		other._owner = nullptr;
		return *this;
	}
}
