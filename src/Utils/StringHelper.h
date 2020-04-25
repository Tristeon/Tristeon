#pragma once
#include <json.h>
#include <TypeDefinitions.h>

namespace Tristeon
{
	class StringHelper
	{
	public:
		/**
		 * Changes all of the string's characters to lowercase characters.
		 */
		static String toLower(String& string);

		/**
		 * Splits a string into separate strings based on the given delimiter.
		 */
		static Vector<String> split(String const& string, char const& delim);

		/**
		 * Generates a string with random characters with a given length.
		 *
		 * \exception invalid_argument if length == 0
		 */
		static String generateRandom(unsigned int const& length);
	private:
		static void internalSplit(const std::string& s, char const& delim, Vector<String>& result);
	};
}
