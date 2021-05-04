#include <cstdint> //Required by GCC
#include <climits> //Required by GCC
#include <doctest/doctest.h>
#include <Math/Vector.h>

TEST_CASE("Vector math")
{
	SUBCASE("Constructors")
	{
		Tristeon::Vector vec{ 5, 10 };

		SUBCASE("Constructor correctness")
		{
			REQUIRE(vec == Tristeon::Vector(5, 10));
			REQUIRE(!(vec != Tristeon::Vector(5, 10)));

			REQUIRE(vec != Tristeon::Vector(5, 0));
			REQUIRE(vec != Tristeon::Vector(0, 10));
			REQUIRE(vec != Tristeon::Vector(10, 5));
			REQUIRE(vec != Tristeon::Vector(0, 0));
		}
		SUBCASE("Assignment")
		{
			Tristeon::Vector vec2 = vec;
			REQUIRE(vec2.x == 5);
			REQUIRE(vec2.y == 10);
			REQUIRE(vec == vec2);
		}
		SUBCASE("Copy constructor")
		{
			Tristeon::Vector vec3(vec);
			REQUIRE(vec3.x == 5);
			REQUIRE(vec3.y == 10);
			REQUIRE(vec == vec3);
		}
		SUBCASE("Explicit type cast")
		{
			Tristeon::Vector vec4{ 0.5, 0.5 };
			Tristeon::VectorI vec4i = (Tristeon::VectorI)vec4;
			REQUIRE(vec4i.x == 0);
			REQUIRE(vec4i.y == 0);
			REQUIRE(vec4 != vec4i);
		}
	}
	SUBCASE("Mathematical operators")
	{
		SUBCASE("Vector multiplied with Vector")
		{
			Tristeon::Vector vec{ 25, 50 };
			Tristeon::Vector other(5, 0.5);

			SUBCASE("Vector * Vector")
			{
				REQUIRE((vec * other) == Tristeon::Vector(125, 25));
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
				REQUIRE(other.x == 5);
				REQUIRE(other.y == 0.5);
			}

			SUBCASE("Vector *= Vector")
			{
				vec *= other;
				REQUIRE(vec.x == 125);
				REQUIRE(vec.y == 25);

				REQUIRE(other.x == 5);
				REQUIRE(other.y == 0.5);
			}

			SUBCASE("VectorI *= Vector")
			{
				vec *= other;

				REQUIRE(other.x == 5);
				REQUIRE(other.y == 0.5);
			}
		}

		SUBCASE("Vector multiplied with float")
		{
			Tristeon::Vector vec{ 25, 50 };
			SUBCASE("Vector * float")
			{
				REQUIRE((vec * -1) == Tristeon::Vector{ -25, -50 });
				REQUIRE((vec * 0.5) == Tristeon::Vector{ 12.5, 25 });
				REQUIRE((vec * 2) == Tristeon::Vector{ 50, 100 });
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
			}
			SUBCASE("Vector *= float")
			{
				vec *= 0.5f;
				REQUIRE(vec.x == 12.5);
				REQUIRE(vec.y == 25);
			}

			SUBCASE("VectorI * float")
			{
				Tristeon::VectorI vec{ 25, 50 };
				REQUIRE((vec * 0.5f) == Tristeon::VectorI{ 12, 25 });
			}
		}

		SUBCASE("Vector divided by Vector")
		{
			Tristeon::Vector vec{ 25, 50 };
			Tristeon::Vector other(5, 100);

			SUBCASE("Vector * Vector")
			{
				REQUIRE((vec / other) == Tristeon::Vector(5, 0.5f));
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
				REQUIRE(other.x == 5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("Vector /= Vector")
			{
				vec /= other;
				REQUIRE(vec.x == 5);
				REQUIRE(vec.y == 0.5f);

				REQUIRE(other.x == 5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("VectorI /= Vector")
			{
				Tristeon::VectorI vec{ 25, 50 };
				Tristeon::Vector other(5, 100);
				vec /= other;

				REQUIRE(vec.x == 5);
				REQUIRE(vec.y == 0);

				REQUIRE(other.x == 5);
				REQUIRE(other.y == 100);
			}
		}

		SUBCASE("Vector divided by float")
		{
			Tristeon::Vector vec{ 25, 50 };
			SUBCASE("Vector / float")
			{
				REQUIRE((vec / -1) == Tristeon::Vector{ -25, -50 });
				REQUIRE((vec / 0.5) == Tristeon::Vector{ 50, 100 });
				REQUIRE((vec / 2) == Tristeon::Vector{ 12.5, 25 });
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
			}
			SUBCASE("Vector /= float")
			{
				vec /= 2.0f;
				REQUIRE(vec.x == 12.5);
				REQUIRE(vec.y == 25);
			}

			SUBCASE("VectorI / float")
			{
				Tristeon::VectorI vec{ 25, 50 };
				REQUIRE((vec / 2.0f) == Tristeon::VectorI{ 12, 25 });
			}
		}

		SUBCASE("Vector addition")
		{
			Tristeon::Vector vec{ 25, 50 };
			Tristeon::Vector other(5.5, 100);

			SUBCASE("Vector + Vector")
			{
				REQUIRE((vec + other) == Tristeon::Vector(30.5, 150));
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("Vector += Vector")
			{
				vec += other;
				REQUIRE(vec.x == 30.5);
				REQUIRE(vec.y == 150);

				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("VectorI += Vector")
			{
				Tristeon::VectorI vec{ 25, 50 };
				Tristeon::Vector other(5.5, 100);
				vec += other;

				REQUIRE(vec.x == 30);
				REQUIRE(vec.y == 150);

				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}
		}

		SUBCASE("Vector subtraction")
		{
			Tristeon::Vector vec{ 25, 50 };
			Tristeon::Vector other(5.5, 100);

			SUBCASE("Vector - Vector")
			{
				REQUIRE((vec - other) == Tristeon::Vector(19.5, -50));
				REQUIRE(vec.x == 25);
				REQUIRE(vec.y == 50);
				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("Vector -= Vector")
			{
				vec -= other;
				REQUIRE(vec.x == 19.5);
				REQUIRE(vec.y == -50);

				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}

			SUBCASE("VectorI -= Vector")
			{
				Tristeon::VectorI vec{ 25, 50 };
				Tristeon::Vector other(5.5, 100);
				vec -= other;

				REQUIRE(vec.x == 19);
				REQUIRE(vec.y == -50);

				REQUIRE(other.x == 5.5);
				REQUIRE(other.y == 100);
			}
		}

		SUBCASE("Vector [] operator")
		{
			Tristeon::Vector vec{ 5.2f, -10 };
			REQUIRE(vec.x == vec[0]);
			REQUIRE(vec.y == vec[1]);
			REQUIRE(vec[0] == 5.2f);
			REQUIRE(vec[1] == -10);
			REQUIRE_THROWS(vec[-1]);
			REQUIRE_THROWS(vec[2]);
			REQUIRE_THROWS(vec[INT_MAX]);
			
			vec[0] = 25.18f;
			REQUIRE(vec[0] == 25.18f);

			vec[1] = -32;
			REQUIRE(vec[1] == -32);
		}
	}
	SUBCASE("Comparison operators")
	{
		Tristeon::Vector a{ -5, 5 };
		Tristeon::Vector b{ 0, 0 };
		Tristeon::Vector c{ 0, -5 };
		Tristeon::Vector d{ 5, -5 };

		REQUIRE(a < b);
		REQUIRE(b > a);
		REQUIRE(c > a);
		REQUIRE(c < b);
		REQUIRE(d > a);
		REQUIRE(d > c);
		REQUIRE(!(b > b));

		REQUIRE(b == b);
		REQUIRE(c != b);
		REQUIRE(a == Tristeon::Vector{ -5, 5 });
	}
	SUBCASE("Mathematical functions")
	{
		SUBCASE("Magnitude")
		{
			//No tests for decimal numbers because checking them would be difficult to get accurate
			Tristeon::Vector a{ 3, 4 };
			REQUIRE(a.magnitude() == 5);

			REQUIRE(a.magnitudeSquared() == (a.magnitude() * a.magnitude()));
			REQUIRE(a.magnitudeSquared() == 25);

			Tristeon::Vector b{ -3, -4 };
			REQUIRE(b.magnitude() == 5);

			Tristeon::Vector c{ 0, 0 };
			REQUIRE(c.magnitude() == 0);

			Tristeon::VectorI d{ 3, 4 };
			REQUIRE(d.magnitude() == 5);
		}

		SUBCASE("Normalization")
		{
			Tristeon::Vector a{ -5, 24 };
			a.normalize();

			REQUIRE(a.magnitude() == 1);
			REQUIRE(a.x < 0);
			REQUIRE(a.y > 0);

			Tristeon::Vector b{ 0, 0 };
			b.normalize();
			REQUIRE(b.magnitude() == 0);
			REQUIRE(b.x == 0);
			REQUIRE(b.y == 0);
		}

		SUBCASE("Rounding")
		{
			Tristeon::Vector a{ 0.6f, -0.4f };
			a.round();
			REQUIRE(a.x == 1);
			REQUIRE(a.y == 0);

			Tristeon::Vector b{ 0.6f, -0.4f };
			b.ceil();
			REQUIRE(b.x == 1);
			REQUIRE(b.y == -0);

			Tristeon::Vector c{ 0.6f, -0.4f };
			c.floor();
			REQUIRE(c.x == 0);
			REQUIRE(c.y == -1);
		}

		SUBCASE("Dot")
		{
			REQUIRE(Tristeon::Vector::dot({ 5, 0 }, { -5, 0 }) < 0);
			REQUIRE(Tristeon::Vector::dot({ 5, 0 }, { 2, 1 } ) > 0);
			REQUIRE(Tristeon::Vector::dot({ 5, 0 }, { 0, 5 }) == 0);
		}

		SUBCASE("Angles and rotation")
		{
			REQUIRE(Tristeon::Vector::angleBetween({ 5, 0 }, { 0, 1 }) == -90);
			REQUIRE(Tristeon::Vector::angleBetween({ 0, 1 }, { 5, 0 }) == 90);
			REQUIRE(Tristeon::Vector::angleBetween({ 1, 0 }, { 1, 0 }) == 0);
			REQUIRE(Tristeon::Vector::angleBetween({ -1, 0 }, { 1, 0 }) == 180);
			
			REQUIRE(Tristeon::Vector{ 1, 0 }.angle() == 0);
			REQUIRE(Tristeon::Vector{ 0, 1 }.angle() == -90);
			REQUIRE(Tristeon::Vector{ 0, -1 }.angle() == 90);
			REQUIRE(Tristeon::Vector{ -1, 0 }.angle() == -180);
			REQUIRE(Tristeon::Vector{ 5, 5 }.angle() == -45);
			REQUIRE(Tristeon::Vector{ 5, -5 }.angle() == 45);
			REQUIRE(Tristeon::Vector{ -5, -5 }.angle() == 135);
			REQUIRE(Tristeon::Vector{ -5, 5 }.angle() == -135);
			REQUIRE(Tristeon::Vector{ 0, 0 }.angle() == 0);

			REQUIRE(Tristeon::VectorI{ 0, 1 }.rotate(90) == Tristeon::VectorI{ 1, 0 });
			REQUIRE(Tristeon::VectorI{ 1, 0 }.rotate(90) == Tristeon::VectorI{ 0, -1 });

			REQUIRE(Tristeon::VectorI{ 0, 1 }.rotate(-90) == Tristeon::VectorI{ -1, 0 });
			REQUIRE(Tristeon::VectorI{ -1, 0 }.rotate(-90) == Tristeon::VectorI{ 0, -1 });

			REQUIRE(Tristeon::Vector{ 0, 0 }.rotate(23) == Tristeon::Vector{ 0, 0 });
		}

		SUBCASE("Distance")
		{
			REQUIRE(Tristeon::Vector::distance({ 0, 0 }, { 0, 0 }) == 0);
			REQUIRE(Tristeon::Vector::distance({ 0, 0 }, { 0, 3 }) == 3);
			REQUIRE(Tristeon::Vector::distance({ 1, 5 }, { 4, 9 }) == 5);

			REQUIRE(Tristeon::Vector::distanceSquared({ 0, 0 }, { 0, 0 }) == 0);
			REQUIRE(Tristeon::Vector::distanceSquared({ 0, 0 }, { 0, 3 }) == 9);
			REQUIRE(Tristeon::Vector::distanceSquared({ 1, 5 }, { 4, 9 }) == 25);
		}

		SUBCASE("Lerp")
		{
			REQUIRE(Tristeon::Vector::lerp({ 0, 0 }, { 0, 5 }, 0.5) == Tristeon::Vector{ 0, 2.5 });
			REQUIRE(Tristeon::Vector::lerp({ -5, -5 }, { 5, 0 }, 0.5) == Tristeon::Vector{ 0, -2.5 });
			REQUIRE(Tristeon::Vector::lerp({ 0, 0 }, { 0, 0 }, 0.5) == Tristeon::Vector{ 0, 0 });

			REQUIRE(Tristeon::Vector::lerp({ 0, 0 }, { 0, 5 }, 0) == Tristeon::Vector{ 0, 0 });
			REQUIRE(Tristeon::Vector::lerp({ 0, 0 }, { 0, 5 }, 1) == Tristeon::Vector{ 0, 5 });
		}
	}
}