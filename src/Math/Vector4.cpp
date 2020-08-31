#include "Vector4.h"
#include <string>
#include <stdexcept>

namespace Tristeon
{
	Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	const Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
	const Vector4 Vector4::one = Vector4(1, 1, 1, 1);

	float& Vector4::getAxis(const int& axis)
	{
		switch (axis)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::invalid_argument("Axis is out of range, using Vector4's index 2 is the maximum");
		}
	}

	Vector4 Vector4::getNormalized() const
	{
		//Normalize a copy of our vector
		Vector4 output(x, y, z, w);
		output.normalize();
		return output;
	}

	void Vector4::normalize()
	{
		if (x == 0 && y == 0 && z == 0 && w == 0)
			return;

		//Normalize
		float const magnitude = getLength();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		w /= magnitude;
	}

	void Vector4::scale(float multiplier)
	{
		x *= multiplier;
		y *= multiplier;
		z *= multiplier;
		w *= multiplier;
	}

	float Vector4::getLength() const
	{
		return sqrt(getSqrLength());
	}

	float Vector4::getSqrLength() const
	{
		return x * x + y * y + z * z + w * w;
	}

	float Vector4::dot(Vector4 a, Vector4 b)
	{
		Vector4 const c = a * b;
		return c.x + c.y + c.z + c.w;
	}

	float Vector4::distance(Vector4 a, Vector4 b)
	{
		Vector4 difference = b - a;
		return difference.getLength();
	}

	float Vector4::distance(Vector4 other) const
	{
		Vector4 difference = other - Vector4(x, y, z, w);
		return difference.getLength();
	}

	float Vector4::dot(Vector4 other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	Vector4 Vector4::lerp(Vector4 a, Vector4 b, float t)
	{
		if (b - a == Vector4(0, 0, 0, 0)) return b; //Positions are equal

		float const interpolation = a.distance(b) * t; //Get interpolation value
		Vector4 const linearDirection = (b - a).getNormalized(); //Get direction
		return linearDirection * interpolation + a; //Pos = direction * distance + start
	}

	bool Vector4::operator==(const Vector4 & other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Vector4::operator!=(const Vector4 & other) const
	{
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Vector4 Vector4::operator*(const float& multiplier) const
	{
		Vector4 result(x * multiplier, y * multiplier, z * multiplier, w * multiplier);
		return result;
	}

	Vector4 Vector4::operator*(const Vector4 & other) const
	{
		Vector4 result(x * other.x, y * other.y, z * other.z, w * other.w);
		return result;
	}

	Vector4 Vector4::operator/(const float& divider) const
	{
		return Vector4(x / divider, y / divider, z / divider, w / divider);
	}

	Vector4 Vector4::operator/(const Vector4 & divider) const
	{
		return Vector4(x / divider.x, y / divider.y, z / divider.z, w / divider.w);
	}

	Vector4 Vector4::operator+(const Vector4 & other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::operator-(const Vector4 & other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	float& Vector4::operator[](const int& value)
	{
		return getAxis(value);
	}

	void Vector4::operator-=(const Vector4 & vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
	}

	void Vector4::operator+=(const Vector4 & vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
	}

	void Vector4::operator*=(const Vector4 & vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		w *= vector.w;
	}

	void Vector4::operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
	}

	bool Vector4::operator<(const Vector4& vec) const
	{
		if (x < vec.x) return true;
		if (x > vec.x) return false;

		if (y < vec.y) return true;
		if (y > vec.y) return false;

		if (z < vec.z) return true;
		if (z > vec.z) return false;

		return w < vec.w;
	}

	bool Vector4::operator>(const Vector4& vec) const
	{
		if (x > vec.x) return true;
		if (x < vec.x) return false;

		if (y > vec.y) return true;
		if (y < vec.y) return false;

		if (z > vec.z) return true;
		if (z < vec.z) return false;

		return w > vec.w;
	}

	std::string Vector4::toString() const
	{
		return "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + " }";
	}

	Vector4 operator*(const float& multiplier, Vector4 vector)
	{
		return vector * multiplier;
	}
}
