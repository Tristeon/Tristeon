#include "Time.h"
#include "Project.h"

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
		return Project::Physics::fixedDeltaTime();
	}

	unsigned int Time::fps()
	{
		return _fps;
	}
}