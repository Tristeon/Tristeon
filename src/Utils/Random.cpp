#include "Random.h"
#include <stdexcept>

namespace Tristeon
{
	unsigned Random::generateUInt()
	{
		return generateNumber<unsigned int, std::uniform_int_distribution<unsigned int>>();
	}

	unsigned Random::generateUInt(const unsigned& min, const unsigned& max)
	{
		return generateNumber<unsigned int, std::uniform_int_distribution<unsigned int>>(min, max);
	}

	int Random::generateInt()
	{
		return generateNumber<int, std::uniform_int_distribution<int>>();
	}

	int Random::generateInt(const int& min, const int& max)
	{
		return generateNumber<int, std::uniform_int_distribution<int>>(min, max);
	}

	float Random::generateFloat01()
	{
		return generateNumber<float, std::uniform_real_distribution<float>>(0, 1);
	}

	float Random::generateFloat(const float& min, const float& max)
	{
		return generateNumber<float, std::uniform_real_distribution<float>>(min, max);
	}

	double Random::generateDouble01()
	{
		return generateNumber<double, std::uniform_real_distribution<double>>(0, 1);
	}

	double Random::generateDouble(const double& min, const double& max)
	{
		return generateNumber<double, std::uniform_real_distribution<double>>(min, max);
	}

	bool Random::generateBool()
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::bernoulli_distribution ber{};
		return ber(rng);
	}

	String Random::generateString(const unsigned int& length)
	{
		if (length == 0)
			throw std::invalid_argument("The length parameter of StringUtils::generateRandom must be > 0!");

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		static const int size = sizeof(alphanum);

		String output;
		for (auto i = 0u; i < length; ++i) {
			output.push_back(alphanum[generateInt() % (size - 1)]);
		}

		return output;
	}
}