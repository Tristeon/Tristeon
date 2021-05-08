#include "Math/Math.h"
#include <doctest/doctest.h>
#include <cstdint>
#include <climits>

TEST_CASE("Math")
{
	SUBCASE("Clamp")
	{
		REQUIRE(Tristeon::Math::clamp(0, -1, 1) == 0);
		REQUIRE(Tristeon::Math::clamp(0, 0, 0) == 0);
		REQUIRE_THROWS(Tristeon::Math::clamp(0, 1, -1));
		REQUIRE(Tristeon::Math::clamp(-5, 0, 1) == 0);
		REQUIRE(Tristeon::Math::clamp(5, 0, 1) == 1);
		REQUIRE(Tristeon::Math::clamp(0.3f, 0.1f, 0.2f) == 0.2f);

		REQUIRE(Tristeon::Math::clamp01(-1) == 0);
		REQUIRE(Tristeon::Math::clamp01(0.3f) == 0.3f);
		REQUIRE(Tristeon::Math::clamp01(1.5) == 1);
	}

	SUBCASE("Orbit")
	{
		REQUIRE(Tristeon::Math::orbit({ 3, 3 }, { 3, 0 }, 0) == Tristeon::Vector{ 6, 3 });
		REQUIRE(Tristeon::Math::orbit({ 3, 3 }, { 3, 0 }, 90) == Tristeon::Vector{ 3, 0 });
		REQUIRE(Tristeon::Math::orbit({ 3, 3 }, { 3, 0 }, -90) == Tristeon::Vector{ 3, 6 });
	}

	SUBCASE("Sign")
	{
		std::vector<float> options = { -1, 0, 1, -15, 23, INFINITY, -INFINITY };
		std::vector<float> answers = { -1, 0, 1, -1,  1,  1,		 -1, };

		for (uint32_t i = 0; i < options.size(); i++)
			REQUIRE(Tristeon::Math::sign(options[i]) == answers[i]);
	}
}