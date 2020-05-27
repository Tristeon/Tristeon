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

	Vector2 Math::orbit(Vector2 const& center, Vector2 const& offset, float const& rotation)
	{
		float const theta = toRadians(rotation);
		float const c = cos(theta);
		float const s = sin(theta);
		
		Vector2 result;
		result.x = center.x + (offset.x) * c + (offset.y) * s;
		result.y = center.y - (offset.x) * s + (offset.y) * c;
		return result;
	}

	int Math::sign(float const& value)
	{
		if (value > 0)
			return 1;
		if (value < 0)
			return -1;
		return 0;
	}
}
