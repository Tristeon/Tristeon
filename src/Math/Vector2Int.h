#pragma once
#include <array>
#include <json.h>
#include <Serialization/Typename.h>
#include "Vector2.h"

namespace Tristeon
{
	/**
	 * Vector2Int interface, describes a 2D point or movement
	 */
	struct Vector2Int final
	{
		Vector2Int() = default;
		/**
		 * Creates a (x, y) Vector2Int
		 * \param x The x of this vector
		 * \param y The y of this vector
		 */
		Vector2Int(const int& x, const int& y);

		/**
		 * Creates a vector2int with the given Vector2
		 */
		explicit Vector2Int(const Vector2& vec2);

		/**
		 * Converts the Vector2Int into a Vector2
		 */
		[[nodiscard]] operator Vector2() const { return Vector2((float)x, (float)y); }

#pragma region quick vectors

		/**
		* (0, -1)
		*/
		[[nodiscard]] static Vector2Int down() { return { 0, -1 }; }
		/**
		* (0, 1)
		*/
		[[nodiscard]] static Vector2Int up() { return { 0, 1 }; }
		/**
		* (1, 0)
		*/
		[[nodiscard]] static Vector2Int right() { return { 1, 0 }; }

		/**
		* (-1, 0)
		*/
		[[nodiscard]] static Vector2Int left() { return { -1, 0 }; }

		/**
		* (0, 0)
		*/
		[[nodiscard]] static Vector2Int zero() { return { 0, 0 }; }
		/**
		* (1, 1)
		*/
		[[nodiscard]] static Vector2Int one() { return { 1, 1 }; }
#pragma endregion

		/**
		 * The horizontal axis, negative is left, positive is right.
		 */
		int x;
		/**
		 * The vertical axis, negative is down, positive is up.
		 */
		int y;

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		*
		* \exception invalid_argument  If the axis is higher than the dimensions supported by Vector2Int, or if it's below 0
		*/
		[[nodiscard]] int getAxis(const int& axis) const;
		/**
		* Scales vector by the multiplier
		* \param multiplier the amount it multiplies by
		*/
		void scale(const int& multiplier);
		/**
		Returns the magnitude of the vector
		*/
		[[nodiscard]] float getLength() const;
		/**
		* Returns the magnitude of the vector squared (It's more optimized than normal getLength)
		*/
		[[nodiscard]] float getSqrLength() const;
		/**
		*Returns the distance between the two given vectors
		*/
		[[nodiscard]] static float distance(const Vector2Int& vec, const Vector2Int& vec2);
		/**
		* Dot product of the two given vectors
		*/
		[[nodiscard]] static int dot(const Vector2Int& vec, const Vector2Int& vec2);
		/**
		* Returns the distance between the vector and the given vector
		*/
		[[nodiscard]] float distance(const Vector2Int& vec) const;
		/**
		* Dot product between the vector and the given vector
		*/
		[[nodiscard]] float dot(const Vector2Int& vec) const;

		/**
		* Gets the axis with the given index
		*/
		[[nodiscard]] int operator[](const int& value) const;

		[[nodiscard]] bool operator==(const Vector2Int& vec) const;
		[[nodiscard]] bool operator!=(const Vector2Int& vec) const;
		[[nodiscard]] Vector2Int operator*(const int& multiplier) const;
		[[nodiscard]] Vector2Int operator*(const Vector2Int& vec) const;
		[[nodiscard]] Vector2Int operator/(const int& divider) const;
		[[nodiscard]] Vector2Int operator/(const Vector2Int& divider) const;
		[[nodiscard]] Vector2Int operator+(const Vector2Int& vec) const;
		[[nodiscard]] Vector2Int operator-(const Vector2Int& vec) const;
		
		void operator-=(const Vector2Int& vector);
		void operator+=(const Vector2Int& vector);
		void operator *=(const Vector2Int& vector);
		void operator *=(const int& value);
		void operator *=(const float& value);

		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator <(const Vector2Int& vec) const;
		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator >(const Vector2Int& vec) const;

		/**
		* Convert this instance to a string describing the properties
		*/
		[[nodiscard]] String toString() const;

		[[nodiscard]] std::array<int, 2> toArray() const { return { x, y }; }

		template<typename T>
		[[nodiscard]] constexpr T convert() const { return { x, y }; }

		template<typename T>
		[[nodiscard]] static constexpr Vector2Int convert(T* vec) { return { vec->x, vec->y }; }

		template<typename T>
		[[nodiscard]] static constexpr Vector2Int convert(T const& vec) { return { vec.x, vec.y }; }
	};

	inline void to_json(nlohmann::json& j, const Vector2Int& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = TRISTEON_TYPENAME(Vector2Int);
	}

	inline void from_json(const nlohmann::json& j, Vector2Int& p) {
		p.x = j.value("x", 0);
		p.y = j.value("y", 0);
	}

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	[[nodiscard]] Vector2Int operator*(const int& multiplier, const Vector2Int& vector);

	static_assert(sizeof(Vector2Int) == 2 * sizeof(int), "Vector2Int shouldn't contain anything else than 2 integers");
}
