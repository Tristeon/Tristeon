#pragma once
#include <array>
#include <json.h>

namespace Tristeon
{
	/**
	* Vector3 interface, describes a 3D point or movement and implements math operations to modify said point/movement.
	*/
	struct Vector3 final
	{
	public:
		/**
		 * Creates a (xyz, xyz, xyz) Vector3
		 * \param xyz The x, y and z component of this vector
		 */
		explicit Vector3(float xyz);
		/**
		 * Creates a (x, y, z) Vector3
		 */
		Vector3(float x = 0, float y = 0, float z = 0);

#pragma region const static vectors
		/**
		(0, 0, -1)
		*/
		const static Vector3 back;
		/**
		(0, -1, 0)
		*/
		const static Vector3 down;
		/**
		(0, 0, 1)
		*/
		const static Vector3 forward;
		/**
		(-1, 0, 0)
		*/
		const static Vector3 left;
		/**
		(1, 0, 0)
		*/
		const static Vector3 right;
		/**
		(0, 1, 0)
		*/
		const static Vector3 up;
		/**
		(1, 1, 1)
		*/
		const static Vector3 one;
		const static Vector3 zero;
#pragma endregion

		float x;
		float y;
		float z;

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		*
		* \exception invalid_argument If the axis is higher than the dimensions supported by Vector3, or if it's below 0.
		*/
		float& getAxis(const int& axis);
		/**
		* Returns a unit (length = 1) vector with the same direction as this vector.
		*/
		Vector3 getNormalized() const;
		/**
		* Normalizes the vector into a unit vector
		*/
		void normalize();
		/**
		* Scales vector by the multiplier
		*/
		void scale(float multiplier);
		/**
		Returns the magnitude of the vector
		*/
		float getLength() const;
		/**
		* Returns the magnitude of the vector squared (It's more optimized than normal getLength)
		*/
		float getSqrLength() const;
		/**
		*Returns the distance between two vectors
		*/
		static float distance(Vector3 vec, Vector3 vec2);
		/**
		* Dot product of the two given vectors
		*/
		static float dot(Vector3 vec, Vector3 vec2);
		/**
		* Returns the distance to the given vector
		*/
		float distance(Vector3 vec) const;
		/**
		* Dot product between the vector and the given vector
		*/
		float dot(Vector3 vec) const;
		/**
		* Linearly interpolates between two vectors.
		* \param a The start point
		* \param b The destination
		* \param t interpolate rate (0-1)
		*/
		static Vector3 lerp(Vector3 a, Vector3 b, float t);

		/**
		* Access the x, y, z components using [0], [1], [2] respectively.
		 */
		float& operator[](const int& value);
		bool operator==(const Vector3& vec) const;
		bool operator!=(const Vector3& vec) const;
		Vector3 operator*(const float& multiplier) const;
		Vector3 operator*(const Vector3& vec) const;
		Vector3 operator/(const float& divider) const;
		Vector3 operator/(const Vector3& divider) const;
		Vector3 operator+(const Vector3& vec) const;
		Vector3 operator-(const Vector3& vec) const;
		void operator-=(const Vector3& vector);
		void operator+=(const Vector3& vector);
		void operator *=(const Vector3& vector);
		void operator *=(const float& value);

		/**
		 * Convert this instance to a string describing the properties
		 */
		std::string toString() const;

		std::array<float, 3> toArray() const { return { x, y, z }; }

		template<typename T>
		constexpr T convert() const { return T{ x, y, z }; }

		template<typename T>
		static constexpr Vector3 convert(T* vec) { return Vector3{ vec->x, vec->y, vec->z }; }

		template<typename T>
		static constexpr Vector3 convert(T const& vec) { return Vector3{ vec.x, vec.y, vec.z }; }
	};

	inline void to_json(nlohmann::json& j, const Vector3& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["z"] = p.z;
	}

	inline void from_json(const nlohmann::json& j, Vector3& p) {
		p.x = j["x"];
		p.y = j["y"];
		p.z = j["z"];
	}

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	Vector3 operator*(const float& multiplier, Vector3 const& vector);

	static_assert(sizeof(Vector3) == 3 * sizeof(float), "Vector3 shouldn't contain anything else than 3 floats");
}
