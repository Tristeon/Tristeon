#include "Math.h"

namespace Tristeon
{
	float Math::toRadians(const float& degrees)
	{
		return degrees * TO_RAD;
	}

	float Math::toDegrees(const float& radians)
	{
		return radians * TO_DEG;
	}

	float Math::clamp(const float& value, const float& min, const float& max)
	{
		if (min > max)
			throw std::invalid_argument("Min can't be more than max!");
		
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	int Math::clamp(const int& value, const int& min, const int& max)
	{
		if (min > max)
			throw std::invalid_argument("Min can't be more than max!");
		
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	Vector Math::orbit(const Vector& center, const Vector& offset, const float& rotation)
	{
		auto const theta = toRadians(rotation);
		auto const c = cos(theta);
		auto const s = sin(theta);

		return {
			center.x + offset.x * c + offset.y * s,
			center.y - offset.x * s + offset.y * c
		};
	}

	int Math::sign(const float& value)
	{
		if (value > 0)
			return 1;
		if (value < 0)
			return -1;
		return 0;
	}
}