#pragma once
#include <random>
#include "TypeDefinitions.h"

namespace Tristeon
{
	/**
	 * Random class that uses the Mersenne Twister 19937 generator together with a std::uniform_x_distribution to generate proper random values.
	 */
	class Random
	{
	public:
		[[nodiscard]] static unsigned int generateUInt();
		[[nodiscard]] static unsigned int generateUInt(const unsigned int& min, const unsigned int& max);

		[[nodiscard]] static int generateInt();
		[[nodiscard]] static int generateInt(const int& min, const int& max);

		[[nodiscard]] static float generateFloat01();
		[[nodiscard]] static float generateFloat(const float& min, const float& max);

		[[nodiscard]] static double generateDouble01();
		[[nodiscard]] static double generateDouble(const double& min, const double& max);

		[[nodiscard]] static bool generateBool();

		[[nodiscard]] static String generateString(const unsigned int& length);
	private:
		template<typename T, typename D>
		[[nodiscard]] static T generateNumber()
		{
			return generateNumber<T, D>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
		}
		
		template<typename T, typename D>
		[[nodiscard]] static T generateNumber(const T& min, const T& max)
		{
			std::random_device rd;
			std::mt19937 m(rd());
			D distribution(min, max);
			return distribution(m);
		}
	};
}
