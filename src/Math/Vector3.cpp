#include "Vector3.h"
#include <string>
#include <stdexcept>

namespace Tristeon
{
	Vector3::Vector3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}

	float& Vector3::getAxis(const int& axis)
	{
		switch (axis)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::invalid_argument("Axis is out of range, using vector3's index 2 is the maximum");
		}
	}

	Vector3 Vector3::getNormalized() const
	{
		//Normalize a copy of our vector
		Vector3 output(x, y, z);
		output.normalize();
		return output;
	}

	void Vector3::normalize()
	{
		if (x == 0 && y == 0 && z == 0)
			return;

		//Normalize
		float const magnitude = getLength();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	void Vector3::scale(const float& multiplier)
	{
		x *= multiplier;
		y *= multiplier;
		z *= multiplier;
	}

	float Vector3::getLength() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	float Vector3::getSqrLength() const
	{
		return x * x + y * y + z * z;
	}

	float Vector3::dot(const Vector3& vec, const Vector3& vec2)
	{
		Vector3 const c = vec * vec2;
		return c.x + c.y + c.z;
	}

	float Vector3::distance(const Vector3& vec, const Vector3& vec2)
	{
		const auto difference = vec2 - vec;
		return difference.getLength();
	}

	float Vector3::distance(const Vector3& vec) const
	{
		const auto difference = vec - Vector3(x, y, z);
		return difference.getLength();
	}

	float Vector3::dot(const Vector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, const float& t)
	{
		if (b - a == Vector3(0, 0, 0)) return b; //Positions are equal

		auto const interpolation = a.distance(b) * t; //Get interpolation value
		auto const linearDirection = (b - a).getNormalized(); //Get direction
		return linearDirection * interpolation + a; //Pos = direction * distance + start
	}

	bool Vector3::operator==(const Vector3& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool Vector3::operator!=(const Vector3& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	Vector3 Vector3::operator*(const float& multiplier) const
	{
		return { x * multiplier, y * multiplier, z * multiplier };
	}

	Vector3 Vector3::operator*(const Vector3& vec) const
	{
		return { x * vec.x, y * vec.y, z * vec.z };
	}

	Vector3 Vector3::operator/(const float& divider) const
	{
		return Vector3(x / divider, y / divider, z / divider);
	}

	Vector3 Vector3::operator/(const Vector3& divider) const
	{
		return Vector3(x / divider.x, y / divider.y, z / divider.z);
	}

	Vector3 Vector3::operator+(const Vector3& vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 Vector3::operator-(const Vector3& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	float& Vector3::operator[](const int& value)
	{
		return getAxis(value);
	}

	void Vector3::operator-=(const Vector3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
	}

	void Vector3::operator+=(const Vector3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
	}

	void Vector3::operator*=(const Vector3& vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
	}

	void Vector3::operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
	}

	bool Vector3::operator<(const Vector3& vec) const
	{
		if (x < vec.x) return true;
		if (x > vec.x) return false;

		if (y < vec.y) return true;
		if (y > vec.y) return false;

		return z < vec.z;
	}

	bool Vector3::operator>(const Vector3& vec) const
	{
		if (x > vec.x) return true;
		if (x < vec.x) return false;

		if (y > vec.y) return true;
		if (y < vec.y) return false;

		return z > vec.z;
	}

	std::string Vector3::toString() const
	{
		return "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " }";
	}

	Vector3 operator*(const float& multiplier, Vector3 const& vector)
	{
		return vector * multiplier;
	}
}