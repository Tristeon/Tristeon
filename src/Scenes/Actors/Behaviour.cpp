#include "Behaviour.h"
#include <Collector.h>
#include "Engine.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Layers/ActorLayer.h"

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
		for (auto* layer : SceneManager::current()->findLayersOfType<ActorLayer>())
		{
			if (layer->contains(actor()))
			{
				layer->destroyBehaviour(this);
				_destroyed = true;
				return;
			}
		}

		assert(_destroyed);
	}
}
