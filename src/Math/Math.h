#pragma once
#include <cstdint>

namespace Tristeon
{
	/**
	 * Math utility class, provides useful functions that might otherwise require more implementation time.
	 */
	class Math
	{
	public:
		/**
		 * Converts a bitmask to an index
		 *
		 * E.g: 16 is the 5th bitmask so the function will return 5.
		 */
		static int maskToIndex(uint64_t mask);
	};
}
