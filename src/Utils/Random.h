#pragma once
#include <random>
#include <algorithm>

namespace Tristeon
{
	class Random
	{
	public:
		static unsigned int generateUInt()
		{
			return generateNumber<unsigned int, std::uniform_int_distribution<unsigned int>>();
		}
		
		static unsigned int generateUInt(const unsigned int& min, const unsigned int& max)
		{
			return generateNumber<unsigned int, std::uniform_int_distribution<unsigned int>>(min, max);
		}

		static int generateInt()
		{
			return generateNumber<int, std::uniform_int_distribution<int>>();
		}

		static int generateInt(const int& min, const int& max)
		{
			return generateNumber<int, std::uniform_int_distribution<int>>(min, max);
		}

		static float generateFloat01()
		{
			return generateNumber<float, std::uniform_real_distribution<float>>(0, 1);
		}

		static float generateFloat(const float& min, const float& max)
		{
			return generateNumber<float, std::uniform_real_distribution<float>>(min, max);
		}

		static double generateDouble01()
		{
			return generateNumber<double, std::uniform_real_distribution<double>>(0, 1);
		}

		static double generateDouble(const double& min, const double& max)
		{
			return generateNumber<double, std::uniform_real_distribution<double>>(min, max);
		}
		
		static bool generateBool()
		{
			std::random_device rd;
			std::mt19937 rng(rd());
			std::bernoulli_distribution ber{};
			return ber(rng);
		}

	private:
		template<typename T, typename D>
		static T generateNumber()
		{
			return generateNumber<T, D>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
		}
		
		template<typename T, typename D>
		static T generateNumber(const T& min, const T& max)
		{
			std::random_device rd;
			std::mt19937 rng(rd());
			D uni(min, max);
			return uni(rng);
		}
	};
}