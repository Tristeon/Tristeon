#include "Behaviour.h"
#include <Actors/Collector.h>
#include "Engine.h"

namespace Tristeon
{
	Behaviour::Behaviour()
	{
		Collector<Behaviour>::add(this);
	}

	Behaviour::~Behaviour()
	{
		assert(destroyed == true);
		Collector<Behaviour>::remove(this);
	}

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

	void Behaviour::destroy()
	{
		destroyed = true;
		Engine::instance()->destroyLater(this);
	}
}
