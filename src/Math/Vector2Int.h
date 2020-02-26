#pragma once
#include <array>

#include "Vector2.h"

namespace Tristeon
{
	/**
	 * Vector2Int interface, describes a 2D point or movement
	 */
	struct Vector2Int final
	{
		/**
		 * Creates a (xy, xy) Vector2Int
		 * \param xy The x and y of this vector
		 */
		Vector2Int(int xy);
		/**
		 * Creates a (x, y) Vector2Int
		 * \param x The x of this vector
		 * \param y The y of this vector
		 */
		Vector2Int(int x = 0, int y = 0);

		/**
		 * Creates a vector2int with the given Vector2
		 */
		explicit Vector2Int(Vector2 vec2);
		
		/**
		 * Converts the Vector2Int into a Vector2
		 */
		operator Vector2() const { return Vector2(x, y); }

#pragma region quick vectors

		/**
		* (0, -1)
		*/
		static Vector2Int down() { return {0, -1}; }
		/**
		* (0, 1)
		*/
		static Vector2Int up() { return {0, 1}; }
		/**
		* (1, 0)
		*/
		static Vector2Int right() { return {1, 0}; }

		/**
		* (-1, 0)
		*/
		static Vector2Int left() { return {-1, 0}; }

		/**
		* (0, 0)
		*/
		static Vector2Int zero() { return {0, 0}; }
		/**
		* (1, 1)
		*/
		static Vector2Int one() { return {1, 1}; }
#pragma endregion

		int x;
		int y;

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		*
		* \exception invalid_argument  If the axis is higher than the dimensions supported by Vector2Int, or if it's below 0
		*/
		int getAxis(const int& axis) const;
		/**
		* Scales vector by the multiplier
		* \param multiplier the amount it multiplies by
		*/
		void scale(int multiplier);
		/**
		Returns the magnitude of the vector
		*/
		float getLength() const;
		/**
		* Returns the magnitude of the vector squared (It's more optimized than normal getLength)
		*/
		float getSqrLength() const;
		/**
		*Returns the distance between the two given vectors
		*/
		static float distance(Vector2Int vec, Vector2Int vec2);
		/**
		* Dot product of the two given vectors
		*/
		static int dot(Vector2Int vec, Vector2Int vec2);
		/**
		* Returns the distance between the vector and the given vector
		*/
		float distance(Vector2Int vec) const;
		/**
		* Dot product between the vector and the given vector
		*/
		float dot(Vector2Int vec) const;

		/**
		* Gets the axis with the given index
		*/
		int operator[](const int& value) const;

		bool operator==(const Vector2Int & vec) const;
		bool operator!=(const Vector2Int & vec) const;
		Vector2Int operator*(const int& multiplier) const;
		Vector2Int operator*(const Vector2Int & vec) const;
		Vector2Int operator/(const int& divider) const;
		Vector2Int operator/(const Vector2Int & divider) const;
		Vector2Int operator+(const Vector2Int & vec) const;
		Vector2Int operator-(const Vector2Int & vec) const;
		void operator-=(const Vector2Int & vector);
		void operator+=(const Vector2Int & vector);
		void operator *=(const Vector2Int & vector);
		void operator *=(const int& value);

		/**
		* Convert this instance to a string describing the properties
		*/
		std::string toString() const;

		std::array<int, 2> toArray() const { return { x, y }; }

		template<typename T>
		constexpr T convert() { return { x, y }; }

		template<typename T>
		static constexpr Vector2Int convert(T * vec) { return { vec->x, vec->y }; }

		template<typename T>
		static constexpr Vector2Int convert(T const& vec) { return { vec.x, vec.y }; }
	};

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	Vector2Int operator*(const int& multiplier, Vector2Int const& vector);

	static_assert(sizeof(Vector2Int) == 2 * sizeof(int), "Vector2Int shouldn't contain anything else than 2 integers");
}
