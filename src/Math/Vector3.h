#pragma once
#include <array>
#include <json.h>
#include <Serialization/Type.h>

namespace Tristeon
{
	/**
	* Vector3 interface, describes a 3D point or movement and implements math operations to modify said point/movement.
	*/
	struct Vector3 final
	{
	public:
		Vector3() = default;
		/**
		 * Creates a (x, y, z) Vector3
		 */
		Vector3(const float& x, const float& y, const float& z);

#pragma region const static vectors
		/**
		* (0, -1, 0)
		*/
		[[nodiscard]] static Vector3 down() { return Vector3(0, -1, 0); }
		/**
		* (0, 1, 0)
		*/
		[[nodiscard]] static Vector3 up() { return Vector3(0, 1, 0); }
		/**
		* (1, 0, 0)
		*/
		[[nodiscard]] static Vector3 right() { return Vector3(1, 0, 0); }
		/**
		* (-1, 0, 0)
		*/
		[[nodiscard]] static Vector3 left() { return Vector3(-1, 0, 0); }
		/**
		* (0, 0, 0)
		*/
		[[nodiscard]] static Vector3 zero() { return Vector3(0, 0, 0); }
		/**
		* (1, 1, 1)
		*/
		[[nodiscard]] static Vector3 one() { return Vector3(1, 1, 1); }
		/**
		(0, 0, -1)
		*/
		[[nodiscard]] static Vector3 back() { return Vector3(0, 0, -1); }
		/**
		(0, 0, 1)
		*/
		[[nodiscard]] static Vector3 forward() { return Vector3(0, 0, 1); }
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
		* Access the x, y, z components using [0], [1], [2] respectively.
		*
		* \exception invalid_argument If the axis is higher than the dimensions supported by Vector3, or if it's below 0.
		*/
		[[nodiscard]] float& getAxis(const int& axis);
		/**
		* Returns a unit (length = 1) vector with the same direction as this vector.
		*/
		[[nodiscard]] Vector3 getNormalized() const;
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
		[[nodiscard]] static float distance(const Vector3& vec, const Vector3& vec2);
		/**
		* Dot product of the two given vectors
		*/
		[[nodiscard]] static float dot(const Vector3& vec, const Vector3& vec2);
		/**
		* Returns the distance to the given vector
		*/
		[[nodiscard]] float distance(const Vector3& vec) const;
		/**
		* Dot product between the vector and the given vector
		*/
		[[nodiscard]] float dot(const Vector3& vec) const;
		/**
		* Linearly interpolates between two vectors.
		* \param a The start point
		* \param b The destination
		* \param t interpolate rate (0-1)
		*/
		[[nodiscard]] static Vector3 lerp(const Vector3& a, const Vector3& b, const float& t);

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		 */
		[[nodiscard]] float& operator[](const int& value);
		[[nodiscard]] bool operator==(const Vector3& vec) const;
		[[nodiscard]] bool operator!=(const Vector3& vec) const;
		[[nodiscard]] Vector3 operator*(const float& multiplier) const;
		[[nodiscard]] Vector3 operator*(const Vector3& vec) const;
		[[nodiscard]] Vector3 operator/(const float& divider) const;
		[[nodiscard]] Vector3 operator/(const Vector3& divider) const;
		[[nodiscard]] Vector3 operator+(const Vector3& vec) const;
		[[nodiscard]] Vector3 operator-(const Vector3& vec) const;
		
		void operator-=(const Vector3& vector);
		void operator+=(const Vector3& vector);
		void operator *=(const Vector3& vector);
		void operator *=(const float& value);

		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator <(const Vector3& vec) const;
		/**
		 * Operator used to order vectors in maps/dictionaries.
		 */
		[[nodiscard]] bool operator >(const Vector3& vec) const;

		/**
		 * Convert this instance to a string describing the properties
		 */
		[[nodiscard]] String toString() const;

		/**
		 * Convert to an array of 3 floats.
		 */
		[[nodiscard]] std::array<float, 3> toArray() const { return { x, y, z }; }

		/**
		 * Convert to any given type, assuming it has a (float, float, float) constructor.
		 */
		template<typename T>
		[[nodiscard]] constexpr T convert() const { return T{ x, y, z }; }

		/**
		 * Convert any given type to a Vector3, assuming it has an x, y, and z field.
		 */
		template<typename T>
		[[nodiscard]] static constexpr Vector3 convert(T* vec) { return Vector3{ vec->x, vec->y, vec->z }; }

		/**
		 * Convert any given type to a Vector3, assuming it has an x, y, and z field.
		 */
		template<typename T>
		[[nodiscard]] static constexpr Vector3 convert(const T& vec) { return Vector3{ vec.x, vec.y, vec.z }; }
	};

	inline void to_json(nlohmann::json& j, const Vector3& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["z"] = p.z;
		j["typeID"] = Type<Vector3>::fullName();
	}

	inline void from_json(const nlohmann::json& j, Vector3& p) {
		p.x = j.value("x", 0.0f);
		p.y = j.value("y", 0.0f);
		p.z = j.value("z", 0.0f);
	}

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	[[nodiscard]] Vector3 operator*(const float& multiplier, const Vector3& vector);

	static_assert(sizeof(Vector3) == 3 * sizeof(float), "Vector3 shouldn't contain anything else than 3 floats");
}
