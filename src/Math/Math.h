#pragma once
#include <cstdint>
#include <Math/Vector.h>

namespace Tristeon
{
	/**
	 * @brief Math utility class, provides useful functions that might otherwise require more implementation time.
	*/
	class Math
	{
	public:
		/**
		 * @brief Converts a value in degrees to a value in radians.
		*/
		static float toRadians(const float& degrees);

		/**
		 * @brief Converts a value in radians to a value in degrees.
		*/
		static float toDegrees(const float& radians);

		/**
		 * @brief Clamps the given value between min [inclusive] and max [inclusive].
		*/
		static float clamp(const float& value, const float& min, const float& max);

		/**
		 * @brief Clamps the given value between 0 [inclusive] and 1 [inclusive].
		 */
		static float clamp01(const float& value);
		
		/**
		 * @brief Clamps the given value between min [inclusive] and max [inclusive].
		 */
		static int clamp(const int& value, const int& min, const int& max);

		/**
		 * @brief Returns a position that orbits around the given center with the given offset using the rotation (degrees).
		 * @param center The center point to be orbited around
		 * @param offset The offset from the center
		 * @param rotation The rotation around the center with 0 being a right-pointing vector 
		 *
		 * @note This function assumes the Tristeon coordinate systemw with clockwise rotation
		 */
		static Vector orbit(const Vector& center, const Vector& offset, const float& rotation);

		/**
		 * @brief Returns -1 if the value is less than 0, 0 if the value == 0, 1 if the value is more than 0.
		 */
		static int sign(const float& value);

		/**
		 * @brief 3.14159265f
		*/
		constexpr static float PI = 3.14159265f;
		/**
		 * @brief PI * 2.0f
		*/
		constexpr static float TAU = PI * 2.0f;
		/**
		 * @brief PI * 4.0f
		*/
		constexpr static float FOUR_PI = PI * 4.0f;
		
		/**
		 * @brief PI / 2.0f
		*/
		constexpr static float HALF_PI = PI / 2.0f;
		/**
		 * @brief PI * 4.0f
		*/
		constexpr static float QUARTER_PI = PI / 4.0f;
		
		/**
		 * @brief Convert degrees to radians by multiplying the degrees with this value.
		*/
		constexpr static float TO_RAD = PI / 180.0f;
		/**
		 * @brief Convert radians to degrees by multiplying the degrees with this value.
		*/
		constexpr static float TO_DEG = 180.0f / PI;
	};
}
