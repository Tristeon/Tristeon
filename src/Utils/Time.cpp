#include "Time.h"
#include "Project.h"

namespace Tristeon
{
	float Time::m_deltaTime;
	unsigned int Time::m_fps;
	
	float Time::deltaTime()
	{
		return m_deltaTime;
	}

	float Time::fixedDeltaTime()
	{
		return Project::Physics::fixedDeltaTime();
	}

	unsigned int Time::fps()
	{
		return m_fps;
	}
}