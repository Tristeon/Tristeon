#pragma once
#include <array>
#include <json.h>
#include <Serialization/Typename.h>

namespace Tristeon
{
	/**
	* Vector4 interface, describes a 4D point or movement and implements math operations to modify said point/movement.
	*/
	struct Vector4 final
	{
		Vector4() = default;
		/**
		 * Creates a (x, y, z, w) Vector4
		 */
		Vector4(const float& x, const float& y, const float& z, const float& w);

#pragma region const static vectors
		/**
		* (0, -1, 0)
		*/
		[[nodiscard]] static Vector4 down() { return Vector4(0, -1, 0, 0); }
		/**
		* (0, 1, 0)
		*/
		[[nodiscard]] static Vector4 up() { return Vector4(0, 1, 0, 0); }
		/**
		* (1, 0, 0)
		*/
		[[nodiscard]] static Vector4 right() { return Vector4(1, 0, 0, 0); }
		/**
		* (-1, 0, 0)
		*/
		[[nodiscard]] static Vector4 left() { return Vector4(-1, 0, 0, 0); }
		/**
		* (0, 0, 0)
		*/
		[[nodiscard]] static Vector4 zero() { return Vector4(0, 0, 0, 0); }
		/**
		* (1, 1, 1)
		*/
		[[nodiscard]] static Vector4 one() { return Vector4(1, 1, 1, 1); }
		/**
		(0, 0, -1)
		*/
		[[nodiscard]] static Vector4 back() { return Vector4(0, 0, -1, 0); }
		/**
		(0, 0, 1)
		*/
		[[nodiscard]] static Vector4 forward() { return Vector4(0, 0, 1, 0); }
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
		 * The depth axis, negative is backwards, positive is forwards.
		 */
		float z;
		/**
		 * The 4th dimensional axis. Nobody can tell you which way this one goes
		 */
		float w;

		/**
		* Access the x, y, z, w components using [0], [1], [2], [3] respectively.
		*
		* \exception invalid_argument If the axis is higher than the dimensions supported by Vector4, or if it's below 0.
		*/
		[[nodiscard]] float& getAxis(const int& axis);
		/**
		* Returns a unit (length = 1) vector with the same direction as this vector.
		*/
		[[nodiscard]] Vector4 getNormalized() const;
		/**
		* Normalizes the vector into a unit vector
		*/
		void normalize();
		/**
		* Scales vector by the multiplier
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
		*Returns the distance between two vectors
		*/
		[[nodiscard]] static float distance(const Vector4& a, const Vector4& b);
		/**
		* Dot product of the two given vectors
		*/
		[[nodiscard]] static float dot(const Vector4& a, const Vector4& b);
		/**
		* Returns the distance to the given vector
		*/
		[[nodiscard]] float distance(const Vector4& other) const;
		/**
		* Dot product between the vector and the given vector
		*/
		[[nodiscard]] float dot(const Vector4& other) const;
		/**
		* Linearly interpolates between two vectors.
		* \param a The start point
		* \param b The destination
		* \param t interpolate rate (0-1)
		*/
		[[nodiscard]] static Vector4 lerp(const Vector4& a, const Vector4& b, const float& t);

		/**
		* Access the x, y, z components using [0], [1], [2], [3] respectively.
		 */
		[[nodiscard]] float& operator[](const int& value);

		[[nodiscard]] bool operator==(const Vector4& other) const;
		[[nodiscard]] bool operator!=(const Vector4& other) const;
		[[nodiscard]] Vector4 operator*(const float& multiplier) const;
		[[nodiscard]] Vector4 operator*(const Vector4& other) const;
		[[nodiscard]] Vector4 operator/(const float& divider) const;
		[[nodiscard]] Vector4 operator/(const Vector4& divider) const;
		[[nodiscard]] Vector4 operator+(const Vector4& other) const;
		[[nodiscard]] Vector4 operator-(const Vector4& other) const;

		void operator-=(const Vector4& vector);
		void operator+=(const Vector4& vector);
		void operator *=(const Vector4& vector);
		void operator *=(const float& value);

		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator <(const Vector4& vec) const;
		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator >(const Vector4& vec) const;

		/**
		 * Convert to a string describing the vector's properties
		 */
		[[nodiscard]] String toString() const;

		/**
		 * Convert to an array of 4 floats.
		 */
		[[nodiscard]] std::array<float, 4> toArray() const { return { x, y, z, w }; }

		/**
		 * Convert to any given type, assuming it has a (float, float, float, float) constructor.
		 */
		template<typename T>
		[[nodiscard]] constexpr T convert() const { return T{ x, y, z, w }; }

		/**
		 * Convert any given type to a Vector4, assuming it has an x, y, z, and w field.
		 */
		template<typename T>
		[[nodiscard]] static constexpr Vector4 convert(T* vec) { return Vector4{ vec->x, vec->y, vec->z, vec->w }; }

		/**
		 * Convert any given type to a Vector4, assuming it has an x, y, z, and w field.
		 */
		template<typename T>
		[[nodiscard]] static constexpr Vector4 convert(const T& vec) { return Vector4{ vec.x, vec.y, vec.z, vec.w }; }
	};

	inline void to_json(nlohmann::json& j, const Vector4& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["z"] = p.z;
		j["w"] = p.w;
		j["typeID"] = TRISTEON_TYPENAME(Vector4);
	}

	inline void from_json(const nlohmann::json& j, Vector4& p) {
		p.x = j.value("x", 0.0f);
		p.y = j.value("y", 0.0f);
		p.z = j.value("z", 0.0f);
		p.w = j.value("w", 0.0f);
	}

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	Vector4 operator*(const float& multiplier, const Vector4& vector);

	static_assert(sizeof(Vector4) == 4 * sizeof(float), "Vector4 shouldn't contain anything else than 4 floats");
}
