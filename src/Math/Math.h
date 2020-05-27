#pragma once
#include <cstdint>

#include "Vector2.h"

namespace Tristeon
{
	/**
	 * Math utility class, provides useful functions that might otherwise require more implementation time.
	 */
	class Math
	{
	public:
		/**
		 * Converts a bitmask to an index
		 *
		 * E.g: 16 is the 5th bitmask so the function will return 5.
		 */
		static int maskToIndex(uint64_t mask);

		/**
		 * Converts a value in degrees to a value in radians.
		 */
		static float toRadians(float const& degrees);

		/**
		 * Converts a value in radians to a value in degrees.
		 */
		static float toDegrees(float const& radians);

		/**
		 * Clamps the given value between min [inclusive] and max [inclusive].
		 */
		static float clamp(float const& value, float const& min, float const& max);

		/**
		 * Returns a position that orbits around the given center with the given offset using the rotation (degrees).
		 */
		static Vector2 orbit(Vector2 const& center, Vector2 const& offset, float const& rotation);

		/**
		 * Returns -1 if the value is less than 0, 0 if the value == 0, 1 if the value is more than 0.
		 */
		static int sign(float const& value);

		constexpr static float PI = 3.14159265;
		constexpr static float toRad = PI / 180.0f;
		constexpr static float toDeg = 180.0f / PI;
	};
}