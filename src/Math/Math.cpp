#include "Math.h"

namespace Tristeon
{
	int Math::maskToIndex(uint64_t mask)
	{
		const int MAX_BIT = 40;
		int result = MAX_BIT;

		while (mask != 0)
		{
			mask >>= 1;
			result -= 1;
		}

		return MAX_BIT - result;
	}
}
