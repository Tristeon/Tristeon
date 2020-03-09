#include "Behaviour.h"
#include "Actors/Collector.h"

namespace Tristeon
{
	Behaviour::Behaviour()
	{
		BehaviourCollector<Behaviour>::add(this);
	}

	Behaviour::~Behaviour()
	{
		BehaviourCollector<Behaviour>::remove(this);
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
}
