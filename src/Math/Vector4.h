#pragma once
#include <array>

namespace Tristeon
{
	/**
	* Vector4 interface, describes a 4D point or movement and implements math operations to modify said point/movement.
	*/
	struct Vector4 final
	{
		/**
		 * Creates a (xyzw, xyzw, xyzw) Vector4
		 * \param xyzw The x, y, z and w component of this vector
		 */
		explicit Vector4(float xyzw);
		/**
		 * Creates a (x, y, z, w) Vector4
		 */
		Vector4(float x = 0, float y = 0, float z = 0, float w = 0);

#pragma region const static vectors
		/**
		(1, 1, 1)
		*/
		const static Vector4 one;
		const static Vector4 zero;
#pragma endregion

		float x;
		float y;
		float z;
		float w;

		/**
		* Access the x, y, z, w components using [0], [1], [2], [3] respectively.
		*
		* \exception invalid_argument If the axis is higher than the dimensions supported by Vector4, or if it's below 0.
		*/
		float& getAxis(const int& axis);
		/**
		* Returns a unit (length = 1) vector with the same direction as this vector.
		*/
		Vector4 getNormalized() const;
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
		static float distance(Vector4 a, Vector4 b);
		/**
		* Dot product of the two given vectors
		*/
		static float dot(Vector4 a, Vector4 b);
		/**
		* Returns the distance to the given vector
		*/
		float distance(Vector4 other) const;
		/**
		* Dot product between the vector and the given vector
		*/
		float dot(Vector4 other) const;
		/**
		* Linearly interpolates between two vectors.
		* \param a The start point
		* \param b The destination
		* \param t interpolate rate (0-1)
		*/
		static Vector4 lerp(Vector4 a, Vector4 b, float t);

		/**
		* Access the x, y, z components using [0], [1], [2], [3] respectively.
		 */
		float& operator[](const int& value);

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;
		Vector4 operator*(const float& multiplier) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator/(const float& divider) const;
		Vector4 operator/(const Vector4& divider) const;
		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;
		void operator-=(const Vector4& vector);
		void operator+=(const Vector4& vector);
		void operator *=(const Vector4& vector);
		void operator *=(const float& value);

		/**
		 * Convert to a string describing the vector's properties
		 */
		std::string toString() const;

		std::array<float, 4> toArray() const { return { x, y, z, w }; }

		template<typename T>
		constexpr T convert() { return T{ x, y, z, w }; }

		template<typename T>
		static constexpr Vector4 convert(T* vec) { return Vector4{ vec->x, vec->y, vec->z, vec->w }; }

		template<typename T>
		static constexpr Vector4 convert(T const& vec) { return Vector4{ vec.x, vec.y, vec.z, vec.w }; }
	};

	/**
	* Multiplies the x,y,z components with the given multiplier
	*/
	Vector4 operator*(const float& multiplier, Vector4 vector);

	static_assert(sizeof(Vector4) == 4 * sizeof(float), "Vector4 shouldn't contain anything else than 4 floats");
}