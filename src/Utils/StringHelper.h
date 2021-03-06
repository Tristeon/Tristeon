#pragma once
#include <Standard/List.h>
#include <Standard/String.h>

namespace Tristeon
{
	class StringHelper
	{
	public:
		/**
		 * Changes all of the string's characters to lowercase characters.
		 */
		[[nodiscard]] static String toLower(String string);

		/**
		 * Splits a string into separate strings based on the given delimiter.
		 */
		[[nodiscard]] static List<String> split(const String& string, const char& delim);
	private:
		static void internalSplit(const String& s, const char& delim, List<String>& result);
	};
}
