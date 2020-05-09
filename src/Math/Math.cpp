#include "Math.h"

namespace Tristeon
{
	int Math::maskToIndex(uint64_t mask)
	{
		const int MAX_BIT = 40;
		int result = MAX_BIT;

		while (mask != 0)
		{
			mask >>= 1;
			result -= 1;
		}

		return MAX_BIT - result;
	}

	float Math::toRadians(float const& degrees)
	{
		return degrees * toRad;
	}

	float Math::toDegrees(float const& radians)
	{
		return radians * toDeg;
	}

	float Math::clamp(float const& value, float const& min, float const& max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}
}
