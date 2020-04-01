#pragma once

namespace Tristeon
{
	class Collider;

	class ISensorEnd
	{
	public:
		virtual void sensorEnd(Collider* other) = 0;
	};
}