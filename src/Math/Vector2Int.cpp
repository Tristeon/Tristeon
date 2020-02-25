#include "Vector2Int.h"
#include <string>

namespace Tristeon
{
	Vector2Int::Vector2Int(int xy) : x(xy), y(xy) {}

	Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}

	Vector2Int::Vector2Int(Vector2 vec2) : x(static_cast<int>(vec2.x)), y(static_cast<int>(vec2.y)) { }

	int Vector2Int::getAxis(const int& axis) const
	{
		switch (axis)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			return 0;
		}
	}

	void Vector2Int::scale(int multiplier)
	{
		x *= multiplier;
		y *= multiplier;
	}

	float Vector2Int::getLength() const
	{
		return sqrt(static_cast<float>(x * x + y * y));
	}

	float Vector2Int::getSqrLength() const
	{
		return static_cast<float>(x * x + y * y);
	}

	int Vector2Int::dot(Vector2Int a, Vector2Int b)
	{
		return a.x* b.x + a.y * b.y;
	}

	float Vector2Int::distance(Vector2Int a, Vector2Int b)
	{
		Vector2Int const difference = b - a;
		return difference.getLength();
	}

	float Vector2Int::distance(Vector2Int vec) const
	{
		Vector2Int const difference = vec - Vector2Int(x, y);
		return difference.getLength();
	}

	float Vector2Int::dot(Vector2Int vec) const
	{
		return static_cast<float>(x * vec.x + y * vec.y);
	}

	bool Vector2Int::operator==(const Vector2Int & vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool Vector2Int::operator!=(const Vector2Int & vec) const
	{
		return x != vec.x || y != vec.y;
	}

	Vector2Int Vector2Int::operator*(const Vector2Int & vec) const
	{
		return { x * vec.x, y * vec.y };
	}

	Vector2Int Vector2Int::operator*(const int& multiplier) const
	{
		return { x * multiplier, y * multiplier };
	}

	Vector2Int Vector2Int::operator/(const int& divider) const
	{
		return { x / divider, y / divider };
	}

	Vector2Int Vector2Int::operator/(const Vector2Int & divider) const
	{
		return { x / divider.x, y / divider.y };
	}

	Vector2Int Vector2Int::operator+(const Vector2Int & vec) const
	{
		return { x + vec.x, y + vec.y };
	}

	Vector2Int Vector2Int::operator-(const Vector2Int & vec) const
	{
		return { x - vec.x, y - vec.y };
	}

	int Vector2Int::operator[](const int& value) const
	{
		return getAxis(value);
	}

	void Vector2Int::operator-=(const Vector2Int & vector)
	{
		x -= vector.x;
		y -= vector.y;
	}

	void Vector2Int::operator+=(const Vector2Int & vector)
	{
		x += vector.x;
		y += vector.y;
	}

	void Vector2Int::operator*=(const Vector2Int & vector)
	{
		x *= vector.x;
		y *= vector.y;
	}

	void Vector2Int::operator*=(const int& value)
	{
		x *= value;
		y *= value;
	}

	std::string Vector2Int::toString() const
	{
		return "{ " + std::to_string(x) + ", " + std::to_string(y) + " }";
	}

	Vector2Int operator*(const int& multiplier, Vector2Int const& vector)
	{
		return vector * multiplier;
	}
}
