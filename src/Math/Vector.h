#pragma once
#include <cmath>
#include <json.h>
#include <stdexcept>
#include <Standard/String.h>

#include "Serialization/Type.h"

namespace Tristeon
{
	template <typename Type = float>
	struct VectorType
	{
		Type x{};
		Type y{};

#pragma region Constructors
		VectorType() = default;
		~VectorType() = default;

		VectorType(const Type& x, const Type& y) : x(x), y(y) { }

		template<typename T = Type>
		VectorType(const VectorType& copy)
		{
			x = (Type)copy.x;
			y = (Type)copy.y;
		}

		template<typename T = Type>
		VectorType& operator=(const VectorType<T>& copy)
		{
			x = (Type)copy.x;
			y = (Type)copy.y;
			return *this;
		}

		template<typename T>
		explicit operator VectorType<T>() const
		{
			return { (T)x, (T)y };
		}
#pragma endregion

#pragma region Quick Vectors
		/**
		* (0, -1)
		*/
		[[nodiscard]] static VectorType down() { return VectorType(0, -1); }
		/**
		* (0, 1)
		*/
		[[nodiscard]] static VectorType up() { return VectorType(0, 1); }
		/**
		* (1, 0)
		*/
		[[nodiscard]] static VectorType right() { return VectorType(1, 0); }

		/**
		* (-1, 0)
		*/
		[[nodiscard]] static VectorType left() { return VectorType(-1, 0); }

		/**
		* (0, 0)
		*/
		[[nodiscard]] static VectorType zero() { return VectorType(0, 0); }
		/**
		* (1, 1)
		*/
		[[nodiscard]] static VectorType one() { return VectorType(1, 1); }
#pragma endregion
		
#pragma region Operators
		template<typename T = Type>
		[[nodiscard]] bool operator==(const VectorType<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		template<typename T = Type>
		[[nodiscard]] bool operator!=(const VectorType<T>& other) const
		{
			return x != other.x || y != other.y;
		}

		template<typename T = Type>
		VectorType<Type> operator*=(const VectorType<T>& other)
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator*(const VectorType<T>& other) const
		{
			return { x * other.x, y * other.y };
		}

		template<typename T = Type>
		VectorType<Type> operator*=(const T& multiplier)
		{
			x *= multiplier;
			y *= multiplier;
			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator*(const T& multiplier) const
		{
			return { (Type)(x * multiplier), (Type)(y * multiplier) };
		}

		template<typename T = Type>
		VectorType<Type> operator/=(const VectorType<T>& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator/(const VectorType<T>& other) const
		{
			return { x / other.x, y / other.y };
		}

		template<typename T = Type>
		VectorType<Type> operator/=(const T& divider)
		{
			x /= divider;
			y /= divider;

			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator/(const T& divider) const
		{
			return { (Type)(x / divider), (Type)(y / divider) };
		}

		template<typename T = Type>
		VectorType<Type> operator+=(const VectorType<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator+(const VectorType<T>& other) const
		{
			return { x + other.x, y + other.y };
		}

		template<typename T = Type>
		VectorType<Type> operator-=(const VectorType<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		template<typename T = Type>
		[[nodiscard]] VectorType operator-(const VectorType<T>& other) const
		{
			return { x - other.x, y - other.y };
		}

		/**
		* @brief Copies the vector and negates the copy's components such that x becomes -x and y becomes -y.
		* For example, { -5, 23 } will become { 5, -23 }. This will also affect 0 and infinity when real numbers are used.
		* 
		* @return Retuns the negated vector.
		*/
		[[nodiscard]] VectorType operator-() const
		{
			return { -x, -y };
		}

		/**
		* @brief Access the vector's components using the [] operator. Returns a reference to the x or y component depending on index being 0 or 1.
		* 
		* @exception Throws std::invalid_argument when index is not 0 or 1.
		* 
		* @return Returns a reference to the x or y component.
		*/
		[[nodiscard]] Type& operator[](const unsigned int& index)
		{
			if (index == 0)
				return x;
			if (index == 1)
				return y;
			throw std::invalid_argument("index used in Vector::operator[] has to be 0 or 1!");
		}

		/**
		 * @brief Operator used for sorting. Vectors are sorted by x value first, and y value second, meaning that { 0, 15 } will come before { 1, 2 } but not before { 0, 14 }
		 * @param other The vector to be compared against.
		 * @return True if the vector is "more" than the other vector (aka is sorted further down the list)
		*/
		template<typename T = Type>
		[[nodiscard]] bool operator<(const VectorType<T>& other) const
		{
			if (x < other.x)
				return true;
			if (x > other.x)
				return false;
			return y < other.y;
		}

		/**
		 * @brief Operator used for sorting. Vectors are sorted by x value first, and y value second, meaning that { 0, 15 } will come before { 1, 2 } but not before { 0, 14 }
		 * @param other The vector to be compared against.
		 * @return True if the vector is "more" than the other vector (aka is sorted further down the list)
		*/
		template<typename T = Type>
		[[nodiscard]] bool operator>(const VectorType<T>& other) const
		{
			if (x > other.x)
				return true;
			if (x < other.x)
				return false;
			return y > other.y;
		}
#pragma endregion

#pragma region Math operations
		
		/**
		 * @brief Returns the magnitude (or length) of the vector.
		 * This value is calculated using the pythagoras algorithm c^2 = (a^2 + b^2) where c == magnitude, a == x and b == y.
		 * The magnitude is never negative, but it may be 0 if both the x and y component are 0
		 * 
		 * @note magnitude() uses a square root calculation which is computationally expensive. Consider using magnitudeSquared() for things like range checks (simply square the range in the check)
		 * 
		 * @return Returns the computed magnitude
		*/
		[[nodiscard]] double magnitude() const
		{
			return (double)sqrt(x * x + y * y);
		}

		/**
		 * @brief Returns the squared magnitude (or squared length) of the vector. This function is less computationally expensive than its more accurate alternative magnitude().
		 * 
		 * This value is calculated using the pythagoras algorithm c^2 = (a^2 + b^2) where c^2 == squaredMagnitude, a == x and b == y.
		 * 
		 * SquaredMagnitude is not an accurate representation of the vector's magnitude, but its value can be used for things like range checks where range can simply be squared (e.g. if (vec.magnitudeSquared() < range * range);)
		 * 
		 * @return Returns the magnitude squared (magnitude ^ 2)
		*/
		[[nodiscard]] double magnitudeSquared() const
		{
			return x * x + y * y;
		}

		/**
		 * @brief Resize the vector so that it maintains its direction but magnitude() becomes 1.
		 * This is commonly used to calculate a purely directional vector for e.g. movement. The function divides the vector's components by its own magnitude.
		 *
		 * @note Magnitude may not become 1 with integer-based vectors because a normalized VectorI might see its components floored to { 0, 0}
		 * @note With integer-based vectors, The vector's direction may not be accurate when normalized.
		 * @note If the vector's magnitude is 0 (aka both x and y are 0) then the magnitude will remain 0 because the vector has no direction.
		 * 
		 * @return Returns itself to enable vector math chaining.
		*/
		VectorType<Type> normalize()
		{
			if (x == 0 && y == 0)
				return *this;

			const auto mag = magnitude();

			x /= mag;
			y /= mag;

			return *this;
		}

		VectorType<Type> floor()
		{
			x = (Type)floorf(x);
			y = (Type)floorf(y);
			return *this;
		}

		VectorType<Type> ceil()
		{
			x = (Type)ceilf(x);
			y = (Type)ceilf(y);
			return *this;
		}

		VectorType<Type> round()
		{
			x = (Type)roundf(x);
			y = (Type)roundf(y);

			return *this;
		}

		template<typename TA = Type, typename TB = Type>
		static double dot(const VectorType<TA>& a, const VectorType<TB>& b)
		{
			return a.x * b.x + a.y * b.y;
		}

		template<typename TA = Type, typename TB = Type>
		static double angleBetween(const VectorType<TA>& a, const VectorType<TB>& b)
		{
			return b.angle() - a.angle();
		}

		double angle() const
		{
			if (x == 0 && y == 0)
			{
				Tristeon::Console::warning("Trying to get the angle of a { 0, 0 } vector. Returning incorrect value of 0");
				return 0;
			}

			return -(atan2(y, x) * 180.0f / 3.14159265f);
		}

		VectorType<Type> rotate(Type degrees)
		{
			if (x == 0 && y == 0)
				return { 0, 0 };
			
			constexpr static auto TO_RAD = (double)(3.14159265 / 180.0);
			const double c = cos(-degrees * TO_RAD);
			const double s = sin(-degrees * TO_RAD);

			const double tempX = x;
			const double tempY = y;

			x = (Type)(tempX * c - tempY * s);
			y = (Type)(tempX * s + tempY * c);
			return *this;
		}

		template<typename TA = Type, typename TB = Type>
		static double distance(const VectorType<TA>& a, const VectorType<TB>& b)
		{
			const VectorType diff = b - a;
			return diff.magnitude();
		}

		template<typename TA = Type, typename TB = Type>
		static double distanceSquared(const VectorType<TA>& a, const VectorType<TB>& b)
		{
			const VectorType diff = b - a;
			return diff.magnitudeSquared();
		}

		template<typename TA = Type, typename TB = Type>
		static VectorType lerp(const VectorType<TA>& a, const VectorType<TB>& b, const float& t)
		{
			if (b == a) return b;
			if (t == 0) return a;
			if (t == 1) return b;

			const auto interpolation = distance(a, b) * t;
			const auto linearDirection = (b - a).normalize();
			return linearDirection * interpolation + a;
		}
#pragma endregion

#pragma region Misc
		[[nodiscard]] String toString() const
		{
			return "{ " + std::to_string(x) + ", " + std::to_string(y) + " }";
		}
#pragma endregion
	};

	template<typename Type = float>
	[[nodiscard]] VectorType<Type> operator*(const float& multiplier, const VectorType<Type>& vector)
	{
		return vector * multiplier;
	}


#pragma region Specializations
	typedef VectorType<float> Vector;
	inline void to_json(nlohmann::json& j, const Vector& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<Vector>::fullName();
	}
	inline void from_json(const nlohmann::json& j, Vector& p) {
		p.x = j.value("x", 0.0f);
		p.y = j.value("y", 0.0f);
	}

	typedef VectorType<double> VectorD;
	inline void to_json(nlohmann::json& j, const VectorD& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<VectorD>::fullName();
	}
	inline void from_json(const nlohmann::json& j, VectorD& p) {
		p.x = j.value("x", 0.0);
		p.y = j.value("y", 0.0);
	}

	typedef VectorType<int> VectorI;
	inline void to_json(nlohmann::json& j, const VectorI& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<VectorI>::fullName();
	}
	inline void from_json(const nlohmann::json& j, VectorI& p) {
		p.x = j.value("x", 0);
		p.y = j.value("y", 0);
	}

	typedef VectorType<unsigned int> VectorU;
	inline void to_json(nlohmann::json& j, const VectorU& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<VectorU>::fullName();
	}
	inline void from_json(const nlohmann::json& j, VectorU& p) {
		p.x = j.value("x", 0u);
		p.y = j.value("y", 0u);
	}

	typedef VectorType<bool> VectorB;
	inline void to_json(nlohmann::json& j, const VectorB& p) {
		j["x"] = p.x;
		j["y"] = p.y;
		j["typeID"] = Type<VectorB>::fullName();
	}
	inline void from_json(const nlohmann::json& j, VectorB& p) {
		p.x = j.value("x", false);
		p.y = j.value("y", false);
	}
#pragma endregion 
}
