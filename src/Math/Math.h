#pragma once
#include <cstdint> //Required by some compilers
#include <Math/Vector.h>

namespace Tristeon
{
	/**
	 * Math utility class, provides useful functions that might otherwise require more implementation time.
	*/
	class Math
	{
	public:
		/**
		 * Converts a value in degrees to a value in radians.
		*/
		static float toRadians(const float& degrees);

		/**
		 * Converts a value in radians to a value in degrees.
		*/
		static float toDegrees(const float& radians);

		/**
		 * Clamps the given value between min [inclusive] and max [inclusive].
		*/
		static float clamp(const float& value, const float& min, const float& max);

		/**
		 * Clamps the given value between 0 [inclusive] and 1 [inclusive].
		 */
		static float clamp01(const float& value);
		
		/**
		 * Clamps the given value between min [inclusive] and max [inclusive].
		 */
		static int clamp(const int& value, const int& min, const int& max);

		/**
		 * Returns a position that orbits around the given center with the given offset using the rotation (degrees).
		 * @param center The center point to be orbited around
		 * @param offset The offset from the center
		 * @param rotation The rotation around the center with 0 being a right-pointing vector 
		 *
		 * @note This function assumes the Tristeon coordinate systemw with clockwise rotation
		 */
		static Vector orbit(const Vector& center, const Vector& offset, const float& rotation);

		/**
		 * Returns -1 if the value is less than 0, 0 if the value == 0, 1 if the value is more than 0.
		 */
		static int sign(const float& value);

		/**
		 * 3.14159265f
		*/
		constexpr static float PI = 3.14159265f;
		/**
		 * PI * 2.0f
		*/
		constexpr static float TAU = PI * 2.0f;
		/**
		 * PI * 4.0f
		*/
		constexpr static float FOUR_PI = PI * 4.0f;
		
		/**
		 * PI / 2.0f
		*/
		constexpr static float HALF_PI = PI / 2.0f;
		/**
		 * PI * 4.0f
		*/
		constexpr static float QUARTER_PI = PI / 4.0f;
		
		/**
		 * Convert degrees to radians by multiplying the degrees with this value.
		*/
		constexpr static float TO_RAD = PI / 180.0f;
		/**
		 * Convert radians to degrees by multiplying the degrees with this value.
		*/
		constexpr static float TO_DEG = 180.0f / PI;
	};
}
