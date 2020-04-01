#pragma once

namespace Tristeon
{
	class Collider;
	
	class ISensorBegin
	{
	public:
		virtual void sensorBegin(Collider* other) = 0;
	};
}