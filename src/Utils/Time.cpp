#include "Time.h"
#include "Settings.h"

namespace Tristeon
{
	float Time::_deltaTime;
	unsigned int Time::_fps;
	
	float Time::deltaTime()
	{
		return _deltaTime;
	}

	float Time::fixedDeltaTime()
	{
		return Settings::Physics::fixedDeltaTime();
	}

	unsigned int Time::fps()
	{
		return _fps;
	}
}