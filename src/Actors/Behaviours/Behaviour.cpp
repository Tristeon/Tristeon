#include "Behaviour.h"
#include <Collectors/Collector.h>
#include "Engine.h"

namespace Tristeon
{
	Behaviour::Behaviour()
	{
		Collector<Behaviour>::add(this);
	}

	Behaviour::~Behaviour()
	{
		assert(_destroyed == true);
		Collector<Behaviour>::remove(this);
	}

	Behaviour::Behaviour(Behaviour&& other) noexcept
	{
		_actor = other._actor;
		other._actor = nullptr;
	}

	Behaviour& Behaviour::operator=(Behaviour&& other) noexcept
	{
		_actor = other._actor;
		other._actor = nullptr;
		return *this;
	}

	void Behaviour::destroy()
	{
		_destroyed = true;
		Engine::instance()->destroyLater(this);
	}
}
