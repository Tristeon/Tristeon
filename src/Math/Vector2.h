#pragma once
#include <array>
#include <json.h>

#include <Serialization/Type.h>

namespace Tristeon
{
	/**
	 * Vector2 interface, describes a 2D point or movement
	 */
	struct Vector2 final
	{
		Vector2() = default;
		/**
		 * Creates a (x, y) Vector2
		 * \param x The x of this vector
		 * \param y The y of this vector
		 */
		Vector2(const float& x, const float& y);

#pragma region quick vectors
		/**
		* (0, -1)
		*/
		[[nodiscard]] static Vector2 down() { return Vector2(0, -1); }
		/**
		* (0, 1)
		*/
		[[nodiscard]] static Vector2 up() { return Vector2(0, 1); }
		/**
		* (1, 0)
		*/
		[[nodiscard]] static Vector2 right() { return Vector2(1, 0); }

		/**
		* (-1, 0)
		*/
		[[nodiscard]] static Vector2 left() { return Vector2(-1, 0); }

		/**
		* (0, 0)
		*/
		[[nodiscard]] static Vector2 zero() { return Vector2(0, 0); }
		/**
		* (1, 1)
		*/
		[[nodiscard]] static Vector2 one() { return Vector2(1, 1); }
#pragma endregion

		/**
		 * The horizontal axis, negative is left, positive is right.
		 */
		float x;
		/**
		 * The vertical axis, negative is down, positive is up.
		 */
		float y;

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		*
		* \exception invalid_argument  If the axis is higher than the dimensions supported by Vector2, or if it's below 0
		*/
		[[nodiscard]] float getAxis(const int& axis) const;
		/**
		* returns normalized vector
		*/
		[[nodiscard]] Vector2 getNormalized() const;
		/**
		* Normalizes the vector into an unit vector
		*/
		void normalize();
		/**
		* Scales vector by the multiplier
		* \param multiplier the amount it multiplies by
		*/
		void scale(const float& multiplier);
		/**
		Returns the magnitude of the vector
		*/
		[[nodiscard]] float getLength() const;
		/**
		* Returns the magnitude of the vector squared (It's more optimized than normal getLength)
		*/
		[[nodiscard]] float getSqrLength() const;
		/**
		 * Floors the vector's values to the nearest integer downwards.
		 */
		[[nodiscard]] static Vector2 floor(const Vector2& vec);
		/**
		 * Ceils the vector's values to the nearest integer upwards.
		 */
		[[nodiscard]] static Vector2 ceil(const Vector2& vec);
		/**
		 * Rounds the vector's values to the nearest integer.
		 */
		[[nodiscard]] static Vector2 round(const Vector2& vec);
		/**
		*Returns the distance between the two given vectors
		*/
		[[nodiscard]] static float distance(const Vector2& vec, const Vector2& vec2);
		/**
		* Dot product of the two given vectors
		*/
		[[nodiscard]] static float dot(const Vector2& vec, const Vector2& vec2);
		/**
		 * Calculates the angle between 2 vectors in degrees.
		 */
		[[nodiscard]] static float angle(const Vector2& vec, const Vector2& vec2);
		/**
		* Returns the distance between the vector and the given vector
		*/
		[[nodiscard]] float distance(const Vector2& vec) const;
		/**
		* Dot product between the vector and the given vector
		*/
		[[nodiscard]] float dot(const Vector2& vec) const;
		/**
		 * Rotate the vector by a given amount in degrees in clockwise direction.
		 */
		void rotate(const float& degrees);
		/**
		 * Gets the rotation of the vector in degrees.
		 */
		[[nodiscard]] float getAngle() const;
		/**
		 * Creates a unit vector with the given rotation in degrees.
		 */
		[[nodiscard]] static Vector2 unit(const float& degrees);

		/**
		* Linearly interpolates between two vectors.
		* \param a The start point
		* \param b The destination
		* \param t interpolate rate (0-1)
		*/
		[[nodiscard]] static Vector2 lerp(const Vector2& a, const Vector2& b, const float& t);

		/**
		* Gets the axis with the given index
		*/
		[[nodiscard]] float operator[](const int& value) const;

		[[nodiscard]] bool operator==(const Vector2& vec) const;
		[[nodiscard]] bool operator!=(const Vector2& vec) const;
		[[nodiscard]] Vector2 operator*(const float& multiplier) const;
		[[nodiscard]] Vector2 operator*(const Vector2& vec) const;
		[[nodiscard]] Vector2 operator/(const float& divider) const;
		[[nodiscard]] Vector2 operator/(const Vector2& divider) const;
		[[nodiscard]] Vector2 operator+(const Vector2& vec) const;
		[[nodiscard]] Vector2 operator-(const Vector2& vec) const;
		[[nodiscard]] Vector2 operator-() const;

		void operator-=(const Vector2& vector);
		void operator+=(const Vector2& vector);
		void operator *=(const Vector2& vector);
		void operator *=(const float& value);
		void operator /=(const Vector2& vector);
		void operator /=(const float& value);

		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator <(const Vector2& vec) const;
		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator >(const Vector2& vec) const;

		/**
		* Convert this instance to a string describing the properties
		*/
		[[nodiscard]] String toString() const;

		[[nodiscard]] std::array<float, 2> toArray() const { return { x, y }; }

		template<typename T>
		[[nodiscard]] constexpr T convert() const { return { x, y }; }

		template<typename T>
		[[nodiscard]] static constexpr Vector2 convert(T* vec) { return { vec->x, vec->y }; }

		template<typename T>
		[[nodiscard]] static constexpr Vector2 convert(T const& vec) { return { vec.x, vec.y }; }
	};

	inline void to_json(nlohmann::json& j, const Vector2& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<Vector2>::fullName();
	}

	inline void from_json(const nlohmann::json& j, Vector2& p) {
		p.x = j.value("x", 0.0f);
		p.y = j.value("y", 0.0f);
	}

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	[[nodiscard]] Vector2 operator*(const float& multiplier, const Vector2& vector);

	static_assert(sizeof(Vector2) == 2 * sizeof(float), "Vector2 shouldn't contain anything else than 2 floats");
}